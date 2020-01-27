// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.options_builder;

import libtt.parser.key_value_parser : KeyValueParser;
import libtt.parser.multiline_value_parser : MultilineValueParser;
import libtt.parser.section_builder : SectionBuilder;
import libtt.services.oneshot_options : OneshotOptions;

abstract class OptionsBuilder : SectionBuilder
{
public:
    override void parseLine(string line)
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

