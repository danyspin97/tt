// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section_line_parser;

import std.exception : enforce;
import std.format : FormatException, formattedRead;
import std.string : strip;

class SectionLineParser
{
public:
    @property string section() { return m_section; }
    @property bool lineValid() { return m_valid; }

    this(string line)
    {
        this.line = line;
        parseLine();
    }

    unittest
    {
        auto line = "[foo]";
        auto parser = new SectionLineParser(line);
        assert(parser.lineValid);
    }

    unittest
    {
        auto line = "foo";
        auto parser = new SectionLineParser(line);
        assert(!parser.lineValid);
    }

    string getSectionOrDefault(string _default)
    {
        if (lineValid())
        {
            return section;
        }

        return _default;
    }

private:

    // Only used for unit testing
    this()
    {

    }

    void parseLine()
    {
        try
       {
            tryParseLine();
        }
        catch (FormatException e)
        {
        }
    }

    void tryParseLine()
    {
        scope(success) m_valid = true;
        scope(failure) m_valid = false;

        line = strip(line);
        line.formattedRead!"[%s]"(m_section);
        enforce(m_section != "", "Section name cannot be empty.");
    }

    unittest
    {
        auto parser = new SectionLineParser();
        parser.line = "[foo]";
        import std.exception : assertNotThrown;
        assertNotThrown!FormatException(parser.tryParseLine());
        assert(parser.section == "foo");
    }

    unittest
    {
        auto parser = new SectionLineParser();
        parser.line = "foo";
        import std.exception : assertThrown;
        assertThrown!FormatException(parser.tryParseLine());
    }

    string line;
    string m_section;
    bool m_valid;
}
