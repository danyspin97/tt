// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.longrun_instance_replacer;

import libtt.parser.instance_replacer : InstanceReplacer, InstanceToken;
import libtt.services.logger_script : LoggerScript;
import libtt.services.longrun : Longrun;
import libtt.services.longrun_options : LongrunOptions;

class LongrunInstanceReplacer : InstanceReplacer
{
public:
    this (string instanceName, ref Longrun longrun)
    {
        this.instanceName = instanceName;
        replaceTokenInServiceData(longrun);
        replaceTokenInScript(longrun.run);
        if (longrun.finish)
        {
            replaceTokenInScript(longrun.finish);
        }

        replaceTokenInLoggerScript(longrun.logger);

        auto longrunOptions = cast(LongrunOptions) longrun.options;
        replaceTokenInOptions(longrunOptions);
    }

    void replaceTokenInLoggerScript(LoggerScript script)
    {
        replaceTokenInScript(script);
    }

    void replaceTokenInOptions(LongrunOptions options)
    {
        // TODO: Implement this
    }
}

