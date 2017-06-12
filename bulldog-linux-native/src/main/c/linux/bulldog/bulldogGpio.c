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
    bcm2835_gpio_fsel(pinAddress, BCM2835_GPIO_FSEL_INPT);
    return bcm2835_gpio_lev(pinAddress);
}
extern void writeSignal(int value, int pinAddress) {
    bcm2835_gpio_fsel(pinAddress, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pinAddress, value ? HIGH : LOW);
}
