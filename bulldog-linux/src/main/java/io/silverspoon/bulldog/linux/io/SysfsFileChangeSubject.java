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

/**
 * Interface which represents thread running in the background, collecting
 * information about changes submitted in sysfs files, usually
 * /sys/class/gpio/gpioX/value files. Classes implementing this interface are
 * the subject components of Observer pattern.
 */
public interface SysfsFileChangeSubject extends Runnable {

    /**
     * Performs necessary setup of the thread. This might include performing
     * checks whether given sysfs files exist, existence of the pin on board,...
     * This can also be a no-op, if no preparation is required.
     */
    void setup();

    /**
     * Starts the thread in the background.
     */
    void start();

    void stop();

    /**
     * Tells whether thread instance represented by this interface is active
     * (i.e. running in the thread context).
     * @return
     */
    boolean isRunning();

    void addListener(LinuxEpollListener listener);

    void removeListener(LinuxEpollListener listener);

    void clearListeners();

    void teardown();
}
