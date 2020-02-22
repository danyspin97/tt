// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.logger_script;

import libtt.data.environment : Environment;
import libtt.data.script : Script;

@safe:
nothrow:

class LoggerScript : Script
{
public:
    @property string destination() const
    {
        return m_destination;
    }

    @property void destination(in string destination)
    {
        m_destination = destination;
    }

    @property uint maxsize() const
    {
        return m_maxsize;
    }

    @property void maxsize(in uint maxsize)
    {
        m_maxsize = maxsize;
    }

    this(in string execute, in string shebang, Environment environment)
    {
        super(execute, shebang, environment);
    }

    this(in string shebang, Environment environment)
    {
        super(defaultExecute, shebang, environment);
    }

private:
    string defaultExecute() const
    {
        // TODO add default logger script
        return "";
    }

    string m_destination;
    uint m_maxsize;
}
