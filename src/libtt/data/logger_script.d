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
    @property string serviceToLog() const
    {
        return m_serviceToLog;
    }

    this(in Type type, in string execute, in string shebang, Environment environment, in string serviceToLog, in string user, in string group)
    {
        super(type, execute, shebang, environment);
        m_serviceToLog = serviceToLog;
        this.user = user;
        this.group = group;
    }

private:
    string m_serviceToLog;
}
