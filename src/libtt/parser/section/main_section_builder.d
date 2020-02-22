// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.main_section_builder;

@safe:

import libtt.parser.line : KeyValueParser;
import libtt.parser.section.section_builder : SectionBuilder;
import libtt.parser.section.main_section : MainSection;

class MainSectionBuilder : SectionBuilder
{
public:
    this(MainSection mainSection)
    {
        this.mainSection = mainSection;
    }

    override void parseLine(in string line)
    {
        if (line == "")
        {
            return;
        }

        string key, value;
        auto keyValueParser = new KeyValueParser(line);
        if (keyValueParser.lineValid())
        {
            key = keyValueParser.key;
            value = keyValueParser.value;
        }

        // TODO: Handle multiline values here

        fillServiceDataWith(key, value);
    }

    void fillServiceDataWith(in string key, in string value)
    {
        switch (key)
        {
        case "name":
            mainSection.name = value;
            break;
        case "description":
            mainSection.description = value;
            break;
        case "polish_name":
            mainSection.polishName = value;
            break;
        case "type":
            mainSection.type = value;
            break;
        default:
            auto errorMessage = `Camp named "` ~ key ~ `" is not allowed.`;
            throw new Exception(errorMessage);
        }
    }

    override void endParsing()
    {

    }

private:
    MainSection mainSection;
}
