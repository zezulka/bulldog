/** *****************************************************************************
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
 ****************************************************************************** */
package io.silverspoon.bulldog.linux.io;

import io.silverspoon.bulldog.core.io.bus.BusConnection;
import io.silverspoon.bulldog.core.io.bus.i2c.I2cBus;
import io.silverspoon.bulldog.core.io.bus.i2c.I2cConnection;
import io.silverspoon.bulldog.core.pin.Pin;
import io.silverspoon.bulldog.linux.jni.NativeI2c;

import java.io.IOException;

public class LinuxI2cBus extends AbstractLinuxBus implements I2cBus {

    private int selectedSlaveAddress;
    /*
    Maximum value which is BSC controller capable of holding.
     */
    public static final int MAX_NUM_BYTES = 1 << 16 - 1;

    public LinuxI2cBus(String name) {
        super(name);
    }

    @Override
    public BusConnection createConnection(int address) {
        return createI2cConnection(address);
    }

    @Override
    public I2cConnection createI2cConnection(int address) {
        I2cConnection i2cConn = null;
        try {
            open();
            i2cConn = new I2cConnection(this, address);
            i2cConn.getBus().selectSlave(address);
        } catch (IOException ex) {
            System.err.println(ex.getMessage());
        }
        return i2cConn;
    }

    @Override
    public int getFrequency() {
        throw new UnsupportedOperationException("Unsupported by generic bus object: " + this.getClass());
    }

    @Override
    public Pin getSCL() {
        throw new UnsupportedOperationException("Unsupported by generic bus object: " + this.getClass());
    }

    @Override
    public Pin getSDA() {
        throw new UnsupportedOperationException("Unsupported by generic bus object: " + this.getClass());
    }

    @Override
    public byte readByte() throws IOException {
        return 0x00;
    }

    @Override
    protected void openImpl() {
        NativeI2c.i2cOpen();
    }

    @Override
    protected void closeImpl() {
        NativeI2c.i2cClose();
    }

    @Override
    public int readBytes(byte[] buffer) throws IOException {
        return NativeI2c.i2cRead(buffer, buffer.length);
    }

    @Override
    public String readString() throws IOException {
        throw new UnsupportedOperationException("Not supported in this implementation");
    }

    @Override
    public byte readByteFromRegister(int register) throws IOException {
        NativeI2c.i2cWrite(new byte[]{(byte) register}, 1);
        byte[] oneByte = new byte[]{0};
        NativeI2c.i2cRead(oneByte, 1);
        return oneByte[0];
    }

    @Override
    public int readBytesFromRegister(int register, byte[] buffer) throws IOException {
        NativeI2c.i2cWrite(new byte[]{(byte) register}, 1);
        return NativeI2c.i2cRead(buffer, buffer.length);
    }

    @Override
    public void selectSlave(int address) throws IOException {
        if (!isOpen()) {
            open();
        }

        NativeI2c.i2cSelectSlave(address);
        this.selectedSlaveAddress = address;
    }

    @Override
    public void writeByte(int b) throws IOException {
        int returnValue = NativeI2c.i2cWrite(new byte[]{(byte) b}, 1);
        if (returnValue < 0) {
            throw new IOException(ERROR_WRITING_BYTE);
        }
    }

    @Override
    public void writeBytes(byte[] bytes) throws IOException {
        int returnValue = NativeI2c.i2cWrite(bytes, bytes.length);
        if (returnValue < 0) {
            throw new IOException(ERROR_WRITING_BYTE);
        }
    }

    @Override
    public void writeString(String string) throws IOException {
        throw new UnsupportedOperationException("Not supported in this implementation yet");
    }

    @Override
    public void writeByteToRegister(int register, int data) throws IOException {
        NativeI2c.i2cWrite(new byte[]{(byte) register, (byte) data}, 2);
    }

    @Override
    public void writeBytesToRegister(int register, byte[] data) throws IOException {
        byte[] bytesToWrite = new byte[data.length + 1];
        bytesToWrite[0] = (byte) register;
        System.arraycopy(data, 0, bytesToWrite, 1, data.length);
        NativeI2c.i2cWrite(bytesToWrite, data.length);
    }

    @Override
    public boolean isSlaveSelected(int address) {
        return selectedSlaveAddress == address;
    }

}
