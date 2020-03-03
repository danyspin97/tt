// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.oneshot_options_builder;

@safe:
nothrow:

import libtt.parser.section.utils : attributeNotFound;
import libtt.parser.section.options_builder : OptionsBuilder;
import libtt.parser.line : MultilineValueParser;
import libtt.parser.utils : parseBoolean;
import libtt.data : OneshotOptions;

class OneshotOptionsBuilder : OptionsBuilder
{
public:
    this(OneshotOptions oneshotOptions)
    {
        this.oneshotOptions = oneshotOptions;
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
