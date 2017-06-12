#ifndef BULLDOG_SPI_H
#define BULLDOG_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

extern void spiOpen(int mode, int speed, int bitsPerWord, int lsbFirst);
extern void spiTransfer(unsigned char* txBuffer, unsigned char* rxBuffer, int transferLength, int delay, int speed, int bitsPerWord);
extern void spiConfig(int mode, int speed, int bitsPerWord, int lsbFirst);
extern void spiClose();
extern void spiSelectSlave(int index);

#ifdef __cplusplus
}
#endif

#endif
