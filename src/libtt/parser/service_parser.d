// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service_parser;

import std.container : DList;
import std.range : drop;
import std.stdio : File;

import libtt.parser.key_value_parser : KeyValueParser;
import libtt.parser.parser_factory : ParserFactory;
import libtt.parser.section_line_parser : SectionLineParser;
import libtt.services.service : Service;

class ServiceParser
{
public:
    @property Service service() { return m_service; }

    this(string path)
    {
        this.path = path;
        auto file = openFile(path);
        auto serviceLines = generateListFrom(file);
        auto type = getTypeFromService(serviceLines);
        auto director = ParserFactory.getDirectorPerType(type);
        m_service = director.parseAndGetService(serviceLines, path);
    }

    unittest
    {
        import std.exception : assertThrown;
        assertThrown!Exception(new ServiceParser("src/libtt/test/noSection"));
    }

protected:
    DList!string generateListFrom(File file)
    {
        auto list = DList!string();
        foreach (line ; file.byLine())
        {
            list.insertBack(line.idup);
        }
        return list;
    }

private:
    File openFile(string path)
    {
        return File(path, "r");
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

    Service m_service;
    string path;
}

