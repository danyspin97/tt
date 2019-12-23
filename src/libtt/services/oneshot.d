module libtt.services.oneshot;

import libtt.services.service;
import libtt.services.script;

class Oneshot : Service {
    Script start;
    Script stop;

    Service[] depends;

    string lol;

    this(Service s) {
        super(s);

        start = new Script();
        stop = new Script();
    }
}
