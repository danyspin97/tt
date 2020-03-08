// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.action.parse_action;

import std.file : exists, isFile;
import std.format : format;
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
        foreach (s ; options.services)
        {
            auto parser = new ServiceParser(s);
            //parser.service.toString();
            continue;
        }
    }

    void parseUserSystemServices()
    {
        auto dirs = [dirs.service, dirs.adminService];
        // TODO: Check for UID != 0 and add xdg.userservice
        foreach (s ; options.services)
        {
            bool found;
            auto possibleNames = [s];
            if (s.chomp(".system") != s && s.chomp(".user") != s)
            {
                possibleNames ~= [s ~ ".system", s ~ ".user"];
            }
            services: foreach (name ; possibleNames)
            {
                foreach_reverse (dir; dirs)
                {
                    auto file = dir ~ "/" ~ name;
                    if (name.exists() && name.isFile())
                    {
                        auto parser = new ServiceParser(s);
                        found = true;
                        break services;
                    }
                }
            }

            if (!found)
            {
                throw new Exception(format("Service %s could not be found", s));
            }
        }
    }

private:
    ParseOptions options;
}
