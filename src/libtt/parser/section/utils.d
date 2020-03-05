// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.utils;

@safe:

import std.stdio : File;
import std.string : strip;

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

void attributeNotFound(in string attribute, in string section)
{
    auto errorMessage = `Camp named "` ~ attribute ~ `" is not allowed`
        ~ " in section [" ~ section ~ "]";
    throw new BuilderException(errorMessage);
}

bool isEmptyLine(in string line)
{
    const auto strippedLine = line.strip;
    if (strippedLine.length == 0 || strippedLine[0] == '#')
    {
        return true;
    }

    return false;
}

unittest
{
    assert(isEmptyLine(""));
    assert(isEmptyLine(" "));
    assert(isEmptyLine("# "));
    assert(isEmptyLine(" # "));
}

unittest
{
    assert(!isEmptyLine("foo = bar"));
    assert(!isEmptyLine("  foo = bar"));
}
