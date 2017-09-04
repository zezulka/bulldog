#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "bcm.h"

// This define enables a little test program (by default a blinking output on pin RPI_GPIO_PIN_11)
// You can do some safe, non-destructive testing on any platform with:
// gcc bcm.c -D BCM_TEST
// ./a.out
//#define BCM_TEST

// Physical addresses for various peripheral register sets

uint32_t BCM_PERI_BASE;
uint32_t BCM_ST_BASE;
uint32_t BCM_GPIO_PADS;
uint32_t BCM_CLOCK_BASE;
uint32_t BCM_GPIO_BASE;
uint32_t BCM_SPI0_BASE;
uint32_t BCM_BSC0_BASE;
uint32_t BCM_GPIO_PWM;
uint32_t BCM_BSC1_BASE;

// Pointers to the hardware register bases
volatile uint32_t *bcm_gpio = MAP_FAILED;
volatile uint32_t *bcm_pwm  = MAP_FAILED;
volatile uint32_t *bcm_clk  = MAP_FAILED;
volatile uint32_t *bcm_pads = MAP_FAILED;
volatile uint32_t *bcm_spi0 = MAP_FAILED;
volatile uint32_t *bcm_bsc0 = MAP_FAILED;
volatile uint32_t *bcm_bsc1 = MAP_FAILED;
volatile uint32_t *bcm_st	= MAP_FAILED;


// This variable allows us to test on hardware other than RPi.
// It prevents access to the kernel memory, and does not do any peripheral access
// Instead it prints out what it _would_ do if debug were 0
static uint8_t debug = 0;

// I2C The time needed to transmit one byte. In microseconds.
static int i2c_byte_wait_us = 0;

//
// Low level register access functions
//

void  bcm_set_debug(uint8_t d)
{
    debug = d;
}

// safe read from peripheral
uint32_t bcm_peri_read(volatile uint32_t* paddr)
{
    if (debug)
    {
        printf("bcm_peri_read  paddr %08X\n", (unsigned) paddr);
	return 0;
    }
    else
    {
	// Make sure we dont return the _last_ read which might get lost
	// if subsequent code changes to a different peripheral
	uint32_t ret = *paddr;
	uint32_t dummy = *paddr;
	return ret;
    }
}

// read from peripheral without the read barrier
uint32_t bcm_peri_read_nb(volatile uint32_t* paddr)
{
    if (debug)
    {
	printf("bcm_peri_read_nb  paddr %08X\n", (unsigned) paddr);
	return 0;
    }
    else
    {
	return *paddr;
    }
}

// safe write to peripheral
void bcm_peri_write(volatile uint32_t* paddr, uint32_t value)
{
    if (debug)
    {
	printf("bcm_peri_write paddr %08X, value %08X\n", (unsigned) paddr, value);
    }
    else
    {
	// Make sure we don't rely on the first write, which may get
	// lost if the previous access was to a different peripheral.
	*paddr = value;
	*paddr = value;
    }
}

// write to peripheral without the write barrier
void bcm_peri_write_nb(volatile uint32_t* paddr, uint32_t value)
{
    if (debug)
    {
	printf("bcm_peri_write_nb paddr %08X, value %08X\n",
               (unsigned) paddr, value);
    }
    else
    {
	*paddr = value;
    }
}

// Set/clear only the bits in value covered by the mask
void bcm_peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask)
{
    uint32_t v = bcm_peri_read(paddr);
    v = (v & ~mask) | (value & mask);
    bcm_peri_write(paddr, v);
}

//
// Low level convenience functions
//

// Function select
// pin is a BCM GPIO pin number NOT RPi pin number
//      There are 6 control registers, each control the functions of a block
//      of 10 pins.
//      Each control register has 10 sets of 3 bits per GPIO pin:
//
//      000 = GPIO Pin X is an input
//      001 = GPIO Pin X is an output
//      100 = GPIO Pin X takes alternate function 0
//      101 = GPIO Pin X takes alternate function 1
//      110 = GPIO Pin X takes alternate function 2
//      111 = GPIO Pin X takes alternate function 3
//      011 = GPIO Pin X takes alternate function 4
//      010 = GPIO Pin X takes alternate function 5
//
// So the 3 bits for port X are:
//      X / 10 + ((X % 10) * 3)
void bcm_gpio_fsel(uint8_t pin, uint8_t mode)
{
    // Function selects are 10 pins per 32 bit word, 3 bits per pin
    volatile uint32_t* paddr = bcm_gpio + BCM_GPFSEL0/4 + (pin/10);
    uint8_t   shift = (pin % 10) * 3;
    uint32_t  mask = BCM_GPIO_FSEL_MASK << shift;
    uint32_t  value = mode << shift;
    bcm_peri_set_bits(paddr, value, mask);
}

// Set output pin
void bcm_gpio_set(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPSET0/4 + pin/32;
    uint8_t shift = pin % 32;
    bcm_peri_write(paddr, 1 << shift);
}

// Clear output pin
void bcm_gpio_clr(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPCLR0/4 + pin/32;
    uint8_t shift = pin % 32;
    bcm_peri_write(paddr, 1 << shift);
}

// Set all output pins in the mask
void bcm_gpio_set_multi(uint32_t mask)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPSET0/4;
    bcm_peri_write(paddr, mask);
}

// Clear all output pins in the mask
void bcm_gpio_clr_multi(uint32_t mask)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPCLR0/4;
    bcm_peri_write(paddr, mask);
}

// Read input pin
uint8_t bcm_gpio_lev(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPLEV0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = bcm_peri_read(paddr);
    return (value & (1 << shift)) ? HIGH : LOW;
}

// See if an event detection bit is set
// Sigh cant support interrupts yet
uint8_t bcm_gpio_eds(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPEDS0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = bcm_peri_read(paddr);
    return (value & (1 << shift)) ? HIGH : LOW;
}

// Write a 1 to clear the bit in EDS
void bcm_gpio_set_eds(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPEDS0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_write(paddr, value);
}

// Rising edge detect enable
void bcm_gpio_ren(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPREN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, value, value);
}
void bcm_gpio_clr_ren(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPREN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, 0, value);
}

// Falling edge detect enable
void bcm_gpio_fen(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPFEN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, value, value);
}
void bcm_gpio_clr_fen(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPFEN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, 0, value);
}

// High detect enable
void bcm_gpio_hen(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPHEN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, value, value);
}
void bcm_gpio_clr_hen(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPHEN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, 0, value);
}

// Low detect enable
void bcm_gpio_len(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPLEN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, value, value);
}
void bcm_gpio_clr_len(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPLEN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, 0, value);
}

// Async rising edge detect enable
void bcm_gpio_aren(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPAREN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, value, value);
}
void bcm_gpio_clr_aren(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPAREN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, 0, value);
}

// Async falling edge detect enable
void bcm_gpio_afen(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPAFEN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, value, value);
}
void bcm_gpio_clr_afen(uint8_t pin)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPAFEN0/4 + pin/32;
    uint8_t shift = pin % 32;
    uint32_t value = 1 << shift;
    bcm_peri_set_bits(paddr, 0, value);
}

// Set pullup/down
void bcm_gpio_pud(uint8_t pud)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPPUD/4;
    bcm_peri_write(paddr, pud);
}

// Pullup/down clock
// Clocks the value of pud into the GPIO pin
void bcm_gpio_pudclk(uint8_t pin, uint8_t on)
{
    volatile uint32_t* paddr = bcm_gpio + BCM_GPPUDCLK0/4 + pin/32;
    uint8_t shift = pin % 32;
    bcm_peri_write(paddr, (on ? 1 : 0) << shift);
}

// Read GPIO pad behaviour for groups of GPIOs
uint32_t bcm_gpio_pad(uint8_t group)
{
    volatile uint32_t* paddr = bcm_pads + BCM_PADS_GPIO_0_27/4 + group*2;
    return bcm_peri_read(paddr);
}

// Set GPIO pad behaviour for groups of GPIOs
// powerup value for al pads is
// BCM_PAD_SLEW_RATE_UNLIMITED | BCM_PAD_HYSTERESIS_ENABLED | BCM_PAD_DRIVE_8mA
void bcm_gpio_set_pad(uint8_t group, uint32_t control)
{
    volatile uint32_t* paddr = bcm_pads + BCM_PADS_GPIO_0_27/4 + group*2;
    bcm_peri_write(paddr, control);
}

// Some convenient arduino-like functions
// milliseconds
void bcm_delay(unsigned int millis)
{
    struct timespec sleeper;

    sleeper.tv_sec  = (time_t)(millis / 1000);
    sleeper.tv_nsec = (long)(millis % 1000) * 1000000;
    nanosleep(&sleeper, NULL);
}

// microseconds
void bcm_delayMicroseconds(uint64_t micros)
{
    struct timespec t1;
    uint64_t        start;

    // Calling nanosleep() takes at least 100-200 us, so use it for
    // long waits and use a busy wait on the System Timer for the rest.
    start =  bcm_st_read();

    if (micros > 450)
    {
	t1.tv_sec = 0;
	t1.tv_nsec = 1000 * (long)(micros - 200);
	nanosleep(&t1, NULL);
    }

    bcm_st_delay(start, micros);
}

//
// Higher level convenience functions
//

// Set the state of an output
void bcm_gpio_write(uint8_t pin, uint8_t on)
{
    if (on)
	bcm_gpio_set(pin);
    else
	bcm_gpio_clr(pin);
}

// Set the state of a all 32 outputs in the mask to on or off
void bcm_gpio_write_multi(uint32_t mask, uint8_t on)
{
    if (on)
	bcm_gpio_set_multi(mask);
    else
	bcm_gpio_clr_multi(mask);
}

// Set the state of a all 32 outputs in the mask to the values in value
void bcm_gpio_write_mask(uint32_t value, uint32_t mask)
{
    bcm_gpio_set_multi(value & mask);
    bcm_gpio_clr_multi((~value) & mask);
}

// Set the pullup/down resistor for a pin
//
// The GPIO Pull-up/down Clock Registers control the actuation of internal pull-downs on
// the respective GPIO pins. These registers must be used in conjunction with the GPPUD
// register to effect GPIO Pull-up/down changes. The following sequence of events is
// required:
// 1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
// to remove the current Pull-up/down)
// 2. Wait 150 cycles ? this provides the required set-up time for the control signal
// 3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
// modify ? NOTE only the pads which receive a clock will be modified, all others will
// retain their previous state.
// 4. Wait 150 cycles ? this provides the required hold time for the control signal
// 5. Write to GPPUD to remove the control signal
// 6. Write to GPPUDCLK0/1 to remove the clock
//
// RPi has P1-03 and P1-05 with 1k8 pullup resistor
void bcm_gpio_set_pud(uint8_t pin, uint8_t pud)
{
    bcm_gpio_pud(pud);
    delayMicroseconds(10);
    bcm_gpio_pudclk(pin, 1);
    delayMicroseconds(10);
    bcm_gpio_pud(BCM_GPIO_PUD_OFF);
    bcm_gpio_pudclk(pin, 0);
}

void bcm_spi_begin(void)
{
    // Set the SPI0 pins to the Alt 0 function to enable SPI0 access on them
    bcm_gpio_fsel(RPI_GPIO_P1_26, BCM_GPIO_FSEL_ALT0); // CE1
    bcm_gpio_fsel(RPI_GPIO_P1_24, BCM_GPIO_FSEL_ALT0); // CE0
    bcm_gpio_fsel(RPI_GPIO_P1_21, BCM_GPIO_FSEL_ALT0); // MISO
    bcm_gpio_fsel(RPI_GPIO_P1_19, BCM_GPIO_FSEL_ALT0); // MOSI
    bcm_gpio_fsel(RPI_GPIO_P1_23, BCM_GPIO_FSEL_ALT0); // CLK

    // Set the SPI CS register to the some sensible defaults
    volatile uint32_t* paddr = bcm_spi0 + BCM_SPI0_CS/4;
    bcm_peri_write(paddr, 0); // All 0s

    // Clear TX and RX fifos
    bcm_peri_write_nb(paddr, BCM_SPI0_CS_CLEAR);
}

void bcm_spi_end(void)
{
    // Set all the SPI0 pins back to input
    bcm_gpio_fsel(RPI_GPIO_P1_26, BCM_GPIO_FSEL_INPT); // CE1
    bcm_gpio_fsel(RPI_GPIO_P1_24, BCM_GPIO_FSEL_INPT); // CE0
    bcm_gpio_fsel(RPI_GPIO_P1_21, BCM_GPIO_FSEL_INPT); // MISO
    bcm_gpio_fsel(RPI_GPIO_P1_19, BCM_GPIO_FSEL_INPT); // MOSI
    bcm_gpio_fsel(RPI_GPIO_P1_23, BCM_GPIO_FSEL_INPT); // CLK
}

void bcm_spi_setBitOrder(uint8_t order)
{
    // BCM_SPI_BIT_ORDER_MSBFIRST is the only one suported by SPI0
}

// defaults to 0, which means a divider of 65536.
// The divisor must be a power of 2. Odd numbers
// rounded down. The maximum SPI clock rate is
// of the APB clock
void bcm_spi_setClockDivider(uint16_t divider)
{
    volatile uint32_t* paddr = bcm_spi0 + BCM_SPI0_CLK/4;
    bcm_peri_write(paddr, divider);
}

void bcm_spi_setDataMode(uint8_t mode)
{
    volatile uint32_t* paddr = bcm_spi0 + BCM_SPI0_CS/4;
    // Mask in the CPO and CPHA bits of CS
    bcm_peri_set_bits(paddr, mode << 2, BCM_SPI0_CS_CPOL | BCM_SPI0_CS_CPHA);
}

// Writes (and reads) a single byte to SPI
uint8_t bcm_spi_transfer(uint8_t value)
{
    volatile uint32_t* paddr = bcm_spi0 + BCM_SPI0_CS/4;
    volatile uint32_t* fifo = bcm_spi0 + BCM_SPI0_FIFO/4;

    // This is Polled transfer as per section 10.6.1
    // BUG ALERT: what happens if we get interupted in this section, and someone else
    // accesses a different peripheral?
    // Clear TX and RX fifos
    bcm_peri_set_bits(paddr, BCM_SPI0_CS_CLEAR, BCM_SPI0_CS_CLEAR);

    // Set TA = 1
    bcm_peri_set_bits(paddr, BCM_SPI0_CS_TA, BCM_SPI0_CS_TA);

    // Maybe wait for TXD
    while (!(bcm_peri_read(paddr) & BCM_SPI0_CS_TXD))
	delayMicroseconds(10);

    // Write to FIFO, no barrier
    bcm_peri_write_nb(fifo, value);

    // Wait for DONE to be set
    while (!(bcm_peri_read_nb(paddr) & BCM_SPI0_CS_DONE))
	delayMicroseconds(10);

    // Read any byte that was sent back by the slave while we sere sending to it
    uint32_t ret = bcm_peri_read_nb(fifo);

    // Set TA = 0, and also set the barrier
    bcm_peri_set_bits(paddr, 0, BCM_SPI0_CS_TA);

    return ret;
}

// Writes (and reads) an number of bytes to SPI
void bcm_spi_transfernb(char* tbuf, char* rbuf, uint32_t len)
{
    volatile uint32_t* paddr = bcm_spi0 + BCM_SPI0_CS/4;
    volatile uint32_t* fifo = bcm_spi0 + BCM_SPI0_FIFO/4;

    // This is Polled transfer as per section 10.6.1
    // BUG ALERT: what happens if we get interupted in this section, and someone else
    // accesses a different peripheral?

    // Clear TX and RX fifos
    bcm_peri_set_bits(paddr, BCM_SPI0_CS_CLEAR, BCM_SPI0_CS_CLEAR);

    // Set TA = 1
    bcm_peri_set_bits(paddr, BCM_SPI0_CS_TA, BCM_SPI0_CS_TA);

    uint32_t i;
    for (i = 0; i < len; i++)
    {
	// Maybe wait for TXD
	while (!(bcm_peri_read(paddr) & BCM_SPI0_CS_TXD))
	    delayMicroseconds(10);

	// Write to FIFO, no barrier
	bcm_peri_write_nb(fifo, tbuf[i]);

	// Wait for RXD
	while (!(bcm_peri_read(paddr) & BCM_SPI0_CS_RXD))
	    delayMicroseconds(10);

	// then read the data byte
	rbuf[i] = bcm_peri_read_nb(fifo);
    }
    // Wait for DONE to be set
    while (!(bcm_peri_read_nb(paddr) & BCM_SPI0_CS_DONE))
	delayMicroseconds(10);

    // Set TA = 0, and also set the barrier
    bcm_peri_set_bits(paddr, 0, BCM_SPI0_CS_TA);
}

// Writes an number of bytes to SPI
void bcm_spi_writenb(char* tbuf, uint32_t len)
{
    volatile uint32_t* paddr = bcm_spi0 + BCM_SPI0_CS/4;
    volatile uint32_t* fifo = bcm_spi0 + BCM_SPI0_FIFO/4;

    // This is Polled transfer as per section 10.6.1
    // BUG ALERT: what happens if we get interupted in this section, and someone else
    // accesses a different peripheral?

    // Clear TX and RX fifos
    bcm_peri_set_bits(paddr, BCM_SPI0_CS_CLEAR, BCM_SPI0_CS_CLEAR);

    // Set TA = 1
    bcm_peri_set_bits(paddr, BCM_SPI0_CS_TA, BCM_SPI0_CS_TA);

    uint32_t i;
	for (i = 0; i < len; i++)
	{
		// Maybe wait for TXD
		while (!(bcm_peri_read(paddr) & BCM_SPI0_CS_TXD))
			;

		// Write to FIFO, no barrier
		bcm_peri_write_nb(fifo, tbuf[i]);
	}

    // Wait for DONE to be set
    while (!(bcm_peri_read_nb(paddr) & BCM_SPI0_CS_DONE))
    	;

    // Set TA = 0, and also set the barrier
    bcm_peri_set_bits(paddr, 0, BCM_SPI0_CS_TA);
}

// Writes (and reads) an number of bytes to SPI
// Read bytes are copied over onto the transmit buffer
void bcm_spi_transfern(char* buf, uint32_t len)
{
    bcm_spi_transfernb(buf, buf, len);
}

void bcm_spi_chipSelect(uint8_t cs)
{
    volatile uint32_t* paddr = bcm_spi0 + BCM_SPI0_CS/4;
    // Mask in the CS bits of CS
    bcm_peri_set_bits(paddr, cs, BCM_SPI0_CS_CS);
}

void bcm_spi_setChipSelectPolarity(uint8_t cs, uint8_t active)
{
    volatile uint32_t* paddr = bcm_spi0 + BCM_SPI0_CS/4;
    uint8_t shift = 21 + cs;
    // Mask in the appropriate CSPOLn bit
    bcm_peri_set_bits(paddr, active << shift, 1 << shift);
}

void bcm_i2c_begin(void)
{
	volatile uint32_t* paddr = bcm_bsc1 + BCM_BSC_DIV/4;

    // Set the I2C/BSC1 pins to the Alt 0 function to enable I2C access on them
    bcm_gpio_fsel(RPI_V2_GPIO_P1_03, BCM_GPIO_FSEL_ALT0); // SDA
    bcm_gpio_fsel(RPI_V2_GPIO_P1_05, BCM_GPIO_FSEL_ALT0); // SCL

    // Read the clock divider register
    uint16_t cdiv = bcm_peri_read(paddr);
    // Calculate time for transmitting one byte
    // 1000000 = micros seconds in a second
    // 9 = Clocks per byte : 8 bits + ACK
    i2c_byte_wait_us = ((float)cdiv / BCM_CORE_CLK_HZ) * 1000000 * 9;
}

void bcm_i2c_end(void)
{
    // Set all the I2C/BSC1 pins back to input
    bcm_gpio_fsel(RPI_V2_GPIO_P1_03, BCM_GPIO_FSEL_INPT); // SDA
    bcm_gpio_fsel(RPI_V2_GPIO_P1_05, BCM_GPIO_FSEL_INPT); // SCL
}

void bcm_i2c_setSlaveAddress(uint8_t addr)
{
	// Set I2C Device Address
	volatile uint32_t* paddr = bcm_bsc1 + BCM_BSC_A/4;
	bcm_peri_write(paddr, addr);
}

// defaults to 0x5dc, should result in a 166.666 kHz I2C clock frequency.
// The divisor must be a power of 2. Odd numbers
// rounded down.
void bcm_i2c_setClockDivider(uint16_t divider)
{
    volatile uint32_t* paddr = bcm_bsc1 + BCM_BSC_DIV/4;
    bcm_peri_write(paddr, divider);
    // Calculate time for transmitting one byte
    // 1000000 = micros seconds in a second
    // 9 = Clocks per byte : 8 bits + ACK
    i2c_byte_wait_us = ((float)divider / BCM_CORE_CLK_HZ) * 1000000 * 9;
}

// set I2C clock divider by means of a baudrate number
void bcm_i2c_set_baudrate(uint32_t baudrate)
{
	uint32_t divider;
	// use 0xFFFE mask to limit a max value and round down any odd number
	divider = (BCM_CORE_CLK_HZ / baudrate) & 0xFFFE;
	bcm_i2c_setClockDivider( (uint16_t)divider );
}

// Writes an number of bytes to I2C
uint8_t bcm_i2c_write(const char * buf, uint32_t len)
{
    volatile uint32_t* dlen    = bcm_bsc1 + BCM_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm_bsc1 + BCM_BSC_FIFO/4;
    volatile uint32_t* status  = bcm_bsc1 + BCM_BSC_S/4;
    volatile uint32_t* control = bcm_bsc1 + BCM_BSC_C/4;

    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = BCM_I2C_REASON_OK;

    // Clear FIFO
    bcm_peri_set_bits(control, BCM_BSC_C_CLEAR_1 , BCM_BSC_C_CLEAR_1 );
    // Clear Status
	bcm_peri_write_nb(status, BCM_BSC_S_CLKT | BCM_BSC_S_ERR | BCM_BSC_S_DONE);
	// Set Data Length
    bcm_peri_write_nb(dlen, len);
    // pre populate FIFO with max buffer
    while( remaining && ( i < BCM_BSC_FIFO_SIZE ) )
    {
        bcm_peri_write_nb(fifo, buf[i]);
        i++;
        remaining--;
    }

    // Enable device and start transfer
    bcm_peri_write_nb(control, BCM_BSC_C_I2CEN | BCM_BSC_C_ST);

    // Transfer is over when BCM_BSC_S_DONE
    while(!(bcm_peri_read_nb(status) & BCM_BSC_S_DONE ))
    {
        while ( remaining && (bcm_peri_read_nb(status) & BCM_BSC_S_TXD ))
    	{
        	// Write to FIFO, no barrier
        	bcm_peri_write_nb(fifo, buf[i]);
        	i++;
        	remaining--;
    	}
    }

    // Received a NACK
    if (bcm_peri_read(status) & BCM_BSC_S_ERR)
    {
		reason = BCM_I2C_REASON_ERROR_NACK;
    }

    // Received Clock Stretch Timeout
    else if (bcm_peri_read(status) & BCM_BSC_S_CLKT)
    {
		reason = BCM_I2C_REASON_ERROR_CLKT;
    }

    // Not all data is sent
    else if (remaining)
    {
		reason = BCM_I2C_REASON_ERROR_DATA;
    }

    bcm_peri_set_bits(control, BCM_BSC_S_DONE , BCM_BSC_S_DONE);

    return reason;
}

// Read an number of bytes from I2C
uint8_t bcm_i2c_read(char* buf, uint32_t len)
{
    volatile uint32_t* dlen    = bcm_bsc1 + BCM_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm_bsc1 + BCM_BSC_FIFO/4;
    volatile uint32_t* status  = bcm_bsc1 + BCM_BSC_S/4;
    volatile uint32_t* control = bcm_bsc1 + BCM_BSC_C/4;

    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = BCM_I2C_REASON_OK;

    // Clear FIFO
    bcm_peri_set_bits(control, BCM_BSC_C_CLEAR_1 , BCM_BSC_C_CLEAR_1 );
    // Clear Status
	  bcm_peri_write_nb(status, BCM_BSC_S_CLKT | BCM_BSC_S_ERR | BCM_BSC_S_DONE);
	  // Set Data Length
    bcm_peri_write_nb(dlen, len);
    // Start read
    bcm_peri_write_nb(control, BCM_BSC_C_I2CEN | BCM_BSC_C_ST | BCM_BSC_C_READ);

    // wait for transfer to complete
    while (!(bcm_peri_read_nb(status) & BCM_BSC_S_DONE))
    {
        // we must empty the FIFO as it is populated and not use any delay
        while (bcm_peri_read_nb(status) & BCM_BSC_S_RXD)
    	{
    		// Read from FIFO, no barrier
    		buf[i] = bcm_peri_read_nb(fifo);
        	i++;
        	remaining--;
    	}
    }

    // transfer has finished - grab any remaining stuff in FIFO
    while (remaining && (bcm_peri_read_nb(status) & BCM_BSC_S_RXD))
    {
        // Read from FIFO, no barrier
        buf[i] = bcm_peri_read_nb(fifo);
        i++;
        remaining--;
    }

    // Received a NACK
    if (bcm_peri_read(status) & BCM_BSC_S_ERR)
    {
		reason = BCM_I2C_REASON_ERROR_NACK;
    }

    // Received Clock Stretch Timeout
    else if (bcm_peri_read(status) & BCM_BSC_S_CLKT)
    {
		reason = BCM_I2C_REASON_ERROR_CLKT;
    }

    // Not all data is received
    else if (remaining)
    {
		reason = BCM_I2C_REASON_ERROR_DATA;
    }

    bcm_peri_set_bits(control, BCM_BSC_S_DONE , BCM_BSC_S_DONE);

    return reason;
}

// Read an number of bytes from I2C sending a repeated start after writing
// the required register. Only works if your device supports this mode
uint8_t bcm_i2c_read_register_rs(char* regaddr, char* buf, uint32_t len)
{
    volatile uint32_t* dlen    = bcm_bsc1 + BCM_BSC_DLEN/4;
    volatile uint32_t* fifo    = bcm_bsc1 + BCM_BSC_FIFO/4;
    volatile uint32_t* status  = bcm_bsc1 + BCM_BSC_S/4;
    volatile uint32_t* control = bcm_bsc1 + BCM_BSC_C/4;

	  uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = BCM_I2C_REASON_OK;

    // Clear FIFO
    bcm_peri_set_bits(control, BCM_BSC_C_CLEAR_1 , BCM_BSC_C_CLEAR_1 );
    // Clear Status
	bcm_peri_write_nb(status, BCM_BSC_S_CLKT | BCM_BSC_S_ERR | BCM_BSC_S_DONE);
	// Set Data Length
    bcm_peri_write_nb(dlen, 1);
    // Enable device and start transfer
    bcm_peri_write_nb(control, BCM_BSC_C_I2CEN);
    bcm_peri_write_nb(fifo, regaddr[0]);
    bcm_peri_write_nb(control, BCM_BSC_C_I2CEN | BCM_BSC_C_ST);

    // poll for transfer has started
    while ( !( bcm_peri_read_nb(status) & BCM_BSC_S_TA ) )
    {
        // Linux may cause us to miss entire transfer stage
        if(bcm_peri_read(status) & BCM_BSC_S_DONE)
            break;
    }

    // Send a repeated start with read bit set in address
    bcm_peri_write_nb(dlen, len);
    bcm_peri_write_nb(control, BCM_BSC_C_I2CEN | BCM_BSC_C_ST  | BCM_BSC_C_READ );

    // Wait for write to complete and first byte back.
    bcm_delayMicroseconds(i2c_byte_wait_us * 3);

    // wait for transfer to complete
    while (!(bcm_peri_read_nb(status) & BCM_BSC_S_DONE))
    {
        // we must empty the FIFO as it is populated and not use any delay
        while (remaining && bcm_peri_read_nb(status) & BCM_BSC_S_RXD)
    	{
    		// Read from FIFO, no barrier
    		buf[i] = bcm_peri_read_nb(fifo);
        	i++;
        	remaining--;
    	}
    }

    // transfer has finished - grab any remaining stuff in FIFO
    while (remaining && (bcm_peri_read_nb(status) & BCM_BSC_S_RXD))
    {
        // Read from FIFO, no barrier
        buf[i] = bcm_peri_read_nb(fifo);
        i++;
        remaining--;
    }

    // Received a NACK
    if (bcm_peri_read(status) & BCM_BSC_S_ERR)
    {
		reason = BCM_I2C_REASON_ERROR_NACK;
    }

    // Received Clock Stretch Timeout
    else if (bcm_peri_read(status) & BCM_BSC_S_CLKT)
    {
		reason = BCM_I2C_REASON_ERROR_CLKT;
    }

    // Not all data is sent
    else if (remaining)
    {
		reason = BCM_I2C_REASON_ERROR_DATA;
    }

    bcm_peri_set_bits(control, BCM_BSC_S_DONE , BCM_BSC_S_DONE);

    return reason;
}

// Read the System Timer Counter (64-bits)
uint64_t bcm_st_read(void)
{
    volatile uint32_t* paddr;
    uint64_t st;
    paddr = bcm_st + BCM_ST_CHI/4;
    st = bcm_peri_read(paddr);
    st <<= 32;
    paddr = bcm_st + BCM_ST_CLO/4;
    st += bcm_peri_read(paddr);
    return st;
}

// Delays for the specified number of microseconds with offset
void bcm_st_delay(uint64_t offset_micros, uint64_t micros)
{
    uint64_t compare = offset_micros + micros;

    while(bcm_st_read() < compare)
	;
}

// Allocate page-aligned memory.
void *malloc_aligned(size_t size)
{
    void *mem;
    errno = posix_memalign(&mem, BCM_PAGE_SIZE, size);
    return (errno ? NULL : mem);
}

// Map 'size' bytes starting at 'off' in file 'fd' to memory.
// Return mapped address on success, MAP_FAILED otherwise.
// On error print message.
static void *mapmem(const char *msg, size_t size, int fd, off_t off)
{
    void *map = mmap(NULL, size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, off);
    if (MAP_FAILED == map)
	fprintf(stderr, "bcm_init: %s mmap failed: %s\n", msg, strerror(errno));
    return map;
}

static void unmapmem(void **pmem, size_t size)
{
    if (*pmem == MAP_FAILED) return;
    munmap(*pmem, size);
    *pmem = MAP_FAILED;
}

void init_constants(uint32_t peri_base) {
  BCM_PERI_BASE = peri_base;
  BCM_ST_BASE = BCM_PERI_BASE + 0x3000;
  BCM_GPIO_PADS = BCM_PERI_BASE + 0x100000;
  BCM_CLOCK_BASE = BCM_PERI_BASE + 0x101000;
  BCM_GPIO_BASE = BCM_PERI_BASE + 0x200000;
  BCM_SPI0_BASE = BCM_PERI_BASE + 0x204000;
  BCM_BSC0_BASE = BCM_PERI_BASE + 0x205000;
  BCM_GPIO_PWM = BCM_PERI_BASE + 0x20C000;
  BCM_BSC1_BASE = BCM_PERI_BASE + 0x804000;
}

// Initialise this library.
int bcm_init(uint32_t peri_base)
{
  init_constants(peri_base);
    if (debug)
    {
	bcm_pads = (uint32_t*)BCM_GPIO_PADS;
	bcm_clk = (uint32_t*)BCM_CLOCK_BASE;
	bcm_gpio = (uint32_t*)BCM_GPIO_BASE;
	bcm_pwm = (uint32_t*)BCM_GPIO_PWM;
	bcm_spi0 = (uint32_t*)BCM_SPI0_BASE;
	bcm_bsc0 = (uint32_t*)BCM_BSC0_BASE;
	bcm_bsc1 = (uint32_t*)BCM_BSC1_BASE;
	bcm_st   = (uint32_t*)BCM_ST_BASE;
	return 1; // Success
    }
    int memfd = -1;
    int ok = 0;


    if(getuid() == 0) {
             // Open the master /dev/memory device
             if ((memfd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0)
             {
                    fprintf(stderr, "bcm_init: Unable to open /dev/mem: %s\n",
                        strerror(errno)) ;
                    goto exit;
             }
    } else {
             // Enables library to use GPIO interface without root permissions
             if((memfd = open("/dev/gpiomem", O_RDWR | O_SYNC)) < 0)
             {
                 fprintf(stderr, "bcm_init: Unable to open /dev/gpiomem: %s\n",
                        strerror(errno)) ;
                 goto exit;          
             }
             // GPIO:
             bcm_gpio = mapmem("gpio", BCM_BLOCK_SIZE, memfd, BCM_GPIO_BASE);
             if (bcm_gpio == MAP_FAILED) goto exit;
             ok = 1;
             // We have to skip other memory mappings as they would be useless
             goto exit;
    }   

    // GPIO:
    bcm_gpio = mapmem("gpio", BCM_BLOCK_SIZE, memfd, BCM_GPIO_BASE);
    if (bcm_gpio == MAP_FAILED) goto exit;

    // PWM
    bcm_pwm = mapmem("pwm", BCM_BLOCK_SIZE, memfd, BCM_GPIO_PWM);
    if (bcm_pwm == MAP_FAILED) goto exit;

    // Clock control (needed for PWM)
    bcm_clk = mapmem("clk", BCM_BLOCK_SIZE, memfd, BCM_CLOCK_BASE);
    if (bcm_clk == MAP_FAILED) goto exit;

    bcm_pads = mapmem("pads", BCM_BLOCK_SIZE, memfd, BCM_GPIO_PADS);
    if (bcm_pads == MAP_FAILED) goto exit;

    bcm_spi0 = mapmem("spi0", BCM_BLOCK_SIZE, memfd, BCM_SPI0_BASE);
    if (bcm_spi0 == MAP_FAILED) goto exit;

    // I2C
    bcm_bsc0 = mapmem("bsc0", BCM_BLOCK_SIZE, memfd, BCM_BSC0_BASE);
    if (bcm_bsc0 == MAP_FAILED) goto exit;

    bcm_bsc1 = mapmem("bsc1", BCM_BLOCK_SIZE, memfd, BCM_BSC1_BASE);
    if (bcm_bsc1 == MAP_FAILED) goto exit;

    // ST
    bcm_st = mapmem("st", BCM_BLOCK_SIZE, memfd, BCM_ST_BASE);
    if (bcm_st == MAP_FAILED) goto exit;

    ok = 1;

exit:
    if (memfd >= 0)
        close(memfd);

    if (!ok)
	bcm_close();

    return ok;
}

// Close this library and deallocate everything
int bcm_close(void)
{
    if (debug) return 1; // Success
    unmapmem((void**) &bcm_gpio, BCM_BLOCK_SIZE);
    unmapmem((void**) &bcm_pwm,  BCM_BLOCK_SIZE);
    unmapmem((void**) &bcm_clk,  BCM_BLOCK_SIZE);
    unmapmem((void**) &bcm_spi0, BCM_BLOCK_SIZE);
    unmapmem((void**) &bcm_bsc0, BCM_BLOCK_SIZE);
    unmapmem((void**) &bcm_bsc1, BCM_BLOCK_SIZE);
    unmapmem((void**) &bcm_st,   BCM_BLOCK_SIZE);
    unmapmem((void**) &bcm_pads, BCM_BLOCK_SIZE);
    return 1; // Success
}
