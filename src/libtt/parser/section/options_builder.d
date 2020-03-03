// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.options_builder;

@safe:

import std.conv : ConvException;

import libtt.exception : BooleanParseException, BuilderException, EmptyValueFoundWhileParsingException,
    LineNotValidWhileParsingException, ParserIsStillParsingException;
import libtt.parser.line : KeyValueParser, MultilineValueParser;
import libtt.parser.section.section_builder : SectionBuilder;

abstract class OptionsBuilder : SectionBuilder
{
public:
    this()
    {
        valuesParser = new MultilineValueParser();
    }

    override void parseLine(in string line)
    {
        if (line == "")
        {
            return;
        }

        if (parseMultilineValue(line))
        {
            return;
        }

        string key, value;
        auto keyValueParser = new KeyValueParser(line);
        if (keyValueParser.lineValid())
        {
            key = keyValueParser.key;
            value = keyValueParser.value;
            setAttributeForKey(key, value);
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
    bool parseMultilineValue(in string line)
    {
        try
        {
            return tryParseMultilineValue(line);
        }
        catch (EmptyValueFoundWhileParsingException e)
        {
            throw new BuilderException(valuesParser.key ~ " is empty in section [options]");
        }
    }

    bool tryParseMultilineValue(in string line)
    {
        if (valuesParser.isParsing())
        {
            valuesParser.parseLine(line);
            checkParserHasFinished();
            return true;
        }

        if (valuesParser.startParsing(line))
        {
            checkParserHasFinished();
            return true;
        }

        return false;
    }

    void checkParserHasFinished()
    {
        if (!valuesParser.isParsing())
        {
            saveValuesOfParser(valuesParser);
            valuesParser.reset();
        }
    }

    void setAttributeForKey(string key, string value)
    {
        const auto sectionErrMsg = " in section [options]";
        try
        {
            trySetAttributeForKey(key, value);
        }
        catch (BooleanParseException e)
        {
            const auto msg = e.msg ~ ` while parsing key "` ~ key ~ `"`;
            throw new BuilderException(msg);
        }
        catch (ConvException e)
        {
            throw new BuilderException(key ~ " value is not valid" ~ sectionErrMsg);
        }
        catch (LineNotValidWhileParsingException e)
        {
            throw new BuilderException(e.msg ~ sectionErrMsg);
        }
    }

protected:
    abstract void saveValuesOfParser(ref MultilineValueParser parser);
    abstract void trySetAttributeForKey(string key, string value);
private:
    MultilineValueParser valuesParser;
}
