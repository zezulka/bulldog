/*******************************************************************************
 * Copyright (c) 2016 Silverspoon.io (silverspoon@silverware.io)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/
package io.silverspoon.bulldog.linux.jni;

public class NativeI2c {

   public static native byte i2cReadBuffer(byte[] buffer, int len, int pos);

   public static native byte i2cRead(int pos);

   public static native byte i2cWrite(int pos, byte data);

   public static native byte i2cWriteBuffer(byte[] buffer, int len, int pos);

   public static native void i2cSelectSlave(int slaveAddress);

   public static native void i2cOpen();

   public static native void i2cClose();

}
