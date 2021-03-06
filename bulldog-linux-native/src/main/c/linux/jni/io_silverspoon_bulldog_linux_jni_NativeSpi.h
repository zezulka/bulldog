/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class io_silverspoon_bulldog_linux_jni_NativeSpi */

#ifndef _Included_io_silverspoon_bulldog_linux_jni_NativeSpi
#define _Included_io_silverspoon_bulldog_linux_jni_NativeSpi
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeSpi
 * Method:    spiOpen
 * Signature: (IIIZ)V
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiOpen
  (JNIEnv *, jclass, jint, jint, jint, jboolean);

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeSpi
 * Method:    spiConfig
 * Signature: (IIIZ)V
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiConfig
  (JNIEnv *, jclass, jint, jint, jint, jboolean);

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeSpi
 * Method:    spiClose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiClose
  (JNIEnv *, jclass);

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeSpi
 * Method:    spiSelectSlave
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiSelectSlave
  (JNIEnv *, jclass, jint);

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeSpi
 * Method:    spiTransfer
 * Signature: (Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;IIII)V
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeSpi_spiTransfer
  (JNIEnv *, jclass, jobject, jobject, jint, jint, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
