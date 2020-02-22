// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.longrun_options_builder;

@safe:
nothrow:

import libtt.parser.section.section_builder : SectionBuilder;
import libtt.data : LongrunOptions;

class LongrunOptionsBuilder : SectionBuilder
{
public:
    this(in LongrunOptions longrunOptions)
    {
        this.longrunOptions = longrunOptions;
    }

    override void parseLine(string line)
    {

    }

    override void endParsing()
    {

    }

private:
    const LongrunOptions longrunOptions;
}
