package io.silverspoon.bulldog.linux.jni;
/**
 *
 * @author Miloslav Zezulka, 2017
 */
public class NativeGpio {
    public static native int readSignal(int pinAddress);
    public static native void writeSignal(int value, int pinAddress);
}
