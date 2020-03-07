// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.oneshot_director;

@safe:

import libtt.parser.section : EnvironmentBuilder, MainSection,
    MainSectionBuilder, OneshotOptionsBuilder, ScriptBuilder, SectionBuilder;
import libtt.parser.service.service_director : ServiceDirector;
import libtt.data : Environment, Oneshot, OneshotOptions, Script, Service;

class OneshotDirector : ServiceDirector
{
public:
    this()
    {
        super();
        options = new OneshotOptions();
        environment = new Environment();
    }

    @system unittest
    {
        auto director = new OneshotDirector();
        import std.array : array;
        import std.stdio : File;

        auto lines = new File("src/libtt/test/init-fsck.system").byLineCopy().array();
        Oneshot o = cast(Oneshot) director.parseAndGetService(lines, "/tmp/init-fsck");

        assert(o);
        assert(o.name == "init-fsck");
        assert(o.start);
        assert(o.start.environment.get("CMDARGS") == "-d");
    }

    override Service instanceService(in string path)
    {
        auto service = new Oneshot(mainSection.name, mainSection.polishName,
                mainSection.description, path, options, start,);

        if (stop !is null)
        {
            service.stop = stop;
        }

        return service;
    }

    override SectionBuilder getBuilderForSection(in string section)
    {
        switch (section)
        {
        case "main":
            return new MainSectionBuilder(&mainSection);
        case "start":
            return new ScriptBuilder(&start, environment);
        case "stop":
            return new ScriptBuilder(&stop, environment);
        case "config":
            return new EnvironmentBuilder(environment);
        case "options":
            return new OneshotOptionsBuilder(options);
        default:
            immutable auto msg = `Section "` ~ section ~ `" is not supported.`;
            throw new Exception(msg);
        }
    }

private:
    MainSection mainSection;
    Script start;
    Script stop;
    Environment environment;
    OneshotOptions options;
}
