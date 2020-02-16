// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.line.multiline_code_parser;

import std.regex : ctRegex, matchFirst;
import std.string : strip;

import libtt.exception : CodeParserNotFinishedException;

class MultilineCodeParser
{
public:
    @property bool isParsing() { return m_isParsing; }
    @property string code()
    {
        if (isParsing())
        {
            throw new CodeParserNotFinishedException("");
        }

        return m_code;
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        auto code = "foo";
        parser.m_code = code;
        assert(parser.code == code);
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        parser.m_isParsing = true;
        import std.exception : assertThrown;
        assertThrown!CodeParserNotFinishedException(parser.code);
    }

    this()
    {

    }

    bool startParsing(string line)
    {
        if (isParsing() || m_code != "")
        {
            throw new CodeParserNotFinishedException("");
        }
        auto regex = ctRegex!r"^ *execute ?= ?\( *$";
        if (matchFirst(line, regex))
        {
            m_isParsing = true;
            return true;
        }

        return false;
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        assert(!parser.startParsing("foo"));
        assert(!parser.isParsing());

        assert(!parser.startParsing("execute = "));
        assert(!parser.isParsing());

        assert(!parser.startParsing("execute = ( foo "));
        assert(!parser.isParsing());

        assert(!parser.startParsing("execute = ( foo )"));
        assert(!parser.isParsing());
    }

    unittest
    {
        // Start parsing
        auto parser = new MultilineCodeParser();
        assert(parser.startParsing("execute = ("));
        assert(parser.isParsing());

        // Continue parsing
        auto code = "foo";
        parser.parseLine(code);
        assert(parser.m_code == code);

        // End parsing
        parser.parseLine(")");
        assert(parser.m_code == code);
    }

    void parseLine(string line)
    {
        if(strip(line) == ")")
        {
            m_isParsing = false;
            return;
        }

        m_code ~= line;
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        parser.m_isParsing = true;
        auto code = "foo";
        parser.parseLine(code);
        assert(parser.isParsing());
        assert(parser.m_code == code);
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        parser.m_isParsing = true;
        parser.parseLine(" ) ");
        assert(!parser.isParsing());
    }

private:
    string m_code;
    bool m_isParsing = false;
}

