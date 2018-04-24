#include "io_silverspoon_bulldog_linux_jni_NativeInterrupt.h"
#include "../bcm/bcm2835.h"
#include <string.h>
#include <stdio.h>

#define EDGE_NONE 0
#define EDGE_RISING 1
#define EDGE_FALLING 2
/*
 * Class:     io_silverspoon_bulldog_linux_jni_NativeInterrupt
 * Method:    hasNewEvent
 * Signature: ()Z
 */
JNIEXPORT jint JNICALL Java_io_silverspoon_bulldog_linux_jni_NativeInterrupt_newEvent
  (JNIEnv* env, jobject obj, jint address) {
    bcm2835_gpio_fsel(address, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(address, BCM2835_GPIO_PUD_UP);
    bcm2835_gpio_ren(address);
    bcm2835_gpio_fen(address);
    bcm2835_gpio_clr_len(address);
    bcm2835_gpio_clr_hen(address);
    if(bcm2835_gpio_eds(address)){
    	 bcm2835_gpio_set_eds(address);
    	 return bcm2835_gpio_lev(address) ? EDGE_RISING : EDGE_FALLING;
    	 //jclass enumClass = (*env)->FindClass(env,"io/silverspoon/bulldog/core/Edge");
         //jmethodID getNameMethod = (*env)->GetMethodID(env,enumClass, "name", "()Ljava/lang/String;");
         //jstring value = (jstring)(*env)->CallObjectMethod(env,pin, getNameMethod);
         //const char* valueNative = (*env)->GetStringUTFChars(env,value, 0);
         //return (strcmp(valueNative, "Both") == 0) ||
         //       ((curr == 0) && (strcmp(valueNative, "Falling") == 0)) ||
         //       ((curr != 0) && (strcmp(valueNative, "Rising") == 0));
    }
    return EDGE_NONE;
  }