#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#include "bulldogI2c.h"
#include "bulldog.h"
#include "bcm2835.h"

//Not recommended to use - i2c request should provide start address
//of the read operation, such step is ommited!
unsigned char i2cReadBufNoRange(char* buf, int len) {
   bcm2835_i2c_begin();
   unsigned char retVal = bcm2835_i2c_read(buf, len);
   bcm2835_i2c_end();
   return retVal;
}

unsigned char i2cReadBuf(char* buf, int len, int pos) {
  bcm2835_i2c_begin();
  unsigned char internalRegister = (unsigned char) pos;
  bcm2835_i2c_write(&internalRegister, 1);
  unsigned char retVal =  bcm2835_i2c_read(buf, len);
  bcm2835_i2c_end();
  return retVal;
}

unsigned char i2cRead(int pos) {
   bcm2835_i2c_begin();
   unsigned char result = 0x00;
   unsigned char internalRegister = (unsigned char) pos;
   bcm2835_i2c_write(&internalRegister, 1);
   bcm2835_i2c_read(&result, 1);
   bcm2835_i2c_end();
   return result;
}

//Writes one byte to the specified position.
unsigned char i2cWrite(int pos, unsigned char data) {
    bcm2835_i2c_begin();
    int len = 2;
    char buf[len];
    *(buf + 1) = data;
    *(buf) = (unsigned char)pos;
    unsigned char retVal = bcm2835_i2c_write(buf, 2);
    bcm2835_i2c_end();
    return retVal;
}

//Not recommended to use - i2c request should provide start address
//of the write operation, such step may even lead to data corruption in the
//i2c device (first address is ALWAYS considered to be the start address),
//use carefully
unsigned char i2cWriteBufNoRange(char * buf, int len) {
    bcm2835_i2c_begin();
    unsigned char retVal =  bcm2835_i2c_write(buf, len);
    bcm2835_i2c_end();
    return retVal;
}

unsigned char i2cWriteBuf(char * buf, int len, int pos) {
    bcm2835_i2c_begin();
    int newLen = strlen(buf) + 1;
    char newBuf[newLen];
    *(newBuf + newLen - 1) = '\0';
    char* ptr = newBuf + 1;
    strcpy(ptr, buf);
    *(newBuf) = (unsigned char)pos;
    unsigned char retVal =  bcm2835_i2c_write(newBuf, len+1);
    bcm2835_i2c_end();
    return retVal;
}

void i2cSelectSlave(int addr) {
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(addr);
}

void i2cOpen(void) {
}

void i2cClose(void) {
}
