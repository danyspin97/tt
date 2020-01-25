// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.services_parser;

import std.file : dirEntries, exists, isFile, SpanMode;

import libtt.services.service : Service;
import libtt.parser.service_parser : ServiceParser;

abstract class ServicesParser
{
public:
    Service[] services()
    {
        Service[] services;
        foreach (name, service ; serviceSet)
        {
            services ~= service;
        }
        return services;
    }

private:
    void parseServices(string[] serviceNames) {
        string[string] pathForName;

        foreach (name; serviceNames)
        {
            parseService(name);
        }

        // TODO: handle dependencies
    }

    void parseService(string serviceName)
    {
        if (name in serviceSet)
        {
            return;
        }

        auto path = getPathForServiceName(name);
        serviceSet[name] = new ServiceParser(path).service;
    }

    string getPathForServiceName(string name)
    {
        foreach_reverse (dirPath; paths)
        {
            auto servicePath = dirPath ~ "/" ~ name ~ suffix;
            if (exists(servicePath) && isFile(servicePath))
            {
                return servicePath;
            }
        }

        auto msg = "Service " ~ name ~ suffix ~ " could not be found.";
        throw new Exception(msg);
    }

protected:
    string suffix;
    string[] paths;

private:
    Service[string] serviceSet;
}

