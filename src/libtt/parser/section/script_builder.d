// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.script_builder;

@safe:

import libtt.dirs : dirs;
import libtt.parser.section.section_builder : SectionBuilder;
import libtt.parser.line : KeyValueParser, MultilineCodeParser;
import libtt.parser.utils : setFailsIfNotEmpty;
import libtt.data : Environment, Script;

class ScriptBuilder : SectionBuilder
{
public:
    this(ref Script script, ref Environment environment)
    {
        this.script = script;
        this.environment = environment;
    }

    @system unittest
    {
        Script s;
        Environment e = new Environment();
        auto builder = new ScriptBuilder(s, e);
        import std.stdio : File;

        auto file = File("src/libtt/test/script_section", "r");

        auto range = file.byLineCopy();
        foreach (line; range)
        {
            builder.parseLine(line);
        }
        builder.endParsing();
    }

    override void parseLine(in string line)
    {
        if (executeParser.isParsing())
        {
            // Continue parsing execute parameter
            executeParser.parseLine(line);
            if (!executeParser.isParsing())
            {
                setFailsIfNotEmpty(&execute, executeParser.code);
            }
            return;
        }

        if (line == "")
        {
            return;
        }

        if (executeParser.startParsing(line))
        {
            return;
        }

        string key, value;
        auto keyValueParser = new KeyValueParser(line);
        if (keyValueParser.lineValid())
        {
            key = keyValueParser.key;
            value = keyValueParser.value;
            auto param = getParamByKey(key);
            setFailsIfNotEmpty(param, value);
            return;
        }

        throw new Exception(`Line "` ~ line ~ `" is not valid`);
    }

    override void endParsing()
    {
        if (execute == "")
        {
            // TODO: should this be caught by a ServiceDirector class?
            throw new Exception("");
        }
        setShebangPerType();

        script = new Script(execute, shebang, environment);

        if (user != "")
        {
            script.user = user;
        }

        if (group != "")
        {
            script.group = group;
        }
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        import std.exception : assertThrown;

        assertThrown!Exception(builder.endParsing());
    }

protected:
    // Only used for unittest
    this()
    {
    }

    string* getParamByKey(string key)
    {
        switch (key)
        {
        case "build":
            return &type;
        case "execute":
            return &execute;
        case "group":
            return &group;
        case "shebang":
            return &shebang;
        case "user":
            return &user;
        default:
            auto errorMessage = `Camp named "` ~ key ~ `" is not allowed.`;
            throw new Exception(errorMessage);
        }
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        builder.type = "oneshot";
        auto param = builder.getParamByKey("build");
        assert(*param == "oneshot");
        *param = "longrun";
        assert(builder.type == "longrun");
        builder.execute = "echo $MYCODE";
        assert(*builder.getParamByKey("execute") == builder.execute);
        builder.group = "nginx";
        assert(*builder.getParamByKey("group") == builder.group);
        builder.shebang = "!#/usr/bin/bash";
        assert(*builder.getParamByKey("shebang") == builder.shebang);
        builder.user = "nginx";
        assert(*builder.getParamByKey("user") == builder.user);
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        import std.exception : assertThrown;

        assertThrown!Exception(builder.getParamByKey("foo"));
    }

    void setShebangPerType()
    {
        switch (type)
        {
            // TODO: add "path" type
        case "auto":
            goto case;
        case "execline":
            auto newShebang = "#!" ~ dirs.execlinePrefix ~ "execlineb";
            setFailsIfNotEmpty(&shebang, newShebang);
            break;
        case "bash":
            auto newShebang = "#!" ~ dirs.bin ~ "bash";
            setFailsIfNotEmpty(&shebang, newShebang);
            break;
        default:
            auto errorMessage = `Type "` ~ type ~ `" is not allowed.`;
            throw new Exception(errorMessage);
        }
    }

    unittest
    {
        dirs.bin = "/usr/bin/";
        dirs.execlinePrefix = dirs.bin;

        auto types = ["auto", "execline", "bash"];
        string[string] executablePerType;
        executablePerType["execline"] = "execlineb";
        executablePerType["auto"] = executablePerType["execline"];
        executablePerType["bash"] = "bash";
        foreach (type; types)
        {
            auto builder = new ScriptBuilder();
            builder.type = type;
            builder.setShebangPerType();

            auto expectedShebang = "#!" ~ dirs.bin ~ executablePerType[type];
            auto msg = "`" ~ expectedShebang ~ "` was expected but instead `"
                ~ builder.shebang ~ "` was found";
            assert(expectedShebang == builder.shebang, msg);
        }
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        builder.type = "";
        import std.exception : assertThrown;

        assertThrown!Exception(builder.setShebangPerType());
    }

    Script script;
    Environment environment;
    MultilineCodeParser executeParser = new MultilineCodeParser();

    string type;
    string shebang;
    string user;
    string group;
    string execute;
}
