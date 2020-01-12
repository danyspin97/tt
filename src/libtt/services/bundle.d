module libtt.services.bundle;

import libtt.services.service;

class Bundle : Service
{
public:
    @property const(Service[]) contents() { return contents; }

private:
    Service[] m_contents;
}
