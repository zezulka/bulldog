package io.silverspoon.bulldog.linux.io;

import io.silverspoon.bulldog.core.Edge;
import io.silverspoon.bulldog.core.event.InterruptEventArgs;
import io.silverspoon.bulldog.core.event.InterruptListener;
import io.silverspoon.bulldog.core.pin.Pin;
import io.silverspoon.bulldog.core.util.BulldogUtil;
import io.silverspoon.bulldog.linux.jni.NativeInterrupt;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.List;

public class LinuxInterruptThread implements Runnable {
    private Thread listenerThread = new Thread(this);
    private boolean running = false;
    private final Pin pin;
    private List<InterruptListener> listeners = new ArrayList<>();
    private static final Logger LOGGER = LoggerFactory.
            getLogger(LinuxInterruptThread.class);

    public LinuxInterruptThread(Pin pin) {
        listenerThread.setDaemon(true);
        this.pin = pin;
    }

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

    public void stop() {
        if (!running) {
            return;
        }
        running = false;
        //TODO

        //block until thread is dead
        while (listenerThread.isAlive()) {
            BulldogUtil.sleepMs(10);
        }
    }

    @Override
    public void run() {
        while (running) {
            Edge e;
            if (!(e = Edge.valueOf(NativeInterrupt.newEvent(pin.getAddress())))
                    .equals(Edge.None)) {
                fireEvent(e);
            }
        }
    }

    public boolean isRunning() {
        return listenerThread.isAlive();
    }

    public void addListener(InterruptListener listener) {
        listeners.add(listener);
    }

    public void removeListener(InterruptListener listener) {
        listeners.remove(listener);
    }

    public void clearListeners() {
        this.listeners.clear();
    }

    protected void fireEvent(Edge edge) {
        for (InterruptListener listener : this.listeners) {
            listener.interruptRequest(new InterruptEventArgs(pin, edge));
        }
    }
}
