// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.instance_service_parser;

import std.container : DList;
import std.stdio : File;
import std.string : tr;

import libtt.parser.service.service_parser : ServiceParser;

enum InstanceToken = "@I";

class InstanceServiceParser : ServiceParser
{
public:
    this (string path, string instanceName)
    {
        this.instanceName = instanceName;
        super(path);
    }
protected:
    override DList!string generateListFrom(File file)
    {
        auto list = DList!string();
        foreach (line ; file.byLine())
        {
            auto parsedLine = replaceTokenInLine(line);
            list.insertBack(parsedLine.idup);
        }
        return list;
    }

private:
    char[] replaceTokenInLine(char[] line)
    {
        return tr(line, InstanceToken, instanceName);
    }

    string instanceName;
}
