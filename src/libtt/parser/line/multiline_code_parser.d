// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.line.multiline_code_parser;

@safe:

import std.ascii : newline;
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

        return m_code[0 .. $ - 1].idup;
    }

    @property string key() const
    {
        return m_key;
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        auto code = "foo".dup;
        parser.m_code = code ~ newline;
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
        if (isParsing())
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
        assert(parser.key == "execute");

        // Continue parsing
        auto line1 = "foo";
        parser.parseLine(line1);
        assert(parser.m_code == line1 ~ newline);
        assert(parser.isParsing());

        auto line2 = "bar";
        parser.parseLine(line2);
        assert(parser.isParsing());

        // Start parsing again
        import std.exception : assertThrown;

        assertThrown!CodeParserNotFinishedException(parser.startParsing(token));

        // End parsing
        parser.parseLine(")");
        assert(parser.code == line1 ~ newline ~ line2);
        assert(!parser.isParsing());
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        parser.m_isParsing = true;
        parser.m_code = "foo".dup;
        import std.exception : assertThrown;

        assertThrown!CodeParserNotFinishedException(parser.code);
    }

    void parseLine(string line) nothrow
    {
        if (strip(line) == ")")
        {
            m_isParsing = false;
            return;
        }

        m_code ~= line.dup ~ newline;
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        parser.m_isParsing = true;
        auto code = "foo";
        parser.parseLine(code);
        assert(parser.isParsing());
        // Otherwise CodeParserNotFinishedException will be thrown
        parser.m_isParsing = false;
        assert(parser.code == code);
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        parser.m_isParsing = true;
        parser.parseLine(" ) ");
        assert(!parser.isParsing());
    }

    void reset()
    {
        m_code = [];
        m_isParsing = false;
        m_key = "";
    }

    unittest
    {
        auto parser = new MultilineCodeParser();
        parser.m_isParsing = true;
        parser.m_key = "execute";
        parser.m_code = "echo".dup;
        parser.reset();
        assert(!parser.isParsing());
        assert(parser.key == "");
        assert(parser.m_code == "");
    }

private:
    void tryStartParsing(string line)
    {
        scope (success)
            m_isParsing = true;
        scope (failure)
            m_isParsing = false;

        auto keyParser = new KeyParser();
        auto assignmentParser = new TokenParser('=');
        auto openParenthesisParser = new TokenParser('(');
        line = keyParser.parse(line);
        line = assignmentParser.parse(line);
        line = openParenthesisParser.parse(line);
        (new WhitespaceParser()).parse(line);

        m_key = keyParser.key;
    }

    char[] m_code;
    string m_key;
    bool m_isParsing = false;
}
