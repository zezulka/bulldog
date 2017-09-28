#include <jni.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "../bcm/bulldogI2c.h"
#include "io_silverspoon_bulldog_linux_jni_NativeI2c.h"

/*
 * Class:     io_silverspoon_bulldog_linux_jni_I2CNative
 * Method:    i2cRead
 * Signature: ([BII)B
 */
JNIEXPORT jbyte JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeI2c_i2cRead
  (JNIEnv * env, jclass clazz, jbyteArray array, jint len) {
    char* buf = (char *)(*env)->GetByteArrayElements(env, array, 0);
    int retVal = i2cRead(buf, (int)len);
    (*env)->ReleaseByteArrayElements(env, array, buf, 0);
    return retVal;
}

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeI2c
 * Method:    i2cWriteBuffer
 * Signature: ([BII)B
 */
JNIEXPORT jbyte JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeI2c_i2cWrite
  (JNIEnv * env, jclass clazz, jbyteArray array, jint len) {
    char* buf = (char *)(*env)->GetByteArrayElements(env, array, 0);
    int retVal = i2cWrite(buf, (int)len);
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
