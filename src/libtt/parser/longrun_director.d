// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.longrun_director;

import libtt.parser.section : EnvironmentBuilder, LoggerScriptBuilder,
                              LongrunOptionsBuilder, MainSectionBuilder,
                              ScriptBuilder, SectionBuilder;
import libtt.parser.main_section : MainSection;
import libtt.parser.service_director : ServiceDirector;
import libtt.services.environment : Environment;
import libtt.services.logger_script : LoggerScript;
import libtt.services.longrun: Longrun;
import libtt.services.longrun_options : LongrunOptions;
import libtt.services.script : Script;
import libtt.services.service : Service;

class LongrunDirector : ServiceDirector
{
public:
    override Service instanceService(string path)
    {
        auto service = new Longrun(
            mainSection.name,
            mainSection.polishName,
            mainSection.description,
            mainSection.path,
            options,
            run,
        );

        if (finish !is null)
        {
            service.finish = finish;
        }
        if (logger !is null)
        {
            service.logger = logger;
        }

        return service;
    }

    override SectionBuilder getBuilderForSection(string section)
    {
        switch (section)
        {
            case "main":
                return new MainSectionBuilder(mainSection);
            case "run":
                return new ScriptBuilder(run, environment);
            case "finish":
                return new ScriptBuilder(finish, environment);
            case "logger":
                return new LoggerScriptBuilder(logger, environment);
            case "config":
                return new EnvironmentBuilder(environment);
            case "options":
                return new LongrunOptionsBuilder(options);
            default:
                auto msg = `Section "` ~ section ~ `" is not supported.`;
                throw new Exception(msg);
        }
    }

private:
    Environment environment = new Environment();
    MainSection mainSection;
    Script run;
    Script finish;
    LoggerScript logger;
    LongrunOptions options = new LongrunOptions();
}

