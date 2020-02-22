// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.section_builder;

@safe:
nothrow:

interface SectionBuilder
{
    void parseLine(in string line);
    void endParsing();
}
