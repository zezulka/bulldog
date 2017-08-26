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

import io.silverspoon.bulldog.core.util.BulldogUtil;
import io.silverspoon.bulldog.linux.jni.NativePollResult;
import io.silverspoon.bulldog.linux.sysfs.SysFsPin;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import static java.nio.charset.CoderResult.OVERFLOW;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import static java.nio.file.StandardWatchEventKinds.ENTRY_MODIFY;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.ArrayList;
import java.util.List;

/**
 * As suggested by the class name, this implementation is based on Watch Service
 * API, which is part of NIO. New watcher with
 * java.nio.file.StandardWatchEventKinds.ENTRY_MODIFY is created on the pin base
 * directory, which then filters out the only relevant file -
 * /sys/class/gpio/gpioX/value representing binary voltage level in this virtual
 * file system. From the kernel sysfs documentation, it is quite clear that the
 * only values the file can have (without any further interference from the user
 * side, that is) is either "1" or "0". This can be used for implementing
 * Java-only "interrupt service routine" triggers (where interrupt here is not
 * exactly the one known in operating systems) without any JNI calls, as in
 * LinuxEpollThread, or implementing programmer's own polling version.
 */
public class LinuxWatchServiceThread implements SysfsFileChangeSubject {

    private Thread listenerThread = new Thread(this);
    private boolean running = false;
    private boolean isSetup = false;
    private final Path pathToPinDirectory;
    private RandomAccessFile gpioValFile;
    private final List<LinuxEpollListener> listeners = new ArrayList<LinuxEpollListener>();
    private final SysFsPin sysFsPin;

    public LinuxWatchServiceThread(SysFsPin pin) {
        this.sysFsPin = pin;
        pathToPinDirectory = sysFsPin.getPinDirectory();
        listenerThread.setDaemon(true);
    }

    @Override
    public void setup() {
        if (!isSetup) {
            try {
                gpioValFile = new RandomAccessFile(sysFsPin.getValueFilePath().toString(), "r");
                isSetup = true;
            } catch (FileNotFoundException ex) {
                throw new RuntimeException(String.format(
                        "sysfs pin '%s' has not been exported to user space!",
                        sysFsPin.getPinDirectory()
                ), ex
                );
            }
        }
    }

    @Override
    public void start() {
        if (running) {
            return;
        }
        if (listenerThread.isAlive()) {
            return;
        }
        listenerThread = new Thread(this);
        listenerThread.setDaemon(true);
        listenerThread.start();
        running = true;
    }

    @Override
    public void stop() {
        if (!running) {
            return;
        }
        running = false;

        //block until thread is dead
        while (listenerThread.isAlive()) {
            BulldogUtil.sleepMs(10);
        }
    }

    @Override
    public void run() {
        try (final WatchService watchService = FileSystems.getDefault().newWatchService()) {
            final WatchKey watchKey = pathToPinDirectory.register(watchService, ENTRY_MODIFY);
            while (running) {
                final WatchKey wk = watchService.take();
                for (WatchEvent<?> event : wk.pollEvents()) {
                    if (event == OVERFLOW) {
                        continue;
                    }
                    final Path changed = (Path) event.context();
                    if (changed.endsWith("value")) {
                        gpioValFile.seek(0);
                        byte val = Byte.valueOf(gpioValFile.readLine());
                        fireEpollEvent(new NativePollResult(0, 0, new byte[]{val}));
                    }
                }
                wk.reset();
            }
        } catch (IOException | InterruptedException ex) {
            // Panic and leave
            throw new RuntimeException(ex);
        }
    }

    @Override
    public boolean isRunning() {
        return listenerThread.isAlive();
    }

    @Override
    public void teardown() {
        stop();
        if (isSetup) {
            try {
                gpioValFile.close();
            } catch (IOException ex) {
            } finally {
                isSetup = false;
            }
        }

    }

    @Override
    public void addListener(LinuxEpollListener listener) {
        this.listeners.add(listener);
    }

    @Override
    public void removeListener(LinuxEpollListener listener) {
        this.listeners.remove(listener);
    }

    @Override
    public void clearListeners() {
        this.listeners.clear();
    }

    protected void fireEpollEvent(NativePollResult result) {
        NativePollResult[] results = new NativePollResult[]{result};
        for (LinuxEpollListener listener : this.listeners) {
            listener.processEpollResults(results);
        }
    }
}
