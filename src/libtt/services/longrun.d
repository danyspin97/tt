// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.services.longrun;

import libtt.services.service : Service;
import libtt.services.script : Script;
import libtt.services.environment : Environment;
import libtt.services.logger_script : LoggerScript;

class Longrun : Service
{
public:
    @property ref const(Script) run () { return m_run; }
    @property ref const(Script) finish () { return m_finish; }
    @property ref const(Service[]) dependencies () { return m_depends; }

private:
    Script m_run;
    Script m_finish;
    Logger m_logger;

    Service[] m_depends;
}
