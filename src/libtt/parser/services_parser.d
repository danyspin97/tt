// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.services_parser;

import std.array : split;
import std.file : dirEntries, exists, isFile, SpanMode;
import std.regex : ctRegex, matchFirst;

import libtt.services.service : Service;
import libtt.parser.instance_service_parser : InstanceServiceParser;
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

    void parseServices(string[] serviceNames)
    {
        foreach (name; serviceNames)
        {
            parseService(name);
        }

        // TODO: handle dependencies
    }

private:
    void parseService(string serviceName)
    {
        if (serviceName in serviceSet)
        {
            return;
        }

        if (isInstancedService(serviceName))
        {
            string instanceName;
            splitServiceNameFromInstance(serviceName, instanceName);
            auto path = getPathForServiceName(serviceName);
            serviceSet[serviceName ~ instanceName] =
                    new InstanceServiceParser(path, instanceName).service;
            return;
        }

        auto path = getPathForServiceName(serviceName);
        serviceSet[serviceName] = new ServiceParser(path).service;
    }

    bool isInstancedService(string serviceName)
    {
        auto regex = ctRegex!r"[\w-]+@[\w-]+";
        if (matchFirst(serviceName, regex))
        {
            return true;
        }
        return false;
    }

    void splitServiceNameFromInstance(ref string service, ref string instance)
    {
        auto splitted = service.split("@");
        service = splitted[0];
        instance = splitted[1];
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

