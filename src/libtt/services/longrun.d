module libtt.services.longrun;

import libtt.services.service : Service;
import libtt.services.script : Script;
import libtt.services.environment : Environment;

class Longrun : Service
{
public:
    class Logger : Script
    {
    public:
        @property string destination() { return m_destination; }
        @property uint maxsize() { return m_maxsize; }

        this (
            string execute,
            string user,
            string group,
            Environment environment,
            string destination,
            uint maxsize,
            Type type,
            string shebang = ""
        ) {
            super(getDefaultExecute(),
            user,
            group,
            environment,
            type,
            shebang);
        }
    private:
        string getDefaultExecute()
        {
            // TODO add default logger script
            return "";
        }

        string m_destination;
        uint m_maxsize;
    }

    @property ref const(Script) run () { return m_run; }
    @property ref const(Script) finish () { return m_finish; }
    @property ref const(Service[]) dependencies () { return m_depends; }

private:
    Script m_run;
    Script m_finish;

    Service[] m_depends;
}
