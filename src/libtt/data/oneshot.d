// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.oneshot;

@safe:
nothrow:

import libtt.data.environment : Environment;
import libtt.data.oneshot_options : OneshotOptions;
import libtt.data.script : Script;
import libtt.data.service : Service;

class Oneshot : Service
{
public:
    @property ref Script start()
    {
        return m_start;
    }

    @property ref Script stop()
    {
        return m_stop;
    }

    @property void stop(Script stop)
    {
        m_stop = stop;
    }

    this(in string name, in string polishName, in string description, in string path,
            ref OneshotOptions options, ref Script start)
    {
        super(name, polishName, description, path, options);

        m_start = start;
    }

    override string toString()
    {
        import std.format : format;

        auto ret = format("[main]\n%s\ntype = oneshot", super.toString());
        ret ~= format("\n\n[start]\n%s", start.toString());
        if (stop)
        {
            ret ~= format("\n\n[stop]\n%s", stop.toString());
        }
        ret ~= format("\n\n[options]\n%s", options.toString());
        if (start.environment.getAll().length != 0)
        {
            ret ~= format("\n\n[config]\n%s", start.environment.toString());
        }
        return ret;
    }

private:
    Script m_start;
    Script m_stop;
}
