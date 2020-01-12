// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.services.oneshot;

import libtt.services.oneshot_options : OneshotOptions;
import libtt.services.service : Service;
import libtt.services.script : Script;

class Oneshot : Service
{
public:
    @property ref const(Script) start() { return m_start; }
    @property ref const(Script) stop() { return m_stop; }
    @property const(Service[]) dependencies() { return m_depends; }

    this(
        string name,
        string polishName,
        string description,
        string path,
        ref OneshotOptions options,
        ref Script start,
        ref Script stop,
        Service[] dependencies
    ) {
        super(
            name,
            polishName,
            description,
            path,
            options
        );

        m_start = start,
        m_stop = stop;
        m_depends = dependencies;
    }

private:
    Script m_start;
    Script m_stop;

    Service[] m_depends;
}
