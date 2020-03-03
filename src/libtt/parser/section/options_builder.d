// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.options_builder;

@safe:

import std.conv : ConvException;

import libtt.exception : BooleanParseException, BuilderException,
    LineNotValidWhileParsingException, ParserIsStillParsingException;
import libtt.parser.line : KeyValueParser, MultilineValueParser;
import libtt.parser.section.section_builder : SectionBuilder;

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
            checkParserHasFinished();
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
            checkParserHasFinished();
            return;
        }

        throw new BuilderException(`Line "` ~ line ~ `" is not valid`);
    }

    override void endParsing()
    {
        if (valuesParser.isParsing())
        {
            throw new ParserIsStillParsingException("");
        }
    }

private:
    void checkParserHasFinished()
    {
        if (!valuesParser.isParsing())
        {
            saveValuesOfParser(valuesParser);
            valuesParser.reset();
        }
    }

protected:
    abstract void saveValuesOfParser(ref MultilineValueParser parser);
    abstract void setParamByKey(string key, string value);
private:
    MultilineValueParser valuesParser = new MultilineValueParser();
}
