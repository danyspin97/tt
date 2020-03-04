// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.oneshot_options_builder;

@safe:
nothrow:

import libtt.data : OneshotOptions;
import libtt.parser.section.utils : attributeNotFound, testBuilderWithFile;
import libtt.parser.section.options_builder : OptionsBuilder;
import libtt.parser.line : MultilineValueParser;
import libtt.parser.utils : parseBoolean;

class OneshotOptionsBuilder : OptionsBuilder
{
public:
    this(OneshotOptions oneshotOptions)
    {
        super();
        this.oneshotOptions = oneshotOptions;
    }

    @system unittest
    {
        OneshotOptions o = new OneshotOptions();
        auto builder = new OneshotOptionsBuilder(o);
        builder.testBuilderWithFile("src/libtt/test/oneshot_options_section");

        assert(o.optional == true);
        assert(o.writeMessage == false);
        assert(o.dependencies == ["foo", "bar"]);
    }

    @system unittest
    {
        OneshotOptions o = new OneshotOptions();
        auto builder = new OneshotOptionsBuilder(o);
        import std.exception : assertThrown;
        import libtt.exception : BuilderException;

        const auto testFiles = [
            "empty_multiline_value", "invalid",
            "invalid_multiline_value", "invalid_quotes", "invalid_boolean", "unclosed_quotes",
            "unclosed_multiline_value", "unknown_key", "unknown_multiline_value"
        ];
        static foreach (test; testFiles)
        {
            assertThrown!BuilderException(builder.testBuilderWithFile("src/libtt/test/" ~ test));
        }
    }

protected:
    override void trySetAttributeForKey(in string key, in string value)
    {
        switch (key)
        {
        case "optional":
            oneshotOptions.optional = parseBoolean(value);
            break;
        case "write_message":
            oneshotOptions.writeMessage = parseBoolean(value);
            break;
        default:
            attributeNotFound(key, "options");
        }
    }

    override void saveValuesOfParser(ref MultilineValueParser parser)
    {
        switch (parser.key)
        {
        case "depends":
            oneshotOptions.dependencies = parser.values;
            break;
        default:
            attributeNotFound(parser.key, "options");
        }
    }

private:
    OneshotOptions oneshotOptions;
}
