// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.logger_script_builder;

import libtt.parser.section.script_builder : ScriptBuilder;
import libtt.data : Environment, LoggerScript;

class LoggerScriptBuilder : ScriptBuilder
{
public:
    this(LoggerScript loggerScript, ref Environment environment)
    {
        this.loggerScript = loggerScript;
        this.environment = environment;
    }

    override void endParsing()
    {
        setShebangPerType(type);

        if (execute == "")
        {
            script = new LoggerScript(
                shebang,
                environment
            );
        }
        else
        {
            script = new LoggerScript(
                executeParser.code,
                shebang,
                environment
            );
        }

        if (user != "")
        {
            script.user = user;
        }

        if (group != "")
        {
            script.group = group;
        }
    }

    override string* getParamByKey(string key)
    {
        switch (key)
        {
            case "destination":
                return &destination;
            case "maxsize":
                return &maxsize;
            default:
                return super.getParamByKey(key);
        }
    }

private:
    LoggerScript loggerScript;

    string destination;
    string maxsize;
}

