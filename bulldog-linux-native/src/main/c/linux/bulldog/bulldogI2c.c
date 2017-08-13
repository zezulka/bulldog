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
#include "bcm.h"

unsigned char i2cRead(char* buf, int len) {
   bcm_i2c_begin();
   unsigned char retVal = bcm_i2c_read(buf, len);
   return retVal;
}

unsigned char i2cWrite(char * buf, int len) {
    bcm_i2c_begin();
    unsigned char retVal =  bcm_i2c_write(buf, len);
    return retVal;
}

void i2cSelectSlave(int addr) {
    bcm_i2c_begin();
    bcm_i2c_setSlaveAddress(addr);
}

void i2cOpen(void) {
}

void i2cClose(void) {
    bcm_i2c_end();
}
