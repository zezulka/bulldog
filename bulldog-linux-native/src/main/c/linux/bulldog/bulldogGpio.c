#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#include "bulldogGpio.h"
#include "bulldog.h"
#include "bcm2835.h"

extern char readSignal(int pinAddress) {
    printf("Read signal called, pin address %d.\n", pinAddress);
    bcm2835_gpio_fsel(pinAddress, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(pinAddress, BCM2835_GPIO_PUD_UP);
    bcm2835_delay(150);
    return bcm2835_gpio_lev(pinAddress);
}
extern void writeSignal(int value, int pinAddress) {
    printf("Write signal called.\n");
    bcm2835_gpio_fsel(pinAddress, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(pinAddress, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_delay(150);
    bcm2835_gpio_write(pinAddress, value);
}
