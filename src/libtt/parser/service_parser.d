// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service_parser.d;

import std.ascii : newline;
import std.format : FormatException;
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

private:
    void openFile()
    {
        file = File(path, "r");
    }

    unittest
    {
        auto parser = new ServiceParser();
        parser.path = "../src/libtt/test/mainSection";
        parser.openFile();
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

    unittest
    {
        auto parser = new ServiceParser();
        parser.path = "../src/libtt/test/mainSection";
        parser.openFile();
        import std.exception : assertNotThrown;
        assertNotThrown!Exception(parser.scanForMainSection());
    }

    unittest
    {
        auto parser = new ServiceParser();
        parser.path = "../src/libtt/test/noSection";
        parser.openFile();
        import std.exception : assertThrown;
        assertThrown!Exception(parser.scanForMainSection());
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

    unittest
    {
        auto parser = new ServiceParser();
        parser.path = "../src/libtt/test/mainSection";
        parser.openFile();
        assert(parser.getSectionOrDefault("") == "main");
    }

    unittest
    {
        auto parser = new ServiceParser();
        parser.path = "../src/libtt/test/noSection";
        parser.openFile();
        assert(parser.getSectionOrDefault("") == "");
    }

    bool fileNotFinished()
    {
        return !file.eof;
    }

    unittest
    {
        auto parser = new ServiceParser();
        parser.path = "../src/libtt/test/mainSection";
        parser.openFile();
        assert(parser.fileNotFinished());
        parser.file.readln();
        assert(parser.file.readln() == "");
        assert(!parser.fileNotFinished());
    }

    // Only used in unit testing
    this()
    {

    }

    string path;
    File file;
    Service service;

    // Base Service attributes
    string name;
    string polishName;
    string description;

    string type;
}

