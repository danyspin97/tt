// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.script_builder;

import libtt.parser.section_builder : SectionBuilder;
import libtt.services.script : Script;
import libtt.services.environment : Environment;

class ScriptBuilder : SectionBuilder
{
public:
    this(ref Script script, ref Environment environment)
    {
        this.script = script;
        this.environment = environment;
    }

    override void parseLine(string line)
    {

    }

private:
    Script script;
    Environment environment;
}
