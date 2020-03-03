// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.utils;

@safe:

import std.stdio : File;

import libtt.exception : BuilderException;
import libtt.parser.section.section_builder : SectionBuilder;

@system final void testBuilderWithFile(SectionBuilder builder, string path)
{
    auto file = File(path, "r");
    auto range = file.byLineCopy();
    foreach (line; range)
    {
        builder.parseLine(line);
    }
    builder.endParsing();
}

void attributeNotFound(string attribute, string section)
{
    auto errorMessage = `Camp named "` ~ attribute ~ `" is not allowed`
        ~ " in section [" ~ section ~ "]";
    throw new BuilderException(errorMessage);
}
