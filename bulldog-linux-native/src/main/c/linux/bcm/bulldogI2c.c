#include <stdio.h>

#include "bulldogI2c.h"
#include "bcm2835.h"

unsigned char i2cRead(char* buf, int len) {
  bcm2835_i2c_begin();
  unsigned char retVal = bcm2835_i2c_read(buf, len);
  return retVal;
}

unsigned char i2cWrite(char * buf, int len) {
  bcm2835_i2c_begin();
  unsigned char retVal = bcm2835_i2c_write(buf, len);
  return retVal;
}

void i2cSelectSlave(int addr) {
  bcm2835_i2c_begin();
  bcm2835_i2c_setSlaveAddress(addr);
}

void i2cOpen(void) {
  bcm2835_i2c_begin();
}

void i2cClose(void) {
  bcm2835_i2c_end();
}
