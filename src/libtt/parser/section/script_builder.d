// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.script_builder;

@safe:

import libtt.dirs : dirs;
import libtt.parser.section.section_builder : SectionBuilder;
import libtt.parser.section.code_section_builder : CodeSectionBuilder;
import libtt.parser.line : KeyValueParser, MultilineCodeParser;
import libtt.parser.utils : setFailsIfNotEmpty;
import libtt.data : Environment, Script;

class ScriptBuilder : CodeSectionBuilder
{
public:
    this(Script* script, ref Environment environment)
    {
        this.script = script;
        assert(environment);
        this.environment = environment;
    }

    @system unittest
    {
        Script s;
        Environment e = new Environment();
        auto builder = new ScriptBuilder(&s, e);
        builder.testBuilderWithFile("src/libtt/test/script_section");

        assert(s.user == "dbus");
        assert(s.group == "dbus");
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        import std.exception : assertThrown;

        assertThrown!Exception(builder.parseLine("invalid"));
    }

    void endParsing()
    {
        if (execute == "")
        {
            // TODO: should this be caught by a ServiceDirector class?
            throw new Exception("");
        }

        Script.Type scriptType = getParsedType();
        *script = new Script(scriptType, execute, environment);

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

    override string* getAttributeForKey(string key)
    {
        switch (key)
        {
        case "build":
            return &type;
        case "execute":
            return &execute;
        case "group":
            return &group;
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
        auto param = builder.getAttributeForKey("build");
        assert(*param == "oneshot");
        *param = "longrun";
        assert(builder.type == "longrun");
        builder.execute = "echo $MYCODE";
        assert(*builder.getAttributeForKey("execute") == builder.execute);
        builder.group = "nginx";
        assert(*builder.getAttributeForKey("group") == builder.group);
        builder.user = "nginx";
        assert(*builder.getAttributeForKey("user") == builder.user);
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        import std.exception : assertThrown;

        assertThrown!Exception(builder.getAttributeForKey("foo"));
    }

    override string* getCodeAttributeForKey(string key)
    {
        switch (key)
        {
        case "execute":
            return &execute;
        default:
            auto errorMessage = `Camp named "` ~ key ~ `" is not allowed.`;
            throw new Exception(errorMessage);
        }
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        builder.execute = "echo";
        assert(*builder.getCodeAttributeForKey("execute") == builder.execute);
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        import std.exception : assertThrown;

        assertThrown!Exception(builder.getCodeAttributeForKey("foo"));
    }

    Script.Type getParsedType()
    {
        switch (type)
        {
            // TODO: add "path" type
        case "auto":
            goto case;
        case "execline":
            return Script.Type.Execline;
        case "bash":
            return Script.Type.Bash;
        default:
            auto errorMessage = `Type "` ~ type ~ `" is not allowed.`;
            throw new Exception(errorMessage);
        }
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        builder.type = "auto";
        assert(builder.getParsedType() == Script.Type.Execline);
        builder.type = "execline";
        assert(builder.getParsedType() == Script.Type.Execline);
        builder.type = "bash";
        assert(builder.getParsedType() == Script.Type.Bash);
    }

    unittest
    {
        auto builder = new ScriptBuilder();
        import std.exception : assertThrown;

        assertThrown!Exception(builder.getParsedType());
    }

    Script* script;
    Environment environment;
    MultilineCodeParser executeParser = new MultilineCodeParser();

    string type;
    string user;
    string group;
    string execute;
}
