// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service_parser.d;

import std.ascii : newline;
import std.exception : enforce;
import std.format : FormatException;
import std.path : baseName, stripExtension;
import std.stdio : File;

import libtt.parser.key_value_parser : KeyValueParser;
import libtt.parser.main_section : MainSection;
import libtt.parser.section_parser : SectionParser;
import libtt.services.service : Service;

class ServiceParser
{
public:
    this(string path)
    {
        mainSection.path = path;
        openFile();
        parseMainSection();
        validateName();
        service = dispatchParserPerType();
    }

private:
    void openFile()
    {
        file = File(mainSection.path, "r");
    }

    unittest
    {
        auto parser = new ServiceParser();
        parser.mainSection.path = "src/libtt/test/mainSection";
        parser.openFile();
    }

    void parseMainSection()
    {
        scanForMainSection();
        while (fileNotFinished())
        {
            currentLine = file.readln();

            if (currentLine == "")
            {
                continue;
            }

            auto sectionParser = new SectionParser(currentLine);
            if (sectionParser.lineValid())
            {
                return;
            }

           handleCamp();
        }
    }

    void scanForMainSection()
    {
        while (fileNotFinished())
        {
            auto line = file.readln();
            auto sectionParser = new SectionParser(line);
            auto currentSection = sectionParser.getSectionOrDefault("");
            if (currentSection == "main")
            {
                return;
            }
        }

        auto errorMessage = "No main section could be found for service "
                ~ mainSection.path;
        throw new Exception(errorMessage);
    }

    unittest
    {
        auto parser = new ServiceParser();
        parser.mainSection.path = "src/libtt/test/mainSection";
        parser.openFile();
        import std.exception : assertNotThrown;
        assertNotThrown!Exception(parser.scanForMainSection());
    }

    unittest
    {
        auto parser = new ServiceParser();
        parser.mainSection.path = "src/libtt/test/noSection";
        parser.openFile();
        import std.exception : assertThrown;
        assertThrown!Exception(parser.scanForMainSection());
    }


    void handleCamp()
    {
        string key, value;
        auto keyValueParser = new KeyValueParser(currentLine);
        if (keyValueParser.lineValid())
        {
            key = keyValueParser.key;
            value = keyValueParser.value;
        }

        // TODO: Handle multiline values here

        fillServiceDataWith(key, value);
    }

    void fillServiceDataWith(string key, string value)
    {
        switch (key)
        {
            case "name":
                mainSection.name = value;
                break;
            case "description":
                mainSection.description = value;
                break;
            case "polish_name":
                mainSection.polishName = value;
                break;
            case "type":
                mainSection.type = value;
                break;
            default:
                auto errorMessage = `Camp named "` ~ key ~ `" is not allowed.`;
                throw new Exception(errorMessage);
        }
    }

    void validateName()
    {
        auto nameFromPath = stripExtension(baseName(mainSection.path));
        auto enforceMessage = "The name camp must match the name of the file";
        enforce(mainSection.name == nameFromPath, enforceMessage);
    }

    Service dispatchParserPerType()
    {
        switch (mainSection.type)
        {
            case "bundle":
                return null;
            case "longrun":
                return null;
            case "oneshot":
                return null;
            default:
                auto msg = `Type "` ~ mainSection.type ~ `" is not supported.`;
                throw new Exception(msg);
        }
    }

    bool fileNotFinished()
    {
        return !file.eof;
    }

    unittest
    {
        auto parser = new ServiceParser();
        parser.mainSection.path = "src/libtt/test/mainSection";
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

    File file;
    Service service;
    string currentLine;

    MainSection mainSection;
}

