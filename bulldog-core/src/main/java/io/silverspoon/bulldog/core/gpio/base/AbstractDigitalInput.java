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
package io.silverspoon.bulldog.core.gpio.base;

import io.silverspoon.bulldog.core.Edge;
import io.silverspoon.bulldog.core.Signal;
import io.silverspoon.bulldog.core.event.InterruptEventArgs;
import io.silverspoon.bulldog.core.event.InterruptListener;
import io.silverspoon.bulldog.core.gpio.DigitalInput;
import io.silverspoon.bulldog.core.pin.AbstractPinFeature;
import io.silverspoon.bulldog.core.pin.Pin;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public abstract class AbstractDigitalInput extends AbstractPinFeature implements DigitalInput {

    private static final int MAX_DEBOUNCE_COUNT = 10;

    private static final String NAME_FORMAT = "Digital Input on Pin %s";
    private Edge trigger = Edge.Both;
    private final List<InterruptListener> interruptListeners = Collections.synchronizedList(new ArrayList<InterruptListener>());
    private int debounceMs = 0;
    private boolean areInterruptsEnabled = true;

    public AbstractDigitalInput(Pin pin) {
        super(pin);
    }

    @Override
    public String getName() {
        return String.format(NAME_FORMAT, getPin().getName());
    }

    @Override
    public void setInterruptTrigger(Edge edge) {
        this.trigger = edge;
    }

    @Override
    public void setInterruptDebounceMs(int milliSeconds) {
        this.debounceMs = milliSeconds;
    }

    @Override
    public int getInterruptDebounceMs() {
        return debounceMs;
    }

    @Override
    public Edge getInterruptTrigger() {
        return this.trigger;
    }

    @Override
    public void addInterruptListener(InterruptListener listener) {
        interruptListeners.add(listener);
    }

    @Override
    public void removeInterruptListener(InterruptListener listener) {
        interruptListeners.remove(listener);
    }

    @Override
    public List<InterruptListener> getInterruptListeners() {
        return interruptListeners;
    }

    @Override
    public void clearInterruptListeners() {
        interruptListeners.clear();
    }

    @Override
    public void fireInterruptEvent(InterruptEventArgs args) {
        synchronized (interruptListeners) {
            if (areInterruptsEnabled() == false) {
                return;
            }
            for (InterruptListener listener : interruptListeners) {
                listener.interruptRequest(args);
            }
        }
    }

    @Override
    public Signal readDebounced(int debounceTime) {
        long startTime = System.currentTimeMillis();
        long delta = 0;
        Signal currentState = read();
        int counter = 0;
        while (delta < debounceTime) {
            Signal reading = read();

            if (reading == currentState && counter > 0) {
                counter--;
            }

            if (reading != currentState) {
                counter++;
            }

            if (counter >= MAX_DEBOUNCE_COUNT) {
                counter = 0;
                currentState = reading;
                return currentState;
            }

            delta = System.currentTimeMillis() - startTime;
        }

        return currentState;
    }

    @Override
    public boolean areInterruptsEnabled() {
        return areInterruptsEnabled;
    }

    @Override
    public void enableInterrupts() {
        enableInterruptsImpl();
        areInterruptsEnabled = true;
    }

    @Override
    public void disableInterrupts() {
        disableInterruptsImpl();
        areInterruptsEnabled = false;
    }

    protected abstract void enableInterruptsImpl();

    protected abstract void disableInterruptsImpl();
}
