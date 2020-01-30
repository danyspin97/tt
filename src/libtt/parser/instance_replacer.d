// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.instance_replacer;

import std.string : tr;

import libtt.services.service : Service;
import libtt.services.script : Script;

enum InstanceToken = "@I";

abstract class InstanceReplacer
{
protected:
    void replaceTokenInServiceData(Service service)
    {
        replaceTokenInString(service.name);
        replaceTokenInString(service.polishName);
        replaceTokenInString(service.description);
    }

    void replaceTokenInScript(Script script)
    {
        script.execute = replaceTokenInString(script.execute());
    }

    string replaceTokenInString(string s)
    {
        return tr(s, InstanceToken, instanceName);
    }

    string instanceName;
}
