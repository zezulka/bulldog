#ifndef BULLDOG_I2C_H
#define BULLDOG_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char i2cRead(char* buf, int len);
extern unsigned char i2cWrite(char * buf, int len);
extern void i2cSelectSlave(int addr);
extern void i2cOpen(void);
extern void i2cClose(void);

#ifdef __cplusplus
}
#endif

#endif
