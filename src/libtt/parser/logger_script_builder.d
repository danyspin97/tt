// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.loggere_script_builder;

import libtt.parser.section_builder : SectionBuilder;
import libtt.services.logger_script : LoggerScript;

class LoggerScriptBuilder : SectionBuilder
{
public:
    this(LoggerScript loggerScript)
    {
        this.loggerScript = loggerScript;
    }

    override void parseLine(string line)
    {

    }

private:
    LoggerScript loggerScript;
}
