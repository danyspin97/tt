// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.longrun;

import std.exception : enforce;

import libtt.data.logger_script : LoggerScript;
import libtt.data.longrun_options : LongrunOptions;
import libtt.data.script : Script;
import libtt.data.service : Service;

@safe:
nothrow:

class Longrun : Service
{
public:
    @property ref Script run()
    {
        return m_run;
    }

    @property ref Script finish()
    {
        return m_finish;
    }

    @property void finish(Script finish)
    {
        m_finish = finish;
    }

    @property LoggerScript logger()
    {
        if (m_logger is null)
        {
            return defaultLogger;
        }

        return m_logger;
    }

    @property void logger(LoggerScript logger)
    {
        m_logger = logger;
    }

    this(in string name, in string polishName, in string description, in string path,
            ref LongrunOptions options, ref Script run)
    {
        super(name, polishName, description, path, options);

        enforce(run, "[run] section cannot be null");
        m_run = run;
    }

    override string toString()
    {
        import std.format : format;

        auto ret = format("[main]\n%s\ntype = longrun", super.toString());
        ret ~= format("\n\n[run]\n%s", run.toString());
        if (finish)
        {
            ret ~= format("\n\n[finish]\n%s", finish.toString());
        }
        ret ~= format("\n\n[logger]\n%s", logger.toString());
        ret ~= format("\n\n[options]\n%s", options.toString());
        if (run.environment.getAll().length != 0)
        {
            ret ~= format("\n\n[config]\n%s", run.environment.toString());
        }
        return ret;
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
