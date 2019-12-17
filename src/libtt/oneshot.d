module libtt.oneshot;

import libtt.service : Service;

class Oneshot : Service {
    Script start;
    Script stop;

    Service[] depends;

    this(Service s) {
        super(s);
    }
}
