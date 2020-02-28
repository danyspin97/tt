// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.logger_script_builder;

@safe:

import libtt.data : Environment, LoggerScript;
import libtt.dirs : dirs;
import libtt.parser.section.script_builder : ScriptBuilder;

class LoggerScriptBuilder : ScriptBuilder
{
public:
    this(LoggerScript* loggerScript, ref Environment environment, string serviceName)
    {
        this.loggerScript = loggerScript;
        assert(environment);
        this.environment = environment;
        this.serviceName = serviceName;
    }

    @system unittest
    {
        LoggerScript s;
        Environment e = new Environment();
        auto builder = new LoggerScriptBuilder(&s, e, "foo");
        testBuilderWithFile(builder, "src/libtt/test/logger_script_no_execute");
        assert(s.serviceToLog == "foo");
    }

    override void endParsing()
    {
        setDefaultForOptionalValues();
        setShebangPerType();

        *loggerScript = new LoggerScript(execute, shebang, environment, serviceName);

        setValuesForScript();
    }

    override string* getAttributeForKey(in string key)
    {
        switch (key)
        {
        case "destination":
            return &destination;
        case "maxsize":
            return &maxsize;
        default:
            return super.getAttributeForKey(key);
        }
    }

    unittest
    {
        auto builder = new LoggerScriptBuilder();
        builder.destination = "/var/log/newdestination";
        assert(*builder.getAttributeForKey("destination") == builder.destination);
        builder.maxsize = "15000";
        assert(*builder.getAttributeForKey("maxsize") == builder.maxsize);
    }

private:
    // only used for unittest
    this()
    {

    }

    void setDefaultForOptionalValues()
    {
        if (type == "")
        {
            type = "auto";
        }

        if (maxsize == "")
        {
            maxsize = "8000000"; // 8MB
        }

        if (destination == "")
        {
            destination = getDefaultDestination();
        }

        if (execute == "")
        {
            execute = getDefaultExecute();
        }
    }

    void setValuesForScript()
    {
        if (user != "")
        {
            loggerScript.user = user;
        }

        if (group != "")
        {
            loggerScript.group = group;
        }
    }

    string getDefaultExecute()
    {
        return "s6-setuidgid log exec -c s6-log -- s" ~ maxsize ~ " n20 t " ~ destination;
    }

    string getDefaultDestination()
    {
        return dirs.log ~ "/" ~ serviceName;
    }

    LoggerScript* loggerScript;

    string destination;
    string maxsize;
    string serviceName;
}
