// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.section_builder;

import std.stdio : File;

@safe:
nothrow:

interface SectionBuilder
{
    void parseLine(in string line);
    void endParsing();

    @system static void testBuilderWithFile(SectionBuilder builder, string path)
    {
        auto file = File(path, "r");
        auto range = file.byLineCopy();
        foreach (line; range)
        {
            builder.parseLine(line);
        }
        builder.endParsing();
    }
}
