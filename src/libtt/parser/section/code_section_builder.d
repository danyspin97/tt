// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.code_section_builder;

import libtt.parser.utils : setFailsIfNotEmpty;
import libtt.parser.line : KeyValueParser, MultilineCodeParser;
import libtt.parser.section.section_builder : SectionBuilder;

@safe:
nothrow:

abstract class CodeSectionBuilder : SectionBuilder
{
    override void parseLine(in string line)
    {
        if (codeParser.isParsing())
        {
            // Continue parsing execute parameter
            codeParser.parseLine(line);
            if (!codeParser.isParsing())
            {
                auto param = getCodeAttributeForKey(codeParser.key);
                setFailsIfNotEmpty(param, codeParser.code);
                codeParser.reset();
            }
            return;
        }

        if (line == "")
        {
            return;
        }

        if (codeParser.startParsing(line))
        {
            return;
        }

        string key, value;
        auto keyValueParser = new KeyValueParser(line);
        if (keyValueParser.lineValid())
        {
            key = keyValueParser.key;
            value = keyValueParser.value;
            auto param = getAttributeForKey(key);
            setFailsIfNotEmpty(param, value);
            return;
        }

        throw new Exception(`Line "` ~ line ~ `" is not valid`);
    }

protected:
    string* getCodeAttributeForKey(string key);
    string* getAttributeForKey(string key);
private:
    MultilineCodeParser codeParser = new MultilineCodeParser();
}
