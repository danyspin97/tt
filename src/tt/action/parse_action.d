// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.action.parse_action;

import std.file : exists, isFile;
import std.format : format;
import std.stdio : writeln;
import std.string : chomp;

import libtt.dirs : dirs;
import libtt.parser.service : ServiceParser;

import tt.action.action : Action;
import tt.options : ParseOptions;

@system:

class ParseAction : Action
{
public:
    this(ParseOptions options) @safe
    {
        this.options = options;
    }

    override void execute()
    {
        if (options.isFile)
        {
            parseFiles();
            return;
        }

        parseUserSystemServices();
    }

    void parseFiles()
    {
        foreach (s; options.services)
        {
            auto parser = new ServiceParser(s);
            if (!options.quiet)
            {
                writeln(parser.service.toString());
            }
            continue;
        }
    }

    void parseUserSystemServices()
    {
        // TODO: Check for UID != 0 and add xdg.userservice
        foreach (s; options.services)
        {
            bool found;
            foreach (name; getPossibleNameForService(s))
            {
                found = checkForFileInDefaultDirs(name);
                if (found)
                {
                    break;
                }
            }

            if (!found)
            {
                throw new Exception(format("Service %s could not be found", s));
            }
        }
    }

    bool checkForFileInDefaultDirs(string name)
    {
        auto defaultDirs = [dirs.service, dirs.adminService];
        foreach_reverse (dir; defaultDirs)
        {
            auto file = dir ~ "/" ~ name;
            if (file.exists() && file.isFile())
            {
                auto parser = new ServiceParser(name);
                if (!options.quiet)
                {
                    writeln(parser.service.toString());
                }
                return true;
            }
        }
        return false;
    }

    string[] getPossibleNameForService(string service)
    {
        auto possibleNames = [service];
        if (service.chomp(".system") != service && service.chomp(".user") != service)
        {
            possibleNames ~= [service ~ ".system", service ~ ".user"];
        }
        return possibleNames;
    }

private:
    ParseOptions options;
}
