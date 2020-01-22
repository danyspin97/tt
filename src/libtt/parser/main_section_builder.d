// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.main_section_builder;

import libtt.parser.key_value_parser : KeyValueParser;
import libtt.parser.main_section : MainSection;
import libtt.parser.section_builder : SectionBuilder;

class MainSectionBuilder : SectionBuilder
{
public:
    this(MainSection mainSection)
    {
        this.mainSection = mainSection;
    }

    override void parseLine(string line)
    {
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

    void fillServiceDataWith(string key, string value)
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

private:
    MainSection mainSection;
}
