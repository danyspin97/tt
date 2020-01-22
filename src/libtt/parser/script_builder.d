// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.script_builder;

import libtt.parser.section_builder : SectionBuilder;
import libtt.services.script : Script;

class ScriptBuilder : SectionBuilder
{
public:
    this(Script script)
    {
        this.script = script;
    }

    override void parseLine(string line)
    {

    }

private:
    Script script;
}
