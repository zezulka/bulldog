#ifndef BULLDOG_I2C_H
#define BULLDOG_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char i2cReadBufNoRange(char* buf, int len);
extern unsigned char i2cReadBuf(char* buf, int len, int pos);
extern unsigned char i2cRead(int pos);
extern unsigned char i2cWrite(int pos, unsigned char data);
extern unsigned char i2cWriteBuf(char * buf, int len, int pos);
extern unsigned char i2cWriteBufNoRange(char * buf, int len);
extern void i2cSelectSlave(int addr);
extern void i2cOpen(void);
extern void i2cClose(void);

#ifdef __cplusplus
}
#endif

#endif
