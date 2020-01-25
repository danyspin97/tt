// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.oneshot_director;

import libtt.parser.environment_builder : EnvironmentBuilder;
import libtt.parser.main_section : MainSection;
import libtt.parser.main_section_builder : MainSectionBuilder;
import libtt.parser.oneshot_options_builder : OneshotOptionsBuilder;
import libtt.parser.script_builder : ScriptBuilder;
import libtt.parser.section_builder : SectionBuilder;
import libtt.parser.service_director : ServiceDirector;
import libtt.services.environment : Environment;
import libtt.services.oneshot : Oneshot;
import libtt.services.oneshot_options : OneshotOptions;
import libtt.services.script : Script;
import libtt.services.service : Service;

class OneshotDirector : ServiceDirector
{
public:
    override Service instanceService(string path)
    {
        auto service = new Oneshot(
            mainSection.name,
            mainSection.polishName,
            mainSection.description,
            mainSection.path,
            options,
            start,
        );

        if (stop !is null)
        {
            service.stop = stop;
        }

        return service;
    }

    override SectionBuilder getBuilderForSection(string section)
    {
        switch (section)
        {
            case "main":
                return new MainSectionBuilder(mainSection);
            case "start":
                return new ScriptBuilder(start, environment);
            case "stop":
                return new ScriptBuilder(stop, environment);
            case "config":
                return new EnvironmentBuilder(environment);
            case "options":
                return new OneshotOptionsBuilder(options);
            default:
                auto msg = `Section "` ~ section ~ `" is not supported.`;
                throw new Exception(msg);
        }
    }

private:
    MainSection mainSection;
    Script start;
    Script stop;
    Environment environment = new Environment();
    OneshotOptions options;
}
