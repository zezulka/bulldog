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
package io.silverspoon.bulldog.linux.io;

import io.silverspoon.bulldog.core.gpio.DigitalOutput;
import io.silverspoon.bulldog.core.io.bus.BusConnection;
import io.silverspoon.bulldog.core.io.bus.spi.SpiBus;
import io.silverspoon.bulldog.core.io.bus.spi.SpiConnection;
import io.silverspoon.bulldog.core.io.bus.spi.SpiMessage;
import io.silverspoon.bulldog.core.io.bus.spi.SpiMode;
import io.silverspoon.bulldog.core.pin.Pin;
import io.silverspoon.bulldog.core.platform.Board;
import io.silverspoon.bulldog.core.util.BulldogUtil;
import io.silverspoon.bulldog.linux.jni.NativeSpi;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class LinuxSpiBus extends AbstractLinuxBus implements SpiBus {

   private List<Integer> slaveSelectIndexes = new ArrayList<Integer>();
   private Board board;

   private int speed = 0;
   private int bitsPerWord = 8;
   private int delayMicroSeconds = 0;
   private SpiMode mode = SpiMode.Mode0;
   private boolean lsbFirst = false;

   public LinuxSpiBus(String name, Board board) {
      super(name);
      this.board = board;
   }

   public BusConnection createConnection(int address) {
      return createSpiConnection(address);
   }

   @Override
   public SpiConnection createSpiConnection() {
      return new SpiConnection(this);
   }

   @Override
   public SpiConnection createSpiConnection(int address) {
      return new SpiConnection(this, address);
   }

   @Override
   public SpiConnection createSpiConnection(DigitalOutput output) {
      throw new UnsupportedOperationException("not supported by this implementation");
      //return new SpiConnection(this, output.getPin().getAddress());
   }

   @Override
   public SpiConnection createSpiConnection(DigitalOutput... chipSelects) {
      throw new UnsupportedOperationException("not supported by this implementation");
      //return new SpiConnection(this, chipSelects);
   }

   @Override
   public SpiConnection createSpiConnection(int... addresses) {
      return new SpiConnection(this, addresses);
   }

   private void configure() {
      if (isOpen()) {
         NativeSpi.spiConfig(getMode().getNumericValue(), getSpeedInHz(), getBitsPerWord(), lsbFirst);
      }
   }

   public byte readByte(byte value) {
      byte[] bytes = new byte[]{value};
      transfer(bytes);
      return bytes[0];
   }

   @Override
   public byte readByte() {
       throw new UnsupportedOperationException("this operation is not supported by SPI bus!");
   }

   @Override
   protected void openImpl() {
      NativeSpi.spiOpen(getMode().getNumericValue(), getSpeedInHz(), getBitsPerWord(), lsbFirst);
   }

   @Override
   protected void closeImpl() {
      NativeSpi.spiClose();
   }

   @Override
   public int readBytes(byte[] buffer) throws IOException {
      transfer(buffer);
      return 0;
   }

   @Override
   public String readString() throws IOException {
      throw new UnsupportedOperationException("not supported by this implementation");
   }

   @Override
   public void selectSlave(DigitalOutput chipSelect) {
       throw new UnsupportedOperationException("not supported by this implementation");
   }

   @Override
   public void selectSlave(int index) throws IOException {
      getslaveSelectIndexes().clear();
      getslaveSelectIndexes().add(index);
   }

   @Override
   public void selectSlaves(Integer... chipSelectIndexes) {
     getslaveSelectIndexes().clear();
     if (chipSelectIndexes != null && chipSelectIndexes.length > 0) {
         getslaveSelectIndexes().addAll(Arrays.asList(chipSelectIndexes));
     }
   }

   @Override
   public void selectSlaves(DigitalOutput... chipSelects) {
       throw new UnsupportedOperationException("not supported by this implementation");
   }

   private void startOutput() {
      for (Integer output : getslaveSelectIndexes()) {
         NativeSpi.spiSelectSlave(output);
      }
   }

   private void endOutput() {
   }

   public void writeByte(int b) throws IOException {
         startOutput();
         transfer(new byte[]{(byte)b});
         endOutput();
   }

   @Override
   public void writeBytes(byte[] bytes) throws IOException {
      startOutput();
      transfer(bytes);
      endOutput();
   }

   @Override
   public void writeString(String string) throws IOException {
      writeBytes(string.getBytes());
   }

   @Override
   public SpiMessage transfer(byte[] buffer) {
      startOutput();
      byte[] sentBytes = buffer.clone();

      ByteBuffer bufferPointer = ByteBuffer.allocateDirect(buffer.length);
      bufferPointer.put(buffer);
      bufferPointer.rewind();

      int length = buffer.length / (getBitsPerWord() / 8);
      NativeSpi.spiTransfer(bufferPointer, bufferPointer, length, getDelayMicroseconds(), getSpeedInHz(), getBitsPerWord());
      endOutput();

      return createSpiMessage(bufferPointer, sentBytes);
   }

   private SpiMessage createSpiMessage(ByteBuffer buffer, byte[] sentBytes) {
      SpiMessage message = new SpiMessage();
      byte[] rxBytes = new byte[sentBytes.length];
      buffer.get(rxBytes);
      message.setReceivedBytes(rxBytes);
      message.setSentBytes(sentBytes);
      return message;
   }

   @Override
   public List<Integer> getslaveSelectIndexes() {
      return slaveSelectIndexes;
   }

   @Override
   public boolean isSlaveSelected(int index) {
      for (Integer output : getslaveSelectIndexes()) {
         if (output == index) {
            return true;
         }
      }

      return false;
   }

   @Override
   public void broadcast(byte[] bytes, Integer... chipSelects) throws IOException {
      selectSlaves(chipSelects);
      writeBytes(bytes);
   }

   @Override
   public Pin getMISO() {
      throw new UnsupportedOperationException("Unsupported by generic bus object: " + this.getClass());
   }

   @Override
   public Pin getMOSI() {
      throw new UnsupportedOperationException("Unsupported by generic bus object: " + this.getClass());
   }

   @Override
   public Pin getSCLK() {
      throw new UnsupportedOperationException("Unsupported by generic bus object: " + this.getClass());
   }

   @Override
   public int getSpeedInHz() {
      return this.speed;
   }

   @Override
   public void setSpeedInHz(int speedInHz) {
      this.speed = speedInHz;
      configure();
   }

   @Override
   public void setBitsPerWord(int bpw) {
      this.bitsPerWord = bpw;
      configure();
   }

   @Override
   public int getBitsPerWord() {
      return this.bitsPerWord;
   }

   @Override
   public void setDelayMicroseconds(int delay) {
      this.delayMicroSeconds = delay;
   }

   @Override
   public int getDelayMicroseconds() {
      return this.delayMicroSeconds;
   }

   @Override
   public void setMode(SpiMode mode) {
      this.mode = mode;
      configure();
   }

   @Override
   public SpiMode getMode() {
      return mode;
   }

   @Override
   public void useLeastSignificantBitFirst() {
      lsbFirst = true;
      configure();
   }

   @Override
   public void useMostSignificantBitFirst() {
      lsbFirst = false;
      configure();
   }

   @Override
   public boolean isLSBUsed() {
      return lsbFirst;
   }

   @Override
   public boolean isMSBUsed() {
      return !lsbFirst;
   }

}
