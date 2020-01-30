// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.services.logger_script;

import libtt.services.environment : Environment;
import libtt.services.script : Script;

class LoggerScript : Script
{
public:
    @property string destination() { return m_destination; }
    @property void destination(string destination)
    {
        m_destination = destination;
    }
    @property uint maxsize() { return m_maxsize; }
    @property void maxsize(uint maxsize) { m_maxsize = maxsize; }

    this (
        string shebang,
        Environment environment
    ) {
        super(
            defaultExecute,
            shebang,
            environment
        );

        execute = defaultExecute;
    }
private:
    string defaultExecute()
    {
        // TODO add default logger script
        return "";
    }

    string m_destination;
    uint m_maxsize;
}
