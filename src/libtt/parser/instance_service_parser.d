// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.instance_service_parser;

import libtt.parser.longrun_instance_replacer : LongrunInstanceReplacer;
import libtt.parser.oneshot_instance_replacer : OneshotInstanceReplacer;
import libtt.parser.service_parser : ServiceParser;
import libtt.services.service : Service;
import libtt.services.longrun : Longrun;
import libtt.services.oneshot : Oneshot;

class InstanceServiceParser
{
public:
    @property Service service() { return m_service; }
    this (string path, string instanceName)
    {
        m_service = new ServiceParser(path).service;
        replaceTokenWithName(instanceName);
    }
private:
    void replaceTokenWithName(string instanceName)
    {
        if (auto oneshot = cast(Oneshot)m_service)
        {
            new OneshotInstanceReplacer(instanceName, oneshot);
            return;
        }
        if (auto longrun = cast(Longrun)m_service)
        {
            new LongrunInstanceReplacer(instanceName, longrun);
            return;
        }

        throw new Exception("Bundle services cannot be instanced");
    }

    Service m_service;
}

