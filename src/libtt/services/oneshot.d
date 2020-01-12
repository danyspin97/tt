module libtt.services.oneshot;

import libtt.services.service : Service;
import libtt.services.script : Script;

class Oneshot : Service
{
public:
    @property ref const(Script) start() { return m_start; }
    @property ref const(Script) stop() { return m_stop; }
    @property const(Service[]) dependencies() { return m_depends; }

private:
    Script m_start;
    Script m_stop;

    Service[] m_depends;
}
