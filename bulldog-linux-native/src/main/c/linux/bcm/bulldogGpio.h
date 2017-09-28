#ifndef BULLDOG_I2C_H
#define BULLDOG_I2C_H


#ifdef __cplusplus
extern "C" {
#endif

extern char readSignal(int pinAddress);
extern void writeSignal(int value, int pinAddress);

#ifdef __cplusplus
}
#endif

#endif
