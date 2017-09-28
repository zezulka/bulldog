#include <jni.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "../bcm/bulldogGpio.h"
#include "io_silverspoon_bulldog_linux_jni_NativeGpio.h"

/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeGpio
 * Method:    readSignal
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeGpio_readSignal
  (JNIEnv * env, jclass clazz, jint pinAddr) {
    return readSignal(pinAddr);
  }


/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeGpio
 * Method:    writeSignal
 * Signature: (II)V
 */
  JNIEXPORT void JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeGpio_writeSignal
    (JNIEnv * env, jclass clazz, jint value, jint pinAddr) {
    writeSignal(value, pinAddr);
}
