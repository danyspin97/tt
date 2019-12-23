module libtt.services.bundle;

import libtt.services.service;

class Bundle : Service {
    Service[] contents;

    this(Service s) {
        super(s);
    }
}
