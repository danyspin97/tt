// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.service_director_test;

import std.container : DList;

import libtt.parser.section : SectionBuilder;
import libtt.parser.service.service_director : ServiceDirector;
import libtt.services.service : Service;

class ServiceDirectorTest : ServiceDirector
{
    class BuilderTest : SectionBuilder
    {
        void parseLine(string line)
        {
            timesParsed++;
        }

        void endParsing()
        {
            parsingEnded = true;
        }

        uint timesParsed;
        bool parsingEnded = false;
    }

protected:
    override Service instanceService(string path)
    {
        return null;
    }

    override SectionBuilder getBuilderForSection(string section)
    {
        builder = new BuilderTest();
        return builder;
    }

    unittest
    {
        auto director = new ServiceDirectorTest();
        auto s = DList!string("", "[foo]");
        auto linesNumber = 10;
        foreach (i; 0 .. linesNumber)
        {
            s.insertBack("");
        }

        director.parseAndGetService(s, "");
        assert(director.builder.timesParsed == linesNumber);
        assert(director.builder.parsingEnded);
    }

    unittest
    {
        auto director = new ServiceDirectorTest();
        auto s = DList!string("foo", "");
        import std.exception : assertThrown;
        assertThrown!Exception(director.parseAndGetService(s, ""));
    }

    BuilderTest builder;
}

