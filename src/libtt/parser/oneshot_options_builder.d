// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.oneshot_options_builder;

import libtt.parser.section_builder : SectionBuilder;
import libtt.services.oneshot_options : OneshotOptions;

class OneshotOptionsBuilder : SectionBuilder
{
public:
    this(OneshotOptions oneshotOptions)
    {
        this.oneshotOptions = oneshotOptions;
    }

    override void parseLine(string line)
    {

    }

private:
    OneshotOptions oneshotOptions;
}

