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

import io.silverspoon.bulldog.core.io.bus.Bus;
import io.silverspoon.bulldog.linux.jni.NativeTools;

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public abstract class AbstractLinuxBus implements Bus {

   protected static final String ERROR_OPENING_BUS = "Bus '%s' could not be opened";
   protected static final String ERROR_CLOSING_BUS = "Bus could not be closed. Invalid file descriptor?";
   protected static final String ERROR_SELECTING_SLAVE = "Error selecting slave on address %s";
   protected static final String ERROR_WRITING_BYTE = "Byte could not be written to bus";
   protected static final String ERROR_READING_BYTE = "Byte could not be read from bus";
   protected static final String ERROR_BUS_NOT_OPENED = "Bus has not been opened!";
   protected static final String EXCEPTION_STREAM = "This implementation is not based on sys fs, therefore does not work with Stream classes.";

   private String deviceFilePath;
   private boolean isOpen = false;
   private int fileDescriptor;
   private String alias;
   private String name;

   public AbstractLinuxBus(String name) {
      this.name = name;
   }

   public String getAlias() {
      return alias;
   }

   public void setAlias(String alias) {
      this.alias = alias;
   }

   public String getName() {
      return name;
   }

   public boolean isOpen() {
      return isOpen;
   }

   public void open() throws IOException {
      if (isOpen()) {
         return;
      }
      openImpl();
      isOpen = true;
   }

   protected abstract void openImpl();

   public void close() throws IOException {
         if (!isOpen()) {
            return;
         }

         fileDescriptor = 0;
         isOpen = false;
         closeImpl();
   }

   protected abstract void closeImpl();

   @Override
   public FileInputStream getInputStream() throws IOException {
      throw new UnsupportedOperationException(EXCEPTION_STREAM);
   }

   @Override
   public FileOutputStream getOutputStream() throws IOException {
      throw new UnsupportedOperationException(EXCEPTION_STREAM);
   }

   protected int getFileDescriptor() {
      return this.fileDescriptor;
   }

   @Override
   public boolean equals(Object obj) {
      if (this == obj) {
         return true;
      }
      if (obj == null) {
         return false;
      }
      if (getClass() != obj.getClass()) {
         return false;
      }
      return true;
   }

   @Override
   public int hashCode() {
      final int prime = 31;
      int result = 1;
      result = prime
            * result
            + ((this.name == null) ? 0 : this.name.hashCode());
      return result;
   }
}
