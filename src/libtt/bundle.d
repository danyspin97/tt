module libtt.bundle;

import libtt.service : Service;

class Bundle : Service {
    Service[] contents;

    this(Service s) {
        super(s);
    }
}
