// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.logger_script_builder;

@safe:

import libtt.data : Environment, LoggerScript, Script;
import libtt.define : DefaultLogGroup, DefaultLogUser;
import libtt.dirs : dirs;
import libtt.format : formatAssertMessage;
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
        builder.testBuilderWithFile("src/libtt/test/logger_script");
        assert(s.serviceToLog == "foo");
        import std.ascii : newline;

        auto expectedExecute = "s6-log /var/log/tt/foo";

        auto msg = formatAssertMessage("LoggerScript execute script", expectedExecute, s.execute);
        assert(s.execute == expectedExecute, msg);

        msg = formatAssertMessage("LoggerScript user", s.user, "tt_log");
        assert(s.user == "tt_log", msg);
        msg = formatAssertMessage("LoggerScript group", s.group, "tt_log");
        assert(s.group == "tt_log", msg);
    }

    @system unittest
    {
        LoggerScript s;
        Environment e = new Environment();
        auto builder = new LoggerScriptBuilder(&s, e, "bar");
        builder.testBuilderWithFile("src/libtt/test/logger_script_no_execute");
        assert(s.serviceToLog == "bar");

        auto msg = formatAssertMessage("LoggerScript default user", s.user, DefaultLogUser);
        assert(s.user == DefaultLogUser, msg);
        msg = formatAssertMessage("LoggerScript default group", s.group, DefaultLogGroup);
        assert(s.group == DefaultLogGroup, msg);
    }

    override void endParsing()
    {
        checkParsedValues();
        setDefaultForOptionalValues();
        setShebangPerType();

        Script.Type scriptType = getParsedType();
        *loggerScript = new LoggerScript(scriptType, execute, shebang, environment, serviceName, user, group);
    }

protected:
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

    void checkParsedValues()
    {
        auto executeSet = execute != "";
        auto typeSet = type != "";
        // A XOR B
        if (executeSet != typeSet)
        {
            auto msg = "Both execute and type attributes needs to set";
            throw new Exception(msg);
        }

        auto destinationSet = destination != "";
        auto maxsizeSet = maxsize != "";
        if ((executeSet || typeSet) && (destinationSet || maxsizeSet))
        {
            auto msg = "destination and maxsize attributes cannot be set when setting execute";
            throw new Exception(msg);
        }
    }

    unittest
    {
        auto builder = new LoggerScriptBuilder();
        builder.execute = "s6-log";
        import std.exception : assertThrown;

        assertThrown!Exception(builder.checkParsedValues);

        builder.execute = "";
        builder.type = "auto";
        assertThrown!Exception(builder.checkParsedValues);
    }

    unittest
    {
        auto builder = new LoggerScriptBuilder();
        builder.execute = "s6-log";
        builder.type = "auto";
        builder.maxsize = "500";
        import std.exception : assertThrown;

        assertThrown!Exception(builder.checkParsedValues);

        builder.maxsize = "";
        builder.destination = "/var/log/foo";
        assertThrown!Exception(builder.checkParsedValues);
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

        if (user == "")
        {
            user = DefaultLogUser;
        }

        if (group == "")
        {
            group = DefaultLogGroup;
        }
    }

    string getDefaultExecute()
    {
        return "exec -c s6-log -- s" ~ maxsize ~ " n20 t " ~ destination;
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
