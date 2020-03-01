// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.environment_builder;

@safe:
nothrow:

import libtt.data : Environment;
import libtt.format : formatAssertMessage;
import libtt.parser.line : KeyValueParser;
import libtt.parser.section.section_builder : SectionBuilder;

class EnvironmentBuilder : SectionBuilder
{
public:
    this(ref Environment environment)
    {
        assert(environment, "Environment needs to be initialized.");
        this.environment = environment;
    }

    @system unittest
    {
        Environment e = new Environment();
        auto builder = new EnvironmentBuilder(e);
        builder.testBuilderWithFile("src/libtt/test/environment_section");
        const auto expected = ["CMDARGS": "-d --nofork", "LOGLEVEL": "0"];
        static foreach (key, value; expected)
        {
            assert(e.get(key) == value, formatAssertMessage(key, e.get(key), value));
        }
    }

    override void parseLine(in string line)
    {
        auto keyValueParser = new KeyValueParser(line);
        if (!keyValueParser.lineValid())
        {
            throw new Exception("");
        }
        const string key = keyValueParser.key;
        const string value = keyValueParser.value;

        environment.set(key, value);
    }

    override void endParsing()
    {

    }

private:
    Environment environment;
}
