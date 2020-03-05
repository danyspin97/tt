// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.service_director_test;

@safe:

import libtt.parser.section : SectionBuilder;
import libtt.parser.service.service_director : ServiceDirector;
import libtt.data : Service;

private class ServiceDirectorTest : ServiceDirector
{
    class BuilderTest : SectionBuilder
    {
        void parseLine(in string line)
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
    override Service instanceService(in string path)
    {
        return null;
    }

    override SectionBuilder getBuilderForSection(in string section)
    {
        builder = new BuilderTest();
        return builder;
    }

    unittest
    {
        auto director = new ServiceDirectorTest();
        auto s = ["", "[foo]"];
        auto const linesNumber = 10;
        foreach (i; 0 .. linesNumber)
        {
            s ~= "";
        }

        director.parseAndGetService(s, "");
        assert(director.builder.timesParsed == linesNumber);
        assert(director.builder.parsingEnded);
    }

    unittest
    {
        auto director = new ServiceDirectorTest();
        auto s = ["foo", ""];
        import std.exception : assertThrown;

        assertThrown!Exception(director.parseAndGetService(s, ""));
    }

    BuilderTest builder;
}
