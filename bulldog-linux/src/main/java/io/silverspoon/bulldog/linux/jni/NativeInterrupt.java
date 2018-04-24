package io.silverspoon.bulldog.linux.jni;

public class NativeInterrupt {
    /*
      The return value corresponds to the io.silverspoon.bulldog.core.Edge
      enum instance index.
     */
    public static native int newEvent(int address);
}
