#include <jni.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "io_silverspoon_bulldog_linux_jni_NativeSpi.h"
#include "../bulldog/bulldogSpi.h"

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeSpi
 * Method:    spiOpen
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiOpen
  (JNIEnv * env, jclass clazz, jint mode, jint speed, jint bitsPerWord, jboolean lsbFirst) {
	spiOpen(mode, speed, bitsPerWord, lsbFirst == JNI_TRUE ? 1 : 0);
}

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeSpi
 * Method:    spiOpen
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiConfig
  (JNIEnv * env, jclass clazz, jint mode, jint speed, jint bitsPerWord, jboolean lsbFirst) {
	spiConfig(mode, speed, bitsPerWord, lsbFirst == JNI_TRUE ? 1 : 0);
}

JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiSelectSlave
  (JNIEnv *env, jclass clazz, jint index) {
    spiSelectSlave(index);
  }

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeSpi
 * Method:    spiClose
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiClose
  (JNIEnv * env, jclass clazz) {
	spiClose();
}

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeSpi
 * Method:    spiTransfer
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiTransfer
  (JNIEnv * env , jclass clazz, jobject txBuffer, jobject rxBuffer, jint transferLength, jint delay, jint speed, jint bitsPerWord) {
	unsigned char *pTx = NULL;
	unsigned char *pRx = NULL;

	if(txBuffer != NULL) {
		pTx = (unsigned char *) (*env)->GetDirectBufferAddress(env, txBuffer);
	}

	if(rxBuffer != NULL) {
		pRx = (unsigned char *) (*env)->GetDirectBufferAddress(env, rxBuffer);
	}

	spiTransfer((unsigned char *) pTx, (unsigned char *) pTx, (int) transferLength,
	    (int) delay, (int) speed, (int) bitsPerWord);
}
