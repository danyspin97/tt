// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.instance_service_parser;

@safe:

import std.array : array;
import std.algorithm : each;
import std.stdio : File;
import std.string : tr;

import libtt.parser.service.service_parser : ServiceParser;

enum InstanceToken = "@I";

class InstanceServiceParser : ServiceParser
{
public:
    this(in string path, in string instanceName) @system
    {
        this.instanceName = instanceName;
        super(path);
    }

protected:
    override string[] generateListFrom(File file) @system
    {
        string[] list = file.byLineCopy().array();
        list.each!((ref s) => replaceTokenInLine(s));
        return list;
    }

private:
    void replaceTokenInLine(ref string line)
    {
        line = tr(line, InstanceToken, instanceName);
    }

    string instanceName;
}
