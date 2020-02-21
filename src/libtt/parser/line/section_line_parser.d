// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.line.section_line_parser;

import libtt.exception : WordNotValidException;
import libtt.parser.word : SectionWordParser, WhitespaceParser;

class SectionLineParser
{
public:
    @property string section()
    {
        return m_section;
    }

    @property bool lineValid()
    {
        return m_valid;
    }

    this(string line)
    {
        this.line = line;
        parseLine();
    }

    unittest
    {
        auto parser = new SectionLineParser("[foo]");
        assert(parser.lineValid);
    }

    unittest
    {
        auto parser = new SectionLineParser("foo");
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

    unittest
    {
        auto parser = new SectionLineParser("[foo]");
        assert(parser.getSectionOrDefault("bar") == "foo");
    }

    unittest
    {
        auto parser = new SectionLineParser("foo");
        auto _default = "bar";
        assert(parser.getSectionOrDefault(_default) == _default);
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
        catch (WordNotValidException e)
        {
        }
    }

    void tryParseLine()
    {
        scope (success)
            m_valid = true;
        scope (failure)
            m_valid = false;

        auto sectionParser = new SectionWordParser();
        line = sectionParser.parse(line);
        (new WhitespaceParser()).parse(line);

        m_section = sectionParser.section;
    }

    unittest
    {
        auto parser = new SectionLineParser();
        parser.line = "[foo]";
        import std.exception : assertNotThrown;

        assertNotThrown!WordNotValidException(parser.tryParseLine());
        assert(parser.section == "foo");
    }

    unittest
    {
        auto parser = new SectionLineParser();
        parser.line = "foo";
        import std.exception : assertThrown;

        assertThrown!WordNotValidException(parser.tryParseLine());
    }

    string line;
    string m_section;
    bool m_valid;
}
