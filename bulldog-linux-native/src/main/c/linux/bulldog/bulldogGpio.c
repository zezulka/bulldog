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
#include "bcm.h"

extern char readSignal(int pinAddress) {
    printf("Read signal called, pin address %d.\n", pinAddress);
    bcm_gpio_fsel(pinAddress, BCM_GPIO_FSEL_INPT);
    bcm_gpio_set_pud(pinAddress, BCM_GPIO_PUD_UP);
    bcm_delay(150);
    return bcm_gpio_lev(pinAddress);
}
extern void writeSignal(int value, int pinAddress) {
    printf("Write signal called.\n");
    bcm_gpio_fsel(pinAddress, BCM_GPIO_FSEL_INPT);
    bcm_gpio_fsel(pinAddress, BCM_GPIO_FSEL_OUTP);
    bcm_delay(150);
    bcm_gpio_write(pinAddress, value);
}
