// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.oneshot_instance_parser;

import libtt.parser.instance_replacer : InstanceReplacer, InstanceToken;
import libtt.services.oneshot : Oneshot;
import libtt.services.oneshot_options : OneshotOptions;

class OneshotInstanceReplacer : InstanceReplacer
{
public:
    this (string instanceName, ref Oneshot oneshot)
    {
        this.instanceName = instanceName;
        replaceTokenInServiceData(oneshot);
        replaceTokenInScript(oneshot.start);
        if (oneshot.stop)
        {
            replaceTokenInScript(oneshot.stop);
        }

        auto oneshotOptions = cast(OneshotOptions) oneshot.options;
        replaceTokenInOptions(oneshotOptions);
    }
    
    void replaceTokenInOptions(OneshotOptions options)
    {
        // TODO: Implement this
    }
}

