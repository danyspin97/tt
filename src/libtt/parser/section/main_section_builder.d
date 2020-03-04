// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.main_section_builder;

@safe:

import libtt.exception : BuilderException, LineNotValidWhileParsingException;
import libtt.parser.line : KeyValueParser;
import libtt.parser.section.main_section : MainSection;
import libtt.parser.section.section_builder : SectionBuilder;
import libtt.parser.section.utils : attributeNotFound, testBuilderWithFile;

class MainSectionBuilder : SectionBuilder
{
public:
    this(MainSection* mainSection)
    {
        this.mainSection = mainSection;
    }

    @system unittest
    {
        MainSection m;
        auto builder = new MainSectionBuilder(&m);
        builder.testBuilderWithFile("src/libtt/test/main_section");

        assert(m.name == "nginx");
        assert(m.polishName == "Nginx Service");
        assert(m.description == "Run nginx server");
        assert(m.type == "longrun");
    }

    @system unittest
    {
        auto m = new MainSection();
        auto builder = new MainSectionBuilder(m);
        import std.exception : assertThrown;
        import libtt.exception : BuilderException;

        const auto testFiles = [
            "invalid", "invalid_quotes", "unclosed_quotes",
            "unknown_key",
        ];
        static foreach (test; testFiles)
        {
            assertThrown!BuilderException(builder.testBuilderWithFile("src/libtt/test/" ~ test));
        }
    }

    override void parseLine(in string line)
    {
        try
        {
            tryParseLine(line);
        }
        catch (LineNotValidWhileParsingException e)
        {
            throw new BuilderException(`Line "` ~ line ~ `" is not valid`);
        }
    }

    override void endParsing()
    {

    }

private:
    void tryParseLine(in string line)
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
            attributeNotFound(key, "main");
        }
    }

    MainSection* mainSection;
}
