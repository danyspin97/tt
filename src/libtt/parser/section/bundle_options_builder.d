// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.bundle_options_builder;

@safe:
nothrow:

import libtt.data : BundleOptions;
import libtt.exception : BuilderException;
import libtt.parser.line : MultilineValueParser;
import libtt.parser.section.options_builder : OptionsBuilder;
import libtt.parser.section.utils : attributeNotFound, isEmptyLine, testBuilderWithFile;

class BundleOptionsBuilder : OptionsBuilder
{
public:
    this(ref BundleOptions options)
    {
        this.options = options;
    }

    @system unittest
    {
        auto o = new BundleOptions;
        auto builder = new BundleOptionsBuilder(o);
        builder.testBuilderWithFile("src/libtt/test/bundle_options_section");

        assert(o.contents == ["foo", "bar"]);
    }

    override void parseLine(string line)
    {
        if (isEmptyLine(line))
        {
            return;
        }

        if (parseMultilineValue(line))
        {
            return;
        }

        throw new BuilderException("Line " ~ line ~ " is not valid in section [options]");
    }

protected:
    override void saveValuesOfParser(ref MultilineValueParser parser)
    {
        switch (parser.key)
        {
        case "contents":
            options.contents = parser.values;
            break;
        default:
            attributeNotFound(parser.key, "options");
        }
    }

    override void trySetAttributeForKey(string key, string value)
    {
        throw new BuilderException("No key is allowed");
    }

private:
    BundleOptions options;
}
