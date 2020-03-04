// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.main_section_builder;

@safe:

import libtt.parser.line : KeyValueParser;
import libtt.parser.section.main_section : MainSection;
import libtt.parser.section.section_builder : SectionBuilder;
import libtt.parser.section.utils : testBuilderWithFile;

class MainSectionBuilder : SectionBuilder
{
public:
    this(MainSection* mainSection)
    {
        this.mainSection = mainSection;
    }

    @system unittest
    {
        auto m = new MainSection();
        auto builder = new MainSectionBuilder(m);
        builder.testBuilderWithFile("src/libtt/test/main_section");

        assert(m.name == "nginx");
        assert(m.polishName == "Nginx Service");
        assert(m.description == "Run nginx server");
        assert(m.type == "longrun");
    }

    override void parseLine(in string line)
    {
        if (line == "")
        {
            return;
        }

        const auto keyValueParser = new KeyValueParser(line, true);
        const auto key = keyValueParser.key;
        const auto value = keyValueParser.value;

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
    MainSection* mainSection;
}
