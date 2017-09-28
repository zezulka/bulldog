#include <stdio.h>

#include "bulldogGpio.h"
#include "bcm2835.h"

extern char readSignal(int pinAddress) {
  bcm2835_gpio_fsel(pinAddress, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_set_pud(pinAddress, BCM2835_GPIO_PUD_UP);
  return bcm2835_gpio_lev(pinAddress);
}
extern void writeSignal(int value, int pinAddress) {
  bcm2835_gpio_fsel(pinAddress, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(pinAddress, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_write(pinAddress, value);
}
