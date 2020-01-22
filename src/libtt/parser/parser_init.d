// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.parser_;

import std.container : DList;
import std.range : drop;
import std.stdio : File;

import libtt.parser.key_value_parser : KeyValueParser;
import libtt.parser.section_line_parser : SectionLineParser;

class ParserInit
{
public:
    @property DList!string service() { return m_service; }
    @property string type() { return m_type; }

    this(string path)
    {
        this.path = path;
        File file = openFile(path);
        m_service = generateListFrom(file);
        m_type = getTypeFromService(service);
    }

    unittest
    {
        auto parser = new ParserInit("src/libtt/test/mainSection");
        assert(parser.type == "oneshot");
    }

    unittest
    {
        import std.exception : assertThrown;
        assertThrown!Exception(new ParserInit("src/libtt/test/noSection"));
    }

private:
    File openFile(string path)
    {
        return File(path, "r");
    }

    DList!string generateListFrom(File file)
    {
        auto list = DList!string();
        foreach (line ; file.byLine())
        {
            list.insertBack(cast(string)line);
        }
        return list;
    }

    string getTypeFromService(DList!string service)
    {
        auto mainSectionIndex = getMainSectionIndex(service);
        foreach (line; service[].drop(mainSectionIndex))
        {
            auto keyValueParser = new KeyValueParser(line);
            if (keyValueParser.lineValid && keyValueParser.key == "type")
            {
                return keyValueParser.value;
            }
        }

        auto errorMessage = "No type data could be found for service "
                ~ path;
        throw new Exception(errorMessage);
    }

    int getMainSectionIndex(DList!string service)
    {
        ushort i = 0;
        foreach (line ; service[])
        {
            auto sectionLineParser = new SectionLineParser(line);
            auto currentSection = sectionLineParser.getSectionOrDefault("");
            if (currentSection == "main")
            {
                return i;
            }
            i++;
        }

        auto errorMessage = "No main section could be found for service "
                ~ path;
        throw new Exception(errorMessage);
    }

    File file;
    string path;
    DList!string m_service;
    string m_type;
}

