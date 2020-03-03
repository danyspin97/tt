// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.environment_builder;

@safe:
nothrow:

import std.ascii : isAlphaNum, isDigit;

import libtt.data : Environment;
import libtt.exception : LineNotValidWhileParsingException, SectionNotValidException;
import libtt.format : formatAssertMessage;
import libtt.parser.line : KeyValueParser;
import libtt.parser.section.section_builder : SectionBuilder;
import libtt.parser.section.utils : testBuilderWithFile;

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
        const auto expected = ["CMDARGS" : "-d --nofork", "LOGLEVEL" : "0"];
        static foreach (key, value; expected)
        {
            assert(e.get(key) == value, formatAssertMessage(key, e.get(key), value));
        }
    }

    @system unittest
    {
        Environment e = new Environment();
        auto builder = new EnvironmentBuilder(e);
        import std.exception : assertThrown;

        assertThrown!SectionNotValidException(builder.testBuilderWithFile("src/libtt/test/invalid"));
        assertThrown!SectionNotValidException(builder.testBuilderWithFile("src/libtt/test/invalid_environment"));
    }

    override void parseLine(in string line)
    {
        try
        {
            tryParseLine(line);
        }
        catch (LineNotValidWhileParsingException e)
        {
            throw new SectionNotValidException(e.msg ~ " of environment section");
        }
    }

    override void endParsing()
    {

    }

private:
    void tryParseLine(in string line)
    {
        auto keyValueParser = new KeyValueParser(line, true);
        const string key = keyValueParser.key;
        checkKeyIsValid(key);
        const string value = keyValueParser.value;

        environment.set(key, value);
    }

    static void checkKeyIsValid(in string key)
    {
        if (key[0].isDigit())
        {
            auto msg = "Key " ~ key ~ " is not valid as it cannot start with a digit";
            throw new LineNotValidWhileParsingException(msg);
        }
        foreach (c; key)
        {
            if (!c.isAlphaNum() && c != '_')
            {
                auto msg = "Key " ~ key ~ " is not valid as it contains `" ~ c ~ "` character";
                throw new LineNotValidWhileParsingException(msg);
            }
        }
    }

    unittest
    {
        import std.exception : assertNotThrown;

        assertNotThrown(checkKeyIsValid("foo"));
        assertNotThrown(checkKeyIsValid("FOO"));
        assertNotThrown(checkKeyIsValid("FOO_BAR"));
        assertNotThrown(checkKeyIsValid("_FOO"));
    }

    unittest
    {
        import std.exception : assertThrown;

        assertThrown!LineNotValidWhileParsingException(checkKeyIsValid("FOO-BAR"));
        assertThrown!LineNotValidWhileParsingException(checkKeyIsValid("1FOO"));
    }

    Environment environment;
}
