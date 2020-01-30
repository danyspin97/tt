// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.environment_builder;

import std.regex : ctRegex, matchAll;

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
        auto regex = ctRegex!r" *(\w+) *= *(.*) *";
        auto match = matchAll(line, regex);
        if (!match)
        {
            throw new Exception(`Line "` ~ line ~ `" is not valid`);
        }

        environment.set(match.front[1], match.front[2]);
    }

    override void endParsing()
    {

    }

private:
    Environment environment;
}

