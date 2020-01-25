// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.dummy_builder;

import libtt.parser.section_builder : SectionBuilder;

class DummyBuilder : SectionBuilder
{
public:
    void parseLine(string line)
    {
        if (line != "")
        {
            // TODO: Handle this at a higher level
            throw new Exception("");
        }
    }

    void endParsing()
    {

    }
}
