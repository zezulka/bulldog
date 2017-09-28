#include <stdio.h>
#include "bulldogSpi.h"
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
  bcm2835_spi_begin();
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
  if((bitsPerWord % 8) != 0){
    return;
  }
  bcm2835_spi_transfernb(txBuffer, rxBuffer, transferLength);
  bcm2835_spi_end();
}

void spiClose() {
  bcm2835_spi_end();
}

void spiSelectSlave(int slaveIndex) {
  bcm2835_spi_chipSelect(slaveIndex);
  bcm2835_spi_setChipSelectPolarity(slaveIndex, LOW);
}
