module libtt.oneshot;

import libtt.service : Service;
import libtt.script : Script;

class Oneshot : Service {
    Script start;
    Script stop;

    Service[] depends;

    this(Service s) {
        super(s);

        start = new Script();
        stop = new Script();
    }
}
