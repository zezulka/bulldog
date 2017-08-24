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
package io.silverspoon.bulldog.core.event;

import io.silverspoon.bulldog.core.Edge;
import io.silverspoon.bulldog.core.pin.Pin;

public final class InterruptEventArgs {

    private final Edge edge;
    private final Pin pin;

    public InterruptEventArgs(Pin pin, Edge edge) {
        this.edge = edge;
        this.pin = pin;
    }

    public Edge getEdge() {
        return edge;
    }

    public Pin getPin() {
        return pin;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 79 * hash + this.edge.ordinal();
        hash = 13 * hash + this.pin.getAddress();
        return hash;
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
        final InterruptEventArgs other = (InterruptEventArgs) obj;
        return this.edge.equals(other.edge) && this.pin.equals(other.pin);
    }

    @Override
    public String toString() {
        return "InterruptEventArgs{"
                + "edge=" + edge
                + ", pin=" + pin.getName() + '}';
    }
}
