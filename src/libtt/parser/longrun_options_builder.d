// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.longrun_options_builder;

import libtt.parser.section_builder : SectionBuilder;
import libtt.services.longrun_options : LongrunOptions;

class LongrunOptionsBuilder : SectionBuilder
{
public:
    this(LongrunOptions longrunOptions)
    {
        this.longrunOptions = longrunOptions;
    }

    override void parseLine(string line)
    {

    }

private:
    LongrunOptions longrunOptions;
}

