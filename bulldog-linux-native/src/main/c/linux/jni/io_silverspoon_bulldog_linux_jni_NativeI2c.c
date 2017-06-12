#include <jni.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "../bulldog/bulldogI2c.h"
#include "io_silverspoon_bulldog_linux_jni_NativeI2c.h"

/*
 * Class:     io_silverspoon_bulldog_linux_jni_I2CNative
 * Method:    i2cRead
 * Signature: ([BII)B
 */
JNIEXPORT jbyte JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeI2c_i2cReadBuffer
  (JNIEnv * env, jclass clazz, jbyteArray array, jint len, jint pos) {
    char* buf = (char *)(*env)->GetByteArrayElements(env, array, 0);
    int retVal = i2cReadBuf(buf, (int)len, pos);
    (*env)->ReleaseByteArrayElements(env, array, buf, 0);
    return retVal;
}

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeI2c
 * Method:    i2cReadByte
 * Signature: (I)B
 */
JNIEXPORT jbyte JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeI2c_i2cRead
  (JNIEnv * env , jclass clazz, jint pos) {
    return i2cRead(pos);
}

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeI2c
 * Method:    i2cWrite
 * Signature: (IB)B
 */
JNIEXPORT jbyte JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeI2c_i2cWrite
  (JNIEnv * env, jclass clazz, jint pos, jbyte data) {
	  return i2cWrite(pos, data);
}

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeI2c
 * Method:    i2cWriteBuffer
 * Signature: ([BII)B
 */
JNIEXPORT jbyte JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeI2c_i2cWriteBuffer
  (JNIEnv * env, jclass clazz, jbyteArray array, jint len, jint pos) {
    char* buf = (char *)(*env)->GetByteArrayElements(env, array, 0);
    int retVal = i2cWriteBuf(buf, (int)len, pos);
    (*env)->ReleaseByteArrayElements(env, array, buf, 0);
    return retVal;
}

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeI2c
 * Method:    i2cSelectSlave
 * Signature: (I)I
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeI2c_i2cSelectSlave
  (JNIEnv * env, jclass clazz, jint addr) {
	   i2cSelectSlave(addr);
}

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeI2c
 * Method:    i2cOpen
 * Signature: (I)I
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeI2c_i2cOpen
  (JNIEnv * env, jclass clazz) {
 	   i2cOpen();
}


/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeI2c
 * Method:    i2cClose
 * Signature: (I)I
 */
JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeI2c_i2cClose
  (JNIEnv * env, jclass clazz) {
	   i2cClose();
}
