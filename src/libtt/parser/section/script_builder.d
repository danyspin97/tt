// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.script_builder;

import libtt.dirs : dirs;
import libtt.parser.section.section_builder : SectionBuilder;
import libtt.parser.line : KeyValueParser, MultilineCodeParser;
import libtt.parser.utils : setFailsIfNotEmpty;
import libtt.services.script : Script;
import libtt.services.environment : Environment;

class ScriptBuilder : SectionBuilder
{
public:
    this(ref Script script, ref Environment environment)
    {
        this.script = script;
        this.environment = environment;
    }

    override void parseLine(string line)
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

        if (executeParser.startParsing(line))
        {
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
        setShebangPerType(type);

        script = new Script(
            execute,
            shebang,
            environment
        );

        if (user != "")
        {
            script.user = user;
        }

        if (group != "")
        {
            script.group = group;
        }
    }

protected:
    // Only used for unittest
    this() {}

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
    }

    void setShebangPerType(string type)
    {
        switch (type)
        {
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

    Script script;
    Environment environment;
    MultilineCodeParser executeParser = new MultilineCodeParser();

    string type = "auto";
    string shebang;
    string user;
    string group;
    string execute;
}
