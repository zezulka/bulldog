#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <errno.h>

#include "bulldogSpi.h"
#include "bulldog.h"
#include "bcm2835.h"

void spiOpen(int mode, int speed, int bitsPerWord, int lsbFirst) {
  spiConfig(mode, speed, bitsPerWord, lsbFirst);
}

//bitPerWord - TODO
void spiConfig(int mode, int speed, int bitsPerWord, int lsbFirst) {
  bcm2835_spi_setBitOrder((unsigned char)lsbFirst);
  bcm2835_spi_setClockDivider(speed);
  bcm2835_spi_setDataMode((unsigned char)mode);
}

//bitsPerWord - TODO
void spiTransfer(unsigned char* txBuffer, unsigned char* rxBuffer, int transferLength, int delay, int speed, int bitsPerWord) {
  if(delay > 0) {
      bcm2835_delayMicroseconds (delay);
  }
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
  bcm2835_spi_begin();
  if((bitsPerWord % 8) != 0){
    return;
  }
  bcm2835_spi_transfernb(txBuffer, rxBuffer, transferLength);
}

void spiClose() {
	bcm2835_spi_end();
}

//Only values : 0,1,2 are permitted (as defined in bcm2835.h bcm2835SPIChipSelect enum)
void spiSelectSlave(int slaveIndex) {
    bcm2835_spi_chipSelect(slaveIndex);
    bcm2835_spi_setChipSelectPolarity(slaveIndex, LOW);
}
