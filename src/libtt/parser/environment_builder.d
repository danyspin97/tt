// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.environment_builder;

import libtt.parser.section_builder : SectionBuilder;
import libtt.services.environment : Environment;

class EnvironmentBuilder : SectionBuilder
{
public:
    this(Environment environment)
    {
        this.environment = environment;
    }

    override void parseLine(string line)
    {

    }

    override void endParsing()
    {

    }

private:
    Environment environment;
}

