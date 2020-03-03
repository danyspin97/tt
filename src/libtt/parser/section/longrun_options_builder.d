// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.longrun_options_builder;

@safe:
nothrow:

import std.conv : to;

import libtt.data : LongrunOptions;
import libtt.exception : BuilderException;
import libtt.parser.line : MultilineValueParser;
import libtt.parser.section.options_builder : OptionsBuilder;
import libtt.signal : Signal, parseSignalFromString;
import libtt.parser.utils : parseBoolean;
import libtt.parser.section.utils : attributeNotFound, testBuilderWithFile;

class LongrunOptionsBuilder : OptionsBuilder
{
public:
    this(LongrunOptions longrunOptions)
    {
        super();
        this.longrunOptions = longrunOptions;
    }

    @system unittest
    {
        LongrunOptions o = new LongrunOptions();
        auto builder = new LongrunOptionsBuilder(o);
        builder.testBuilderWithFile("src/libtt/test/longrun_options_section");

        assert(o.dependencies == ["foo", "bar"]);
        assert(o.notify == 3);
        assert(o.timeoutFinish == 5000);
        assert(o.timeoutKill == 10);
        assert(o.downSignal == Signal.SIGKILL);
        assert(o.maxDeath == 250);
        assert(o.writeMessage == false);
        assert(o.optional == true);
    }

    @system unittest
    {
        LongrunOptions o = new LongrunOptions();
        auto builder = new LongrunOptionsBuilder(o);
        import std.exception : assertThrown;

        assertThrown!BuilderException(
                builder.testBuilderWithFile("src/libtt/test/invalid_int_value"));

        assertThrown!BuilderException(
                builder.testBuilderWithFile("src/libtt/test/invalid"));

        assertThrown!BuilderException(
                builder.testBuilderWithFile("src/libtt/test/invalid_multiline_value"));
    }

    override void saveValuesOfParser(ref MultilineValueParser parser)
    {
        switch (parser.key)
        {
        case "depends":
            longrunOptions.dependencies = parser.values;
            break;
        default:
            attributeNotFound(parser.key, "options");
        }
    }

    override void trySetAttributeForKey(in string key, in string value)
    {
        switch (key)
        {
        case "down_signal":
            longrunOptions.downSignal = parseSignalFromString(value);
            break;
        case "maxdeath":
            longrunOptions.maxDeath = to!ushort(value);
            break;
        case "notify":
            longrunOptions.notify = to!uint(value);
            break;
        case "optional":
            longrunOptions.optional = parseBoolean(value);
            break;
        case "timeout_finish":
            longrunOptions.timeoutFinish = to!uint(value);
            break;
        case "timeout_kill":
            longrunOptions.timeoutKill = to!uint(value);
            break;
        case "write_message":
            longrunOptions.writeMessage = parseBoolean(value);
            break;
        default:
            attributeNotFound(key, "options");
        }
    }

private:
    LongrunOptions longrunOptions;
}
