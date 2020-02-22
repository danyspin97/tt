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

private:
    Script m_start;
    Script m_stop;
}
