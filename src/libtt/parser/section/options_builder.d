// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.options_builder;

@safe:

import libtt.parser.line : KeyValueParser, MultilineValueParser;
import libtt.parser.section.section_builder : SectionBuilder;
import libtt.data : OneshotOptions;

abstract class OptionsBuilder : SectionBuilder
{
public:
    override void parseLine(in string line)
    {
        if (line == "")
        {
            return;
        }

        if (valuesParser.isParsing())
        {
            valuesParser.parseLine(line);
            if (!valuesParser.isParsing())
            {
                saveValuesOfParser(valuesParser);
                valuesParser.reset();
            }
            return;
        }

        string key, value;
        auto keyValueParser = new KeyValueParser(line);
        if (keyValueParser.lineValid())
        {
            key = keyValueParser.key;
            value = keyValueParser.value;
            setParamByKey(key, value);
            return;
        }

        if (valuesParser.startParsing(line))
        {
            if (!valuesParser.isParsing())
            {
                saveValuesOfParser(valuesParser);
                valuesParser.reset();
            }
            return;
        }

        throw new Exception(`Line "` ~ line ~ `" is not valid`);
    }

    override void endParsing()
    {
        if (valuesParser.isParsing())
        {
            // TODO: should this be caught by a ServiceDirector class?
            throw new Exception("");
        }
    }

protected:
    abstract void saveValuesOfParser(ref MultilineValueParser parser);
    abstract void setParamByKey(string key, string value);
private:
    MultilineValueParser valuesParser = new MultilineValueParser();
}
