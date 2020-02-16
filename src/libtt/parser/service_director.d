// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service_director;

import std.ascii : newline;
import std.container : DList;
import std.format : FormatException;
import std.typecons : Tuple, tuple;

import libtt.exception : DummyBuilderException;
import libtt.parser.dummy_builder : DummyBuilder;
import libtt.parser.key_value_parser : KeyValueParser;
import libtt.parser.main_section : MainSection;
import libtt.parser.section_line_parser : SectionLineParser;
import libtt.parser.section_builder : SectionBuilder;
import libtt.services.service : Service;

abstract class ServiceDirector
{
public:
    @property Service service() { return m_service; }

    this()
    {
    }

    Service parseAndGetService(ref DList!string serviceLines, string path)
    {
        this.serviceLines = serviceLines;
        parseSections();
        return instanceService(path);
    }

protected:
    abstract Service instanceService(string path);
    abstract SectionBuilder getBuilderForSection(string section);

private:
    void parseSections()
    {
        try
        {
            tryParseSections();
        }
        catch (DummyBuilderException e)
        {
            throw new Exception("No line is allowed before a section");
        }
    }

    void tryParseSections()
    {
        SectionBuilder currentBuilder = new DummyBuilder();
        foreach (line ; serviceLines[])
        {
            auto sectionLineParser = new SectionLineParser(line);
            if (sectionLineParser.lineValid())
            {
                currentBuilder.endParsing();
                currentBuilder =
                        getBuilderForSection(sectionLineParser.section);
                continue;
            }

            currentBuilder.parseLine(line);
        }
        currentBuilder.endParsing();
    }

    Service m_service;
    DList!string serviceLines;
}

