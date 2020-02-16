// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.service_director;

import std.ascii : newline;
import std.container : DList;
import std.format : FormatException;
import std.typecons : Tuple, tuple;

import libtt.exception : DummyBuilderException;
import libtt.parser.line : KeyValueParser, SectionLineParser;
import libtt.parser.section : DummyBuilder, MainSection, SectionBuilder;
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

