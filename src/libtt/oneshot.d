import service : Service;

class Oneshot : Service {
    Script start;
    Script stop;

    Service[] depends;

    this(Service s) {
        super(s);
    }
}
