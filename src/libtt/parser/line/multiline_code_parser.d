// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.line.multiline_code_parser;

@safe:

import std.string : strip;

import libtt.exception : CodeParserNotFinishedException, WordNotValidException;
import libtt.parser.word : KeyParser, TokenParser, WhitespaceParser;

class MultilineCodeParser
{
public:
    @property bool isParsing() const
    {
        return m_isParsing;
    }

    @property string code() const
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

        try
        {
            tryStartParsing(line);
        }
        catch (WordNotValidException e)
        {
            return false;
        }

        return true;
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
        auto token = "execute = (";
        assert(parser.startParsing(token));
        assert(parser.isParsing());

        // Continue parsing
        auto code = "foo";
        parser.parseLine(code);
        assert(parser.m_code == code);

        // End parsing
        parser.parseLine(")");
        assert(parser.m_code == code);

        // Start parsing again
        import std.exception : assertThrown;

        assertThrown!CodeParserNotFinishedException(parser.startParsing(token));
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        auto token = "execute=(";
        parser.m_code = "foo";
        import std.exception : assertThrown;

        assertThrown!CodeParserNotFinishedException(parser.startParsing(token));
    }

    void parseLine(string line) nothrow
    {
        if (strip(line) == ")")
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
    void tryStartParsing(string line)
    {
        scope(success) m_isParsing = true;
        scope(failure) m_isParsing = false;

        auto keyParser = new KeyParser();
        auto assignmentParser = new TokenParser('=');
        auto openParenthesisParser = new TokenParser('(');
        line = keyParser.parse(line);
        line = assignmentParser.parse(line);
        line = openParenthesisParser.parse(line);
        (new WhitespaceParser()).parse(line);
    }

    string m_code;
    bool m_isParsing = false;
}
