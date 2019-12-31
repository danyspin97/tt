module libtt.services.bundle;

import libtt.services.service;

class Bundle : Service
{
public:
    @property const(Service[]) contents() { return contents; }
    this(Service s)
    {
        super(s);
    }
private:
    Service[] m_contents;
}
