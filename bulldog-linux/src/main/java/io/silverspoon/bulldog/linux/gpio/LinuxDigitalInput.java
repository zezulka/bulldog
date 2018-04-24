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
package io.silverspoon.bulldog.linux.gpio;

import io.silverspoon.bulldog.core.Edge;
import io.silverspoon.bulldog.core.Signal;
import io.silverspoon.bulldog.core.event.InterruptEventArgs;
import io.silverspoon.bulldog.core.event.InterruptListener;
import io.silverspoon.bulldog.core.gpio.base.AbstractDigitalInput;
import io.silverspoon.bulldog.core.gpio.base.DigitalIOFeature;
import io.silverspoon.bulldog.core.pin.Pin;
import io.silverspoon.bulldog.linux.io.LinuxInterruptThread;

public class LinuxDigitalInput extends AbstractDigitalInput
        implements InterruptListener {

    private LinuxInterruptThread interruptControl;
    private Edge lastEdge;
    private volatile long lastInterruptTime;

    public LinuxDigitalInput(Pin pin) {
        super(pin);
        interruptControl = new LinuxInterruptThread(getPin());
        interruptControl.addListener(this);
    }

    @Override
    public Signal read() {
        return getPin().getFeature(DigitalIOFeature.class).read();
    }

    @Override
    public void addInterruptListener(InterruptListener listener) {
        super.addInterruptListener(listener);
        interruptControl.addListener(listener);
        if (areInterruptsEnabled() && !interruptControl.isRunning()) {
            interruptControl.start();
        }
    }

    @Override
    public void removeInterruptListener(InterruptListener listener) {
        super.removeInterruptListener(listener);
        interruptControl.removeListener(listener);
        if (getInterruptListeners().isEmpty()) {
            interruptControl.stop();
        }
    }

    @Override
    public void clearInterruptListeners() {
        super.clearInterruptListeners();
        interruptControl.stop();
    }

    @Override
    protected void enableInterruptsImpl() {
        if (getInterruptListeners().size() > 0 && !interruptControl.isRunning()) {
            interruptControl.start();
        }
    }

    @Override
    protected void disableInterruptsImpl() {
    }

    @Override
    protected void setupImpl() {
    }

    @Override
    protected void teardownImpl() {
        disableInterrupts();
    }

    @Override
    public void setInterruptTrigger(Edge edge) {
        super.setInterruptTrigger(edge);
    }

    @Override
    public void interruptRequest(InterruptEventArgs args) {
        if (lastEdge != null && lastEdge.equals(args.getEdge())) {
            return;
        }

        long delta = System.currentTimeMillis() - lastInterruptTime;
        if (delta <= this.getInterruptDebounceMs()) {
            return;
        }

        lastInterruptTime = System.currentTimeMillis();
        lastEdge = args.getEdge();
        //fireInterruptEvent(new InterruptEventArgs(getPin(), args.getEdge()));
    }
}
