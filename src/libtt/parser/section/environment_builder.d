// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.environment_builder;

@safe:
nothrow:

import std.regex : ctRegex, matchAll;

import libtt.parser.section.section_builder : SectionBuilder;
import libtt.data : Environment;

class EnvironmentBuilder : SectionBuilder
{
public:
    this(Environment environment)
    {
        this.environment = environment;
    }

    override void parseLine(in string line)
    {
        // Use a specialized regex instead of using KeyValueParser
        // The latter can parse values without quotes, and this shouldn't be
        // allowed when parsing the environment.
        // Environment keys are also more strict rules
        auto regex = ctRegex!` *(\w+) *= *"(.*)" *`;
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
