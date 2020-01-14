// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.services.longrun;

import libtt.services.logger_script : LoggerScript;
import libtt.services.longrun_options : LongrunOptions;
import libtt.services.script : Script;
import libtt.services.service : Service;

class Longrun : Service
{
public:
    @property ref const(Script) run () { return m_run; }
    @property ref const(Script) finish () { return m_finish; }

    this(
        string name,
        string polishName,
        string description,
        string path,
        ref LongrunOptions options,
        ref Script run,
        ref Script finish,
        ref Script logger,
    ) {
        super(
            name,
            polishName,
            description,
            path,
            options
        );

        m_run = run;
        m_finish = finish;

        if (logger is null)
        {
            m_logger = defaultLogger;
        }
    }

private:
    LoggerScript defaultLogger()
    {
        // TODO: instance a LoggerScript object
        return null;
    }

    Script m_run;
    Script m_finish;
    LoggerScript m_logger;
}
