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
#include "bcm.h"

void spiOpen(int mode, int speed, int bitsPerWord, int lsbFirst) {
  spiConfig(mode, speed, bitsPerWord, lsbFirst);
}

//bitPerWord - TODO
void spiConfig(int mode, int speed, int bitsPerWord, int lsbFirst) {
  bcm_spi_setBitOrder((unsigned char)lsbFirst);
  bcm_spi_setClockDivider(speed);
  bcm_spi_setDataMode((unsigned char)mode);
}

//bitsPerWord - TODO
void spiTransfer(unsigned char* txBuffer, unsigned char* rxBuffer, int transferLength, int delay, int speed, int bitsPerWord) {
  if(delay > 0) {
      bcm_delayMicroseconds (delay);
  }
  bcm_spi_setClockDivider(BCM_SPI_CLOCK_DIVIDER_65536);
  bcm_spi_begin();
  if((bitsPerWord % 8) != 0){
    return;
  }
  bcm_spi_transfernb(txBuffer, rxBuffer, transferLength);
}

void spiClose() {
	bcm_spi_end();
}

//Only values : 0,1,2 are permitted (as defined in bcm.h bcmSPIChipSelect enum)
void spiSelectSlave(int slaveIndex) {
    bcm_spi_chipSelect(slaveIndex);
    bcm_spi_setChipSelectPolarity(slaveIndex, LOW);
}
