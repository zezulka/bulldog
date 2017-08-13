// Defines for BCM
#ifndef BCM_H
#define BCM_H

#include <stdint.h>

/// \defgroup constants Constants for passing to and from library functions
/// The values here are designed to be passed to various functions in the bcm library.
/// @{


/// This means pin HIGH, true, 3.3volts on a pin.
#define HIGH 0x1
/// This means pin LOW, false, 0volts on a pin.
#define LOW  0x0

/// Speed of the core clock core_clk
#define BCM_CORE_CLK_HZ				250000000	///< 250 MHz

// Physical addresses for various peripheral register sets
/// Base Physical Address of the BCM peripheral registers
extern uint32_t BCM_PERI_BASE;
/// Base Physical Address of the System Timer registers
extern uint32_t BCM_ST_BASE;
/// Base Physical Address of the Pads registers
extern uint32_t BCM_GPIO_PADS;
/// Base Physical Address of the Clock/timer registers
extern uint32_t BCM_CLOCK_BASE;
/// Base Physical Address of the GPIO registers
extern uint32_t BCM_GPIO_BASE;
/// Base Physical Address of the SPI0 registers
extern uint32_t BCM_SPI0_BASE;
/// Base Physical Address of the BSC0 registers
extern uint32_t BCM_BSC0_BASE;
/// Base Physical Address of the PWM registers
extern uint32_t BCM_GPIO_PWM;
 /// Base Physical Address of the BSC1 registers
extern uint32_t BCM_BSC1_BASE;


/// Base of the ST (System Timer) registers.
/// Available after bcm_init has been called
extern volatile uint32_t *bcm_st;

/// Base of the GPIO registers.
/// Available after bcm_init has been called
extern volatile uint32_t *bcm_gpio;

/// Base of the PWM registers.
/// Available after bcm_init has been called
extern volatile uint32_t *bcm_pwm;

/// Base of the CLK registers.
/// Available after bcm_init has been called
extern volatile uint32_t *bcm_clk;

/// Base of the PADS registers.
/// Available after bcm_init has been called
extern volatile uint32_t *bcm_pads;

/// Base of the SPI0 registers.
/// Available after bcm_init has been called
extern volatile uint32_t *bcm_spi0;

/// Base of the BSC0 registers.
/// Available after bcm_init has been called
extern volatile uint32_t *bcm_bsc0;

/// Base of the BSC1 registers.
/// Available after bcm_init has been called
extern volatile uint32_t *bcm_bsc1;

/// Size of memory page on RPi
#define BCM_PAGE_SIZE               (4*1024)
/// Size of memory block on RPi
#define BCM_BLOCK_SIZE              (4*1024)


// Defines for GPIO
// The BCM has 54 GPIO pins.
//      BCM data sheet, Page 90 onwards.
/// GPIO register offsets from BCM_GPIO_BASE. Offsets into the GPIO Peripheral block in bytes per 6.1 Register View
#define BCM_GPFSEL0                      0x0000 ///< GPIO Function Select 0
#define BCM_GPFSEL1                      0x0004 ///< GPIO Function Select 1
#define BCM_GPFSEL2                      0x0008 ///< GPIO Function Select 2
#define BCM_GPFSEL3                      0x000c ///< GPIO Function Select 3
#define BCM_GPFSEL4                      0x0010 ///< GPIO Function Select 4
#define BCM_GPFSEL5                      0x0014 ///< GPIO Function Select 5
#define BCM_GPSET0                       0x001c ///< GPIO Pin Output Set 0
#define BCM_GPSET1                       0x0020 ///< GPIO Pin Output Set 1
#define BCM_GPCLR0                       0x0028 ///< GPIO Pin Output Clear 0
#define BCM_GPCLR1                       0x002c ///< GPIO Pin Output Clear 1
#define BCM_GPLEV0                       0x0034 ///< GPIO Pin Level 0
#define BCM_GPLEV1                       0x0038 ///< GPIO Pin Level 1
#define BCM_GPEDS0                       0x0040 ///< GPIO Pin Event Detect Status 0
#define BCM_GPEDS1                       0x0044 ///< GPIO Pin Event Detect Status 1
#define BCM_GPREN0                       0x004c ///< GPIO Pin Rising Edge Detect Enable 0
#define BCM_GPREN1                       0x0050 ///< GPIO Pin Rising Edge Detect Enable 1
#define BCM_GPFEN0                       0x0058 ///< GPIO Pin Falling Edge Detect Enable 0
#define BCM_GPFEN1                       0x005c ///< GPIO Pin Falling Edge Detect Enable 1
#define BCM_GPHEN0                       0x0064 ///< GPIO Pin High Detect Enable 0
#define BCM_GPHEN1                       0x0068 ///< GPIO Pin High Detect Enable 1
#define BCM_GPLEN0                       0x0070 ///< GPIO Pin Low Detect Enable 0
#define BCM_GPLEN1                       0x0074 ///< GPIO Pin Low Detect Enable 1
#define BCM_GPAREN0                      0x007c ///< GPIO Pin Async. Rising Edge Detect 0
#define BCM_GPAREN1                      0x0080 ///< GPIO Pin Async. Rising Edge Detect 1
#define BCM_GPAFEN0                      0x0088 ///< GPIO Pin Async. Falling Edge Detect 0
#define BCM_GPAFEN1                      0x008c ///< GPIO Pin Async. Falling Edge Detect 1
#define BCM_GPPUD                        0x0094 ///< GPIO Pin Pull-up/down Enable
#define BCM_GPPUDCLK0                    0x0098 ///< GPIO Pin Pull-up/down Enable Clock 0
#define BCM_GPPUDCLK1                    0x009c ///< GPIO Pin Pull-up/down Enable Clock 1

/// \brief bcmPortFunction
/// Port function select modes for bcm_gpio_fsel()
typedef enum
{
    BCM_GPIO_FSEL_INPT  = 0,   ///< Input
    BCM_GPIO_FSEL_OUTP  = 1,   ///< Output
    BCM_GPIO_FSEL_ALT0  = 4,   ///< Alternate function 0
    BCM_GPIO_FSEL_ALT1  = 5,   ///< Alternate function 1
    BCM_GPIO_FSEL_ALT2  = 6,   ///< Alternate function 2
    BCM_GPIO_FSEL_ALT3  = 7,   ///< Alternate function 3
    BCM_GPIO_FSEL_ALT4  = 3,   ///< Alternate function 4
    BCM_GPIO_FSEL_ALT5  = 2,   ///< Alternate function 5
    BCM_GPIO_FSEL_MASK  = 7    ///< Function select bits mask
} bcmFunctionSelect;

/// \brief bcmPUDControl
/// Pullup/Pulldown defines for bcm_gpio_pud()
typedef enum
{
    BCM_GPIO_PUD_OFF     = 0,   ///< Off ? disable pull-up/down
    BCM_GPIO_PUD_DOWN    = 1,   ///< Enable Pull Down control
    BCM_GPIO_PUD_UP      = 2    ///< Enable Pull Up control
} bcmPUDControl;

/// Pad control register offsets from BCM_GPIO_PADS
#define BCM_PADS_GPIO_0_27               0x002c ///< Pad control register for pads 0 to 27
#define BCM_PADS_GPIO_28_45              0x0030 ///< Pad control register for pads 28 to 45
#define BCM_PADS_GPIO_46_53              0x0034 ///< Pad control register for pads 46 to 53

/// Pad Control masks
#define BCM_PAD_PASSWRD                  (0x5A << 24)  ///< Password to enable setting pad mask
#define BCM_PAD_SLEW_RATE_UNLIMITED      0x10 ///< Slew rate unlimited
#define BCM_PAD_HYSTERESIS_ENABLED       0x08 ///< Hysteresis enabled
#define BCM_PAD_DRIVE_2mA                0x00 ///< 2mA drive current
#define BCM_PAD_DRIVE_4mA                0x01 ///< 4mA drive current
#define BCM_PAD_DRIVE_6mA                0x02 ///< 6mA drive current
#define BCM_PAD_DRIVE_8mA                0x03 ///< 8mA drive current
#define BCM_PAD_DRIVE_10mA               0x04 ///< 10mA drive current
#define BCM_PAD_DRIVE_12mA               0x05 ///< 12mA drive current
#define BCM_PAD_DRIVE_14mA               0x06 ///< 14mA drive current
#define BCM_PAD_DRIVE_16mA               0x07 ///< 16mA drive current

/// \brief bcmPadGroup
/// Pad group specification for bcm_gpio_pad()
typedef enum
{
    BCM_PAD_GROUP_GPIO_0_27         = 0, ///< Pad group for GPIO pads 0 to 27
    BCM_PAD_GROUP_GPIO_28_45        = 1, ///< Pad group for GPIO pads 28 to 45
    BCM_PAD_GROUP_GPIO_46_53        = 2  ///< Pad group for GPIO pads 46 to 53
} bcmPadGroup;

/// \brief GPIO Pin Numbers
///
/// Here we define Raspberry Pin GPIO pins on P1 in terms of the underlying BCM GPIO pin numbers.
/// These can be passed as a pin number to any function requiring a pin.
/// Not all pins on the RPi 26 bin IDE plug are connected to GPIO pins
/// and some can adopt an alternate function.
/// RPi version 2 has some slightly different pinouts, and these are values RPI_V2_*.
/// At bootup, pins 8 and 10 are set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
/// When SPI0 is in use (ie after bcm_spi_begin()), pins 19, 21, 23, 24, 26 are dedicated to SPI
/// and cant be controlled independently
typedef enum
{
    RPI_GPIO_P1_03        =  0,  ///< Version 1, Pin P1-03
    RPI_GPIO_P1_05        =  1,  ///< Version 1, Pin P1-05
    RPI_GPIO_P1_07        =  4,  ///< Version 1, Pin P1-07
    RPI_GPIO_P1_08        = 14,  ///< Version 1, Pin P1-08, defaults to alt function 0 UART0_TXD
    RPI_GPIO_P1_10        = 15,  ///< Version 1, Pin P1-10, defaults to alt function 0 UART0_RXD
    RPI_GPIO_P1_11        = 17,  ///< Version 1, Pin P1-11
    RPI_GPIO_P1_12        = 18,  ///< Version 1, Pin P1-12
    RPI_GPIO_P1_13        = 21,  ///< Version 1, Pin P1-13
    RPI_GPIO_P1_15        = 22,  ///< Version 1, Pin P1-15
    RPI_GPIO_P1_16        = 23,  ///< Version 1, Pin P1-16
    RPI_GPIO_P1_18        = 24,  ///< Version 1, Pin P1-18
    RPI_GPIO_P1_19        = 10,  ///< Version 1, Pin P1-19, MOSI when SPI0 in use
    RPI_GPIO_P1_21        =  9,  ///< Version 1, Pin P1-21, MISO when SPI0 in use
    RPI_GPIO_P1_22        = 25,  ///< Version 1, Pin P1-22
    RPI_GPIO_P1_23        = 11,  ///< Version 1, Pin P1-23, CLK when SPI0 in use
    RPI_GPIO_P1_24        =  8,  ///< Version 1, Pin P1-24, CE0 when SPI0 in use
    RPI_GPIO_P1_26        =  7,  ///< Version 1, Pin P1-26, CE1 when SPI0 in use

    // RPi Version 2
    RPI_V2_GPIO_P1_03     =  2,  ///< Version 2, Pin P1-03
    RPI_V2_GPIO_P1_05     =  3,  ///< Version 2, Pin P1-05
    RPI_V2_GPIO_P1_07     =  4,  ///< Version 2, Pin P1-07
    RPI_V2_GPIO_P1_08     = 14,  ///< Version 2, Pin P1-08, defaults to alt function 0 UART0_TXD
    RPI_V2_GPIO_P1_10     = 15,  ///< Version 2, Pin P1-10, defaults to alt function 0 UART0_RXD
    RPI_V2_GPIO_P1_11     = 17,  ///< Version 2, Pin P1-11
    RPI_V2_GPIO_P1_12     = 18,  ///< Version 2, Pin P1-12
    RPI_V2_GPIO_P1_13     = 27,  ///< Version 2, Pin P1-13
    RPI_V2_GPIO_P1_15     = 22,  ///< Version 2, Pin P1-15
    RPI_V2_GPIO_P1_16     = 23,  ///< Version 2, Pin P1-16
    RPI_V2_GPIO_P1_18     = 24,  ///< Version 2, Pin P1-18
    RPI_V2_GPIO_P1_19     = 10,  ///< Version 2, Pin P1-19, MOSI when SPI0 in use
    RPI_V2_GPIO_P1_21     =  9,  ///< Version 2, Pin P1-21, MISO when SPI0 in use
    RPI_V2_GPIO_P1_22     = 25,  ///< Version 2, Pin P1-22
    RPI_V2_GPIO_P1_23     = 11,  ///< Version 2, Pin P1-23, CLK when SPI0 in use
    RPI_V2_GPIO_P1_24     =  8,  ///< Version 2, Pin P1-24, CE0 when SPI0 in use
    RPI_V2_GPIO_P1_26     =  7,  ///< Version 2, Pin P1-26, CE1 when SPI0 in use

    // RPi Version 2, new plug P5
    RPI_V2_GPIO_P5_03     = 28,  ///< Version 2, Pin P5-03
    RPI_V2_GPIO_P5_04     = 29,  ///< Version 2, Pin P5-04
    RPI_V2_GPIO_P5_05     = 30,  ///< Version 2, Pin P5-05
    RPI_V2_GPIO_P5_06     = 31,  ///< Version 2, Pin P5-06

} RPiGPIOPin;

// Defines for SPI
// GPIO register offsets from BCM_SPI0_BASE.
// Offsets into the SPI Peripheral block in bytes per 10.5 SPI Register Map
#define BCM_SPI0_CS                      0x0000 ///< SPI Master Control and Status
#define BCM_SPI0_FIFO                    0x0004 ///< SPI Master TX and RX FIFOs
#define BCM_SPI0_CLK                     0x0008 ///< SPI Master Clock Divider
#define BCM_SPI0_DLEN                    0x000c ///< SPI Master Data Length
#define BCM_SPI0_LTOH                    0x0010 ///< SPI LOSSI mode TOH
#define BCM_SPI0_DC                      0x0014 ///< SPI DMA DREQ Controls

// Register masks for SPI0_CS
#define BCM_SPI0_CS_LEN_LONG             0x02000000 ///< Enable Long data word in Lossi mode if DMA_LEN is set
#define BCM_SPI0_CS_DMA_LEN              0x01000000 ///< Enable DMA mode in Lossi mode
#define BCM_SPI0_CS_CSPOL2               0x00800000 ///< Chip Select 2 Polarity
#define BCM_SPI0_CS_CSPOL1               0x00400000 ///< Chip Select 1 Polarity
#define BCM_SPI0_CS_CSPOL0               0x00200000 ///< Chip Select 0 Polarity
#define BCM_SPI0_CS_RXF                  0x00100000 ///< RXF - RX FIFO Full
#define BCM_SPI0_CS_RXR                  0x00080000 ///< RXR RX FIFO needs Reading ( full)
#define BCM_SPI0_CS_TXD                  0x00040000 ///< TXD TX FIFO can accept Data
#define BCM_SPI0_CS_RXD                  0x00020000 ///< RXD RX FIFO contains Data
#define BCM_SPI0_CS_DONE                 0x00010000 ///< Done transfer Done
#define BCM_SPI0_CS_TE_EN                0x00008000 ///< Unused
#define BCM_SPI0_CS_LMONO                0x00004000 ///< Unused
#define BCM_SPI0_CS_LEN                  0x00002000 ///< LEN LoSSI enable
#define BCM_SPI0_CS_REN                  0x00001000 ///< REN Read Enable
#define BCM_SPI0_CS_ADCS                 0x00000800 ///< ADCS Automatically Deassert Chip Select
#define BCM_SPI0_CS_INTR                 0x00000400 ///< INTR Interrupt on RXR
#define BCM_SPI0_CS_INTD                 0x00000200 ///< INTD Interrupt on Done
#define BCM_SPI0_CS_DMAEN                0x00000100 ///< DMAEN DMA Enable
#define BCM_SPI0_CS_TA                   0x00000080 ///< Transfer Active
#define BCM_SPI0_CS_CSPOL                0x00000040 ///< Chip Select Polarity
#define BCM_SPI0_CS_CLEAR                0x00000030 ///< Clear FIFO Clear RX and TX
#define BCM_SPI0_CS_CLEAR_RX             0x00000020 ///< Clear FIFO Clear RX
#define BCM_SPI0_CS_CLEAR_TX             0x00000010 ///< Clear FIFO Clear TX
#define BCM_SPI0_CS_CPOL                 0x00000008 ///< Clock Polarity
#define BCM_SPI0_CS_CPHA                 0x00000004 ///< Clock Phase
#define BCM_SPI0_CS_CS                   0x00000003 ///< Chip Select

/// \brief bcmSPIBitOrder SPI Bit order
/// Specifies the SPI data bit ordering for bcm_spi_setBitOrder()
typedef enum
{
    BCM_SPI_BIT_ORDER_LSBFIRST = 0,  ///< LSB First
    BCM_SPI_BIT_ORDER_MSBFIRST = 1   ///< MSB First
}bcmSPIBitOrder;

/// \brief SPI Data mode
/// Specify the SPI data mode to be passed to bcm_spi_setDataMode()
typedef enum
{
    BCM_SPI_MODE0 = 0,  ///< CPOL = 0, CPHA = 0
    BCM_SPI_MODE1 = 1,  ///< CPOL = 0, CPHA = 1
    BCM_SPI_MODE2 = 2,  ///< CPOL = 1, CPHA = 0
    BCM_SPI_MODE3 = 3,  ///< CPOL = 1, CPHA = 1
}bcmSPIMode;

/// \brief bcmSPIChipSelect
/// Specify the SPI chip select pin(s)
typedef enum
{
    BCM_SPI_CS0 = 0,     ///< Chip Select 0
    BCM_SPI_CS1 = 1,     ///< Chip Select 1
    BCM_SPI_CS2 = 2,     ///< Chip Select 2 (ie pins CS1 and CS2 are asserted)
    BCM_SPI_CS_NONE = 3, ///< No CS, control it yourself
} bcmSPIChipSelect;

/// \brief bcmSPIClockDivider
/// Specifies the divider used to generate the SPI clock from the system clock.
/// Figures below give the divider, clock period and clock frequency.
/// Clock divided is based on nominal base clock rate of 250MHz
/// It is reported that (contrary to the documentation) any even divider may used.
/// The frequencies shown for each divider have been confirmed by measurement
typedef enum
{
    BCM_SPI_CLOCK_DIVIDER_65536 = 0,       ///< 65536 = 262.144us = 3.814697260kHz
    BCM_SPI_CLOCK_DIVIDER_32768 = 32768,   ///< 32768 = 131.072us = 7.629394531kHz
    BCM_SPI_CLOCK_DIVIDER_16384 = 16384,   ///< 16384 = 65.536us = 15.25878906kHz
    BCM_SPI_CLOCK_DIVIDER_8192  = 8192,    ///< 8192 = 32.768us = 30/51757813kHz
    BCM_SPI_CLOCK_DIVIDER_4096  = 4096,    ///< 4096 = 16.384us = 61.03515625kHz
    BCM_SPI_CLOCK_DIVIDER_2048  = 2048,    ///< 2048 = 8.192us = 122.0703125kHz
    BCM_SPI_CLOCK_DIVIDER_1024  = 1024,    ///< 1024 = 4.096us = 244.140625kHz
    BCM_SPI_CLOCK_DIVIDER_512   = 512,     ///< 512 = 2.048us = 488.28125kHz
    BCM_SPI_CLOCK_DIVIDER_256   = 256,     ///< 256 = 1.024us = 976.5625MHz
    BCM_SPI_CLOCK_DIVIDER_128   = 128,     ///< 128 = 512ns = = 1.953125MHz
    BCM_SPI_CLOCK_DIVIDER_64    = 64,      ///< 64 = 256ns = 3.90625MHz
    BCM_SPI_CLOCK_DIVIDER_32    = 32,      ///< 32 = 128ns = 7.8125MHz
    BCM_SPI_CLOCK_DIVIDER_16    = 16,      ///< 16 = 64ns = 15.625MHz
    BCM_SPI_CLOCK_DIVIDER_8     = 8,       ///< 8 = 32ns = 31.25MHz
    BCM_SPI_CLOCK_DIVIDER_4     = 4,       ///< 4 = 16ns = 62.5MHz
    BCM_SPI_CLOCK_DIVIDER_2     = 2,       ///< 2 = 8ns = 125MHz, fastest you can get
    BCM_SPI_CLOCK_DIVIDER_1     = 1,       ///< 0 = 262.144us = 3.814697260kHz, same as 0/65536
} bcmSPIClockDivider;

// Defines for I2C
// GPIO register offsets from BCM_BSC*_BASE.
// Offsets into the BSC Peripheral block in bytes per 3.1 BSC Register Map
#define BCM_BSC_C 							0x0000 ///< BSC Master Control
#define BCM_BSC_S 							0x0004 ///< BSC Master Status
#define BCM_BSC_DLEN						0x0008 ///< BSC Master Data Length
#define BCM_BSC_A 							0x000c ///< BSC Master Slave Address
#define BCM_BSC_FIFO						0x0010 ///< BSC Master Data FIFO
#define BCM_BSC_DIV							0x0014 ///< BSC Master Clock Divider
#define BCM_BSC_DEL							0x0018 ///< BSC Master Data Delay
#define BCM_BSC_CLKT						0x001c ///< BSC Master Clock Stretch Timeout

// Register masks for BSC_C
#define BCM_BSC_C_I2CEN 					0x00008000 ///< I2C Enable, 0 = disabled, 1 = enabled
#define BCM_BSC_C_INTR 						0x00000400 ///< Interrupt on RX
#define BCM_BSC_C_INTT 						0x00000200 ///< Interrupt on TX
#define BCM_BSC_C_INTD 						0x00000100 ///< Interrupt on DONE
#define BCM_BSC_C_ST 						0x00000080 ///< Start transfer, 1 = Start a new transfer
#define BCM_BSC_C_CLEAR_1 					0x00000020 ///< Clear FIFO Clear
#define BCM_BSC_C_CLEAR_2 					0x00000010 ///< Clear FIFO Clear
#define BCM_BSC_C_READ 						0x00000001 ///<	Read transfer

// Register masks for BSC_S
#define BCM_BSC_S_CLKT 						0x00000200 ///< Clock stretch timeout
#define BCM_BSC_S_ERR 						0x00000100 ///< ACK error
#define BCM_BSC_S_RXF 						0x00000080 ///< RXF FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define BCM_BSC_S_TXE 						0x00000040 ///< TXE FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define BCM_BSC_S_RXD 						0x00000020 ///< RXD FIFO contains data
#define BCM_BSC_S_TXD 						0x00000010 ///< TXD FIFO can accept data
#define BCM_BSC_S_RXR 						0x00000008 ///< RXR FIFO needs reading (full)
#define BCM_BSC_S_TXW 						0x00000004 ///< TXW FIFO needs writing (full)
#define BCM_BSC_S_DONE 						0x00000002 ///< Transfer DONE
#define BCM_BSC_S_TA 						0x00000001 ///< Transfer Active

#define BCM_BSC_FIFO_SIZE   				16 ///< BSC FIFO size

/// \brief bcmI2CClockDivider
/// Specifies the divider used to generate the I2C clock from the system clock.
/// Clock divided is based on nominal base clock rate of 250MHz
typedef enum
{
    BCM_I2C_CLOCK_DIVIDER_2500   = 2500,      ///< 2500 = 10us = 100 kHz
    BCM_I2C_CLOCK_DIVIDER_626    = 626,       ///< 622 = 2.504us = 399.3610 kHz
    BCM_I2C_CLOCK_DIVIDER_150    = 150,       ///< 150 = 60ns = 1.666 MHz (default at reset)
    BCM_I2C_CLOCK_DIVIDER_148    = 148,       ///< 148 = 59ns = 1.689 MHz
} bcmI2CClockDivider;

/// \brief bcmI2CReasonCodes
/// Specifies the reason codes for the bcm_i2c_write and bcm_i2c_read functions.
typedef enum
{
    BCM_I2C_REASON_OK   		 = 0x00,      ///< Success
    BCM_I2C_REASON_ERROR_NACK    = 0x01,      ///< Received a NACK
    BCM_I2C_REASON_ERROR_CLKT    = 0x02,      ///< Received Clock Stretch Timeout
    BCM_I2C_REASON_ERROR_DATA    = 0x04,      ///< Not all data is sent / received
} bcmI2CReasonCodes;

// Defines for ST
// GPIO register offsets from BCM_ST_BASE.
// Offsets into the ST Peripheral block in bytes per 12.1 System Timer Registers
// The System Timer peripheral provides four 32-bit timer channels and a single 64-bit free running counter.
// BCM_ST_CLO is the System Timer Counter Lower bits register.
// The system timer free-running counter lower register is a read-only register that returns the current value
// of the lower 32-bits of the free running counter.
// BCM_ST_CHI is the System Timer Counter Upper bits register.
// The system timer free-running counter upper register is a read-only register that returns the current value
// of the upper 32-bits of the free running counter.
#define BCM_ST_CS 							0x0000 ///< System Timer Control/Status
#define BCM_ST_CLO 							0x0004 ///< System Timer Counter Lower 32 bits
#define BCM_ST_CHI 							0x0008 ///< System Timer Counter Upper 32 bits

/// @}


// Defines for PWM
#define BCM_PWM_CONTROL 0
#define BCM_PWM_STATUS  1
#define BCM_PWM0_RANGE  4
#define BCM_PWM0_DATA   5
#define BCM_PWM1_RANGE  8
#define BCM_PWM1_DATA   9

#define BCM_PWMCLK_CNTL     40
#define BCM_PWMCLK_DIV      41

#define BCM_PWM1_MS_MODE    0x8000  /// Run in MS mode
#define BCM_PWM1_USEFIFO    0x2000  /// Data from FIFO
#define BCM_PWM1_REVPOLAR   0x1000  /// Reverse polarity
#define BCM_PWM1_OFFSTATE   0x0800  /// Ouput Off state
#define BCM_PWM1_REPEATFF   0x0400  /// Repeat last value if FIFO empty
#define BCM_PWM1_SERIAL     0x0200  /// Run in serial mode
#define BCM_PWM1_ENABLE     0x0100  /// Channel Enable

#define BCM_PWM0_MS_MODE    0x0080  /// Run in MS mode
#define BCM_PWM0_USEFIFO    0x0020  /// Data from FIFO
#define BCM_PWM0_REVPOLAR   0x0010  /// Reverse polarity
#define BCM_PWM0_OFFSTATE   0x0008  /// Ouput Off state
#define BCM_PWM0_REPEATFF   0x0004  /// Repeat last value if FIFO empty
#define BCM_PWM0_SERIAL     0x0002  /// Run in serial mode
#define BCM_PWM0_ENABLE     0x0001  /// Channel Enable

// Historical name compatibility
#ifndef BCM_NO_DELAY_COMPATIBILITY
#define delay(x) bcm_delay(x)
#define delayMicroseconds(x) bcm_delayMicroseconds(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /// \defgroup init Library initialisation and management
    /// These functions allow you to intialise and control the bcm library
    /// @{

    /// Initialise the library by opening /dev/mem and getting pointers to the
    /// internal memory for BCM  device registers. You must call this (successfully)
    /// before calling any other
    /// functions in this library (except bcm_set_debug).
    /// If bcm_init() fails by returning 0,
    /// calling any other function may result in crashes or other failures.
    /// Prints messages to stderr in case of errors.
    /// \param[in] peri_base peripheral base address, which is processor dependent
    /// \return 1 if successful else 0
    extern int bcm_init(uint32_t peri_base);

    /// Close the library, deallocating any allocated memory and closing /dev/mem
    /// \return 1 if successful else 0
    extern int bcm_close(void);

    /// Sets the debug level of the library.
    /// A value of 1 prevents mapping to /dev/mem, and makes the library print out
    /// what it would do, rather than accessing the GPIO registers.
    /// A value of 0, the default, causes normal operation.
    /// Call this before calling bcm_init();
    /// \param[in] debug The new debug level. 1 means debug
    extern void  bcm_set_debug(uint8_t debug);

    /// @} // end of init

    /// \defgroup lowlevel Low level register access
    /// These functions provide low level register access, and should not generally
    /// need to be used
    ///
    /// @{

    /// Reads 32 bit value from a peripheral address
    /// The read is done twice, and is therefore always safe in terms of
    /// manual section 1.3 Peripheral access precautions for correct memory ordering
    /// \param[in] paddr Physical address to read from. See BCM_GPIO_BASE etc.
    /// \return the value read from the 32 bit register
    /// \sa Physical Addresses
    extern uint32_t bcm_peri_read(volatile uint32_t* paddr);


    /// Reads 32 bit value from a peripheral address without the read barrier
    /// You should only use this when your code has previously called bcm_peri_read()
    /// within the same peripheral, and no other peripheral access has occurred since.
    /// \param[in] paddr Physical address to read from. See BCM_GPIO_BASE etc.
    /// \return the value read from the 32 bit register
    /// \sa Physical Addresses
    extern uint32_t bcm_peri_read_nb(volatile uint32_t* paddr);


    /// Writes 32 bit value from a peripheral address
    /// The write is done twice, and is therefore always safe in terms of
    /// manual section 1.3 Peripheral access precautions for correct memory ordering
    /// \param[in] paddr Physical address to read from. See BCM_GPIO_BASE etc.
    /// \param[in] value The 32 bit value to write
    /// \sa Physical Addresses
    extern void bcm_peri_write(volatile uint32_t* paddr, uint32_t value);

    /// Writes 32 bit value from a peripheral address without the write barrier
    /// You should only use this when your code has previously called bcm_peri_write()
    /// within the same peripheral, and no other peripheral access has occurred since.
    /// \param[in] paddr Physical address to read from. See BCM_GPIO_BASE etc.
    /// \param[in] value The 32 bit value to write
    /// \sa Physical Addresses
    extern void bcm_peri_write_nb(volatile uint32_t* paddr, uint32_t value);

    /// Alters a number of bits in a 32 peripheral regsiter.
    /// It reads the current valu and then alters the bits deines as 1 in mask,
    /// according to the bit value in value.
    /// All other bits that are 0 in the mask are unaffected.
    /// Use this to alter a subset of the bits in a register.
    /// The write is done twice, and is therefore always safe in terms of
    /// manual section 1.3 Peripheral access precautions for correct memory ordering
    /// \param[in] paddr Physical address to read from. See BCM_GPIO_BASE etc.
    /// \param[in] value The 32 bit value to write, masked in by mask.
    /// \param[in] mask Bitmask that defines the bits that will be altered in the register.
    /// \sa Physical Addresses
    extern void bcm_peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask);
    /// @} // end of lowlevel

    /// \defgroup gpio GPIO register access
    /// These functions allow you to control the GPIO interface. You can set the
    /// function of each GPIO pin, read the input state and set the output state.
    /// @{

    /// Sets the Function Select register for the given pin, which configures
    /// the pin as Input, Output or one of the 6 alternate functions.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from RPiGPIOPin.
    /// \param[in] mode Mode to set the pin to, one of BCM_GPIO_FSEL_* from \ref bcmFunctionSelect
    extern void bcm_gpio_fsel(uint8_t pin, uint8_t mode);

    /// Sets the specified pin output to
    /// HIGH.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \sa bcm_gpio_write()
    extern void bcm_gpio_set(uint8_t pin);

    /// Sets the specified pin output to
    /// LOW.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \sa bcm_gpio_write()
    extern void bcm_gpio_clr(uint8_t pin);

    /// Sets any of the first 32 GPIO output pins specified in the mask to
    /// HIGH.
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \sa bcm_gpio_write_multi()
    extern void bcm_gpio_set_multi(uint32_t mask);

    /// Sets any of the first 32 GPIO output pins specified in the mask to
    /// LOW.
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \sa bcm_gpio_write_multi()
    extern void bcm_gpio_clr_multi(uint32_t mask);

    /// Reads the current level on the specified
    /// pin and returns either HIGH or LOW. Works whether or not the pin
    /// is an input or an output.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \return the current level  either HIGH or LOW
    extern uint8_t bcm_gpio_lev(uint8_t pin);

    /// Event Detect Status.
    /// Tests whether the specified pin has detected a level or edge
    /// as requested by bcm_gpio_ren(), bcm_gpio_fen(), bcm_gpio_hen(),
    /// bcm_gpio_len(), bcm_gpio_aren(), bcm_gpio_afen().
    /// Clear the flag for a given pin by calling bcm_gpio_set_eds(pin);
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \return HIGH if the event detect status for th given pin is true.
    extern uint8_t bcm_gpio_eds(uint8_t pin);

    /// Sets the Event Detect Status register for a given pin to 1,
    /// which has the effect of clearing the flag. Use this afer seeing
    /// an Event Detect Status on the pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_set_eds(uint8_t pin);

    /// Enable Rising Edge Detect Enable for the specified pin.
    /// When a rising edge is detected, sets the appropriate pin in Event Detect Status.
    /// The GPRENn registers use
    /// synchronous edge detection. This means the input signal is sampled using the
    /// system clock and then it is looking for a ?011? pattern on the sampled signal. This
    /// has the effect of suppressing glitches.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_ren(uint8_t pin);

    /// Disable Rising Edge Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_clr_ren(uint8_t pin);

    /// Enable Falling Edge Detect Enable for the specified pin.
    /// When a falling edge is detected, sets the appropriate pin in Event Detect Status.
    /// The GPRENn registers use
    /// synchronous edge detection. This means the input signal is sampled using the
    /// system clock and then it is looking for a ?100? pattern on the sampled signal. This
    /// has the effect of suppressing glitches.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_fen(uint8_t pin);

    /// Disable Falling Edge Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_clr_fen(uint8_t pin);

    /// Enable High Detect Enable for the specified pin.
    /// When a HIGH level is detected on the pin, sets the appropriate pin in Event Detect Status.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_hen(uint8_t pin);

    /// Disable High Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_clr_hen(uint8_t pin);

    /// Enable Low Detect Enable for the specified pin.
    /// When a LOW level is detected on the pin, sets the appropriate pin in Event Detect Status.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_len(uint8_t pin);

    /// Disable Low Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_clr_len(uint8_t pin);

    /// Enable Asynchronous Rising Edge Detect Enable for the specified pin.
    /// When a rising edge is detected, sets the appropriate pin in Event Detect Status.
    /// Asynchronous means the incoming signal is not sampled by the system clock. As such
    /// rising edges of very short duration can be detected.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_aren(uint8_t pin);

    /// Disable Asynchronous Rising Edge Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_clr_aren(uint8_t pin);

    /// Enable Asynchronous Falling Edge Detect Enable for the specified pin.
    /// When a falling edge is detected, sets the appropriate pin in Event Detect Status.
    /// Asynchronous means the incoming signal is not sampled by the system clock. As such
    /// falling edges of very short duration can be detected.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_afen(uint8_t pin);

    /// Disable Asynchronous Falling Edge Detect Enable for the specified pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    extern void bcm_gpio_clr_afen(uint8_t pin);

    /// Sets the Pull-up/down register for the given pin. This is
    /// used with bcm_gpio_pudclk() to set the  Pull-up/down resistor for the given pin.
    /// However, it is usually more convenient to use bcm_gpio_set_pud().
    /// \param[in] pud The desired Pull-up/down mode. One of BCM_GPIO_PUD_* from bcmPUDControl
    /// \sa bcm_gpio_set_pud()
    extern void bcm_gpio_pud(uint8_t pud);

    /// Clocks the Pull-up/down value set earlier by bcm_gpio_pud() into the pin.
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \param[in] on HIGH to clock the value from bcm_gpio_pud() into the pin.
    /// LOW to remove the clock.
    /// \sa bcm_gpio_set_pud()
    extern void bcm_gpio_pudclk(uint8_t pin, uint8_t on);

    /// Reads and returns the Pad Control for the given GPIO group.
    /// \param[in] group The GPIO pad group number, one of BCM_PAD_GROUP_GPIO_*
    /// \return Mask of bits from BCM_PAD_* from \ref bcmPadGroup
    extern uint32_t bcm_gpio_pad(uint8_t group);

    /// Sets the Pad Control for the given GPIO group.
    /// \param[in] group The GPIO pad group number, one of BCM_PAD_GROUP_GPIO_*
    /// \param[in] control Mask of bits from BCM_PAD_* from \ref bcmPadGroup
    extern void bcm_gpio_set_pad(uint8_t group, uint32_t control);

    /// Delays for the specified number of milliseconds.
    /// Uses nanosleep(), and therefore does not use CPU until the time is up.
    /// However, you are at the mercy of nanosleep(). From the manual for nanosleep():
    /// If the interval specified in req is not an exact multiple of the granularity
    /// underlying  clock  (see  time(7)),  then the interval will be
    /// rounded up to the next multiple. Furthermore, after the sleep completes,
    /// there may still be a delay before the CPU becomes free to once
    /// again execute the calling thread.
    /// \param[in] millis Delay in milliseconds
    extern void bcm_delay (unsigned int millis);

    /// Delays for the specified number of microseconds.
    /// Uses a combination of nanosleep() and a busy wait loop on the BCM system timers,
    /// However, you are at the mercy of nanosleep(). From the manual for nanosleep():
    /// If the interval specified in req is not an exact multiple of the granularity
    /// underlying  clock  (see  time(7)),  then the interval will be
    /// rounded up to the next multiple. Furthermore, after the sleep completes,
    /// there may still be a delay before the CPU becomes free to once
    /// again execute the calling thread.
    /// For times less than about 450 microseconds, uses a busy wait on the System Timer.
    /// It is reported that a delay of 0 microseconds on RaspberryPi will in fact
    /// result in a delay of about 80 microseconds. Your mileage may vary.
    /// \param[in] micros Delay in microseconds
    extern void bcm_delayMicroseconds (uint64_t micros);

    /// Sets the output state of the specified pin
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \param[in] on HIGH sets the output to HIGH and LOW to LOW.
    extern void bcm_gpio_write(uint8_t pin, uint8_t on);

    /// Sets any of the first 32 GPIO output pins specified in the mask to the state given by on
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \param[in] on HIGH sets the output to HIGH and LOW to LOW.
    extern void bcm_gpio_write_multi(uint32_t mask, uint8_t on);

    /// Sets the first 32 GPIO output pins specified in the mask to the value given by value
    /// \param[in] value values required for each bit masked in by mask, eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    /// \param[in] mask Mask of pins to affect. Use eg: (1 << RPI_GPIO_P1_03) | (1 << RPI_GPIO_P1_05)
    extern void bcm_gpio_write_mask(uint32_t value, uint32_t mask);

    /// Sets the Pull-up/down mode for the specified pin. This is more convenient than
    /// clocking the mode in with bcm_gpio_pud() and bcm_gpio_pudclk().
    /// \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    /// \param[in] pud The desired Pull-up/down mode. One of BCM_GPIO_PUD_* from bcmPUDControl
    extern void bcm_gpio_set_pud(uint8_t pin, uint8_t pud);

    /// @}

    /// \defgroup spi SPI access
    /// These functions let you use SPI0 (Serial Peripheral Interface) to
    /// interface with an external SPI device.
    /// @{

    /// Start SPI operations.
    /// Forces RPi SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK), P1-24 (CE0) and P1-26 (CE1)
    /// to alternate function ALT0, which enables those pins for SPI interface.
    /// You should call bcm_spi_end() when all SPI funcitons are complete to return the pins to
    /// their default functions
    /// \sa  bcm_spi_end()
    extern void bcm_spi_begin(void);

    /// End SPI operations.
    /// SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK), P1-24 (CE0) and P1-26 (CE1)
    /// are returned to their default INPUT behaviour.
    extern void bcm_spi_end(void);

    /// Sets the SPI bit order
    /// NOTE: has no effect. Not supported by SPI0.
    /// Defaults to
    /// \param[in] order The desired bit order, one of BCM_SPI_BIT_ORDER_*,
    /// see \ref bcmSPIBitOrder
    extern void bcm_spi_setBitOrder(uint8_t order);

    /// Sets the SPI clock divider and therefore the
    /// SPI clock speed.
    /// \param[in] divider The desired SPI clock divider, one of BCM_SPI_CLOCK_DIVIDER_*,
    /// see \ref bcmSPIClockDivider
    extern void bcm_spi_setClockDivider(uint16_t divider);

    /// Sets the SPI data mode
    /// Sets the clock polariy and phase
    /// \param[in] mode The desired data mode, one of BCM_SPI_MODE*,
    /// see \ref bcmSPIMode
    extern void bcm_spi_setDataMode(uint8_t mode);

    /// Sets the chip select pin(s)
    /// When an bcm_spi_transfer() is made, the selected pin(s) will be asserted during the
    /// transfer.
    /// \param[in] cs Specifies the CS pins(s) that are used to activate the desired slave.
    ///   One of BCM_SPI_CS*, see \ref bcmSPIChipSelect
    extern void bcm_spi_chipSelect(uint8_t cs);

    /// Sets the chip select pin polarity for a given pin
    /// When an bcm_spi_transfer() occurs, the currently selected chip select pin(s)
    /// will be asserted to the
    /// value given by active. When transfers are not happening, the chip select pin(s)
    /// return to the complement (inactive) value.
    /// \param[in] cs The chip select pin to affect
    /// \param[in] active Whether the chip select pin is to be active HIGH
    extern void bcm_spi_setChipSelectPolarity(uint8_t cs, uint8_t active);

    /// Transfers one byte to and from the currently selected SPI slave.
    /// Asserts the currently selected CS pins (as previously set by bcm_spi_chipSelect)
    /// during the transfer.
    /// Clocks the 8 bit value out on MOSI, and simultaneously clocks in data from MISO.
    /// Returns the read data byte from the slave.
    /// Uses polled transfer as per section 10.6.1 of the BCM  ARM Peripherls manual
    /// \param[in] value The 8 bit data byte to write to MOSI
    /// \return The 8 bit byte simultaneously read from  MISO
    /// \sa bcm_spi_transfern()
    extern uint8_t bcm_spi_transfer(uint8_t value);

    /// Transfers any number of bytes to and from the currently selected SPI slave.
    /// Asserts the currently selected CS pins (as previously set by bcm_spi_chipSelect)
    /// during the transfer.
    /// Clocks the len 8 bit bytes out on MOSI, and simultaneously clocks in data from MISO.
    /// The data read read from the slave is placed into rbuf. rbuf must be at least len bytes long
    /// Uses polled transfer as per section 10.6.1 of the BCM  ARM Peripherls manual
    /// \param[in] tbuf Buffer of bytes to send.
    /// \param[out] rbuf Received bytes will by put in this buffer
    /// \param[in] len Number of bytes in the tbuf buffer, and the number of bytes to send/received
    /// \sa bcm_spi_transfer()
    extern void bcm_spi_transfernb(char* tbuf, char* rbuf, uint32_t len);

    /// Transfers any number of bytes to and from the currently selected SPI slave
    /// using bcm_spi_transfernb.
    /// The returned data from the slave replaces the transmitted data in the buffer.
    /// \param[in,out] buf Buffer of bytes to send. Received bytes will replace the contents
    /// \param[in] len Number of bytes int eh buffer, and the number of bytes to send/received
    /// \sa bcm_spi_transfer()
    extern void bcm_spi_transfern(char* buf, uint32_t len);

    /// Transfers any number of bytes to the currently selected SPI slave.
    /// Asserts the currently selected CS pins (as previously set by bcm_spi_chipSelect)
    /// during the transfer.
    /// \param[in] buf Buffer of bytes to send.
    /// \param[in] len Number of bytes in the tbuf buffer, and the number of bytes to send
    extern void bcm_spi_writenb(char* buf, uint32_t len);

    /// @}

    /// \defgroup i2c I2C access
    /// These functions let you use I2C (The Broadcom Serial Control bus with the Philips
    /// I2C bus/interface version 2.1 January 2000.) to interface with an external I2C device.
    /// @{

    /// Start I2C operations.
    /// Forces RPi I2C pins P1-03 (SDA) and P1-05 (SCL)
    /// to alternate function ALT0, which enables those pins for I2C interface.
    /// You should call bcm_i2c_end() when all I2C functions are complete to return the pins to
    /// their default functions
    /// \sa  bcm_i2c_end()
    extern void bcm_i2c_begin(void);

    /// End I2C operations.
    /// I2C pins P1-03 (SDA) and P1-05 (SCL)
    /// are returned to their default INPUT behaviour.
    extern void bcm_i2c_end(void);

    /// Sets the I2C slave address.
    /// \param[in] addr The I2C slave address.
    extern void bcm_i2c_setSlaveAddress(uint8_t addr);

    /// Sets the I2C clock divider and therefore the I2C clock speed.
    /// \param[in] divider The desired I2C clock divider, one of BCM_I2C_CLOCK_DIVIDER_*,
    /// see \ref bcmI2CClockDivider
    extern void bcm_i2c_setClockDivider(uint16_t divider);

    /// Sets the I2C clock divider by converting the baudrate parameter to
    /// the equivalent I2C clock divider. ( see \sa bcm_i2c_setClockDivider)
    /// For the I2C standard 100khz you would set baudrate to 100000
    /// The use of baudrate corresponds to its use in the I2C kernel device
    /// driver. (Of course, bcm has nothing to do with the kernel driver)
    extern void bcm_i2c_set_baudrate(uint32_t baudrate);

    /// Transfers any number of bytes to the currently selected I2C slave.
    /// (as previously set by \sa bcm_i2c_setSlaveAddress)
    /// \param[in] buf Buffer of bytes to send.
    /// \param[in] len Number of bytes in the buf buffer, and the number of bytes to send.
	/// \return reason see \ref bcmI2CReasonCodes
    extern uint8_t bcm_i2c_write(const char * buf, uint32_t len);

    /// Transfers any number of bytes from the currently selected I2C slave.
    /// (as previously set by \sa bcm_i2c_setSlaveAddress)
    /// \param[in] buf Buffer of bytes to receive.
    /// \param[in] len Number of bytes in the buf buffer, and the number of bytes to received.
	/// \return reason see \ref bcmI2CReasonCodes
    extern uint8_t bcm_i2c_read(char* buf, uint32_t len);

    /// Allows reading from I2C slaves that require a repeated start (without any prior stop)
    /// to read after the required slave register has been set. For example, the popular
    /// MPL3115A2 pressure and temperature sensor. Note that your device must support or
    /// require this mode. If your device does not require this mode then the standard
    /// combined:
    ///   \sa bcm_i2c_write
    ///   \sa bcm_i2c_read
    /// are a better choice.
    /// Will read from the slave previously set by \sa bcm_i2c_setSlaveAddress
    /// \param[in] regaddr Buffer containing the slave register you wish to read from.
    /// \param[in] buf Buffer of bytes to receive.
    /// \param[in] len Number of bytes in the buf buffer, and the number of bytes to received.
	/// \return reason see \ref bcmI2CReasonCodes
    extern uint8_t bcm_i2c_read_register_rs(char* regaddr, char* buf, uint32_t len);

    /// @}

    /// \defgroup st System Timer access
    /// Allows access to and delays using the System Timer Counter.
    /// @{

    /// Read the System Timer Counter register.
    /// \return the value read from the System Timer Counter Lower 32 bits register
    uint64_t bcm_st_read(void);

    /// Delays for the specified number of microseconds with offset.
    /// \param[in] offset_micros Offset in microseconds
    /// \param[in] micros Delay in microseconds
    extern void bcm_st_delay(uint64_t offset_micros, uint64_t micros);

    /// @}

#ifdef __cplusplus
}
#endif

#endif // BCM_H
