// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service_parser.d;

import std.ascii : newline;
import std.stdio : File;

import libtt.services.service : Service;

class ServiceParser
{
public:
    this(string path)
    {
        this.path = path;
        openFile();
        scanForMainSection();
        parseMainSection();
        service = dispatchParserPerType();
    }

    void openFile()
    {
        file = File(path, "r");
    }

    void scanForMainSection()
    {
        while (fileNotFinished())
        {
            auto currentSection = getSectionOrDefault("");
            if (currentSection == "main")
            {
                return;
            }
        }

        auto errorMessage = "No main section could be found for service "
                ~ path;
        throw new Exception(errorMessage);
    }

    void parseMainSection()
    {
        while (fileNotFinished())
        {

        }
    }

    Service dispatchParserPerType()
    {
        return null;
    }

    string getSectionOrDefault(string _default)
    {
        import std.format : FormatException;
        try
        {
            string section;
            file.readf!"[%s]"(section);
            return section;
        }
        catch (FormatException e)
        {
            return _default;
        }
    }

    bool fileNotFinished()
    {
        return !file.eof;
    }

private:
    string path;
    File file;
    Service service;

    // Base Service attributes
    string name;
    string polishName;
    string description;

    string type;
}

