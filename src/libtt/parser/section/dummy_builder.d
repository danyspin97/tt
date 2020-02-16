// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.dummy_builder;

import libtt.exception : DummyBuilderException;
import libtt.parser.section.section_builder : SectionBuilder;

class DummyBuilder : SectionBuilder
{
public:
    void parseLine(string line)
    {
        if (line != "")
        {
            throw new DummyBuilderException("");
        }
    }

    unittest
    {
        auto builder = new DummyBuilder();
        import std.exception : assertThrown;
        assertThrown!DummyBuilderException(builder.parseLine("foobar"));
    }

    void endParsing()
    {

    }
}
