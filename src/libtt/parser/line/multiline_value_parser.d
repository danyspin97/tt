// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.line.multiline_value_parser;

import std.algorithm : each, endsWith, filter, startsWith;
import std.array : split;
import std.exception : assertNotThrown, assertThrown;
import std.range : popFrontN, popBackN;
import std.string: strip;
import std.uni : isWhite;

import libtt.parser.word : KeyParser, TokenParser, UntilTokenParser,
                           WhitespaceParser;

import libtt.exception : EmptyValueFoundWhileParsingException,
                         WordNotValidException,
                         LineNotValidWhileParsingException,
                         ParserHasNotBeenInitialized,
                         ParserIsStillParsingException;

class MultilineValueParser
{
public:
    @property bool isParsing() { return m_isParsing; }
    @property string key() { return m_key; }
    @property string[] values() { return m_values; }

    this()
    {

    }

    bool startParsing(string line)
    {
        if (isParsing())
        {
            throw new ParserIsStillParsingException("");
        }

        try
        {
            tryStartParse(line);
        }
        catch (WordNotValidException e)
        {
            return false;
        }
        return true;
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        parser.m_isParsing = true;
        assertThrown!ParserIsStillParsingException(parser.startParsing(""));
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        assert(!parser.startParsing("foo"));
        assert(!parser.isParsing());

        assert(!parser.startParsing(`foo = "foo bar"`));
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        assertThrown!LineNotValidWhileParsingException(
            parser.startParsing(`foo = (foo )`)
        );
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        assertThrown!LineNotValidWhileParsingException(
            parser.startParsing(`foo = ( foo)`)
        );
    }

    unittest
    {
        auto linesWithEmptyValues =
        [
            "foo = (  )"
        ];
        foreach (line ; linesWithEmptyValues)
        {
            auto parser = new MultilineValueParser();
            assertThrown!EmptyValueFoundWhileParsingException(
                    parser.startParsing(line));
        }
    }

    void parseLine(string line)
    {
        if (!isParsing())
        {
            throw new ParserHasNotBeenInitialized("");
        }

        tryParseLine(line);
    }

    unittest
    {
       auto parser = new MultilineValueParser();
       auto val = "foo";
       parser.m_isParsing = true;
       parser.parseLine(val);
       assert(parser.values == [val]);

       auto val2 = "bar";
       parser.parseLine(val2 ~ " )");
       assert(!parser.isParsing());
       assert(parser.values == [val, val2]);
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        auto line = "foo bar";
        assertThrown!ParserHasNotBeenInitialized(parser.parseLine(line));
    }

    void reset()
    {
        m_values = [];
        m_isParsing = false;
        m_key = "";
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        parser.m_values = ["foo"];
        parser.m_isParsing = true;
        parser.m_key = "bar";
        parser.reset();
        assert(parser.values == []);
        assert(!parser.isParsing);
        assert(parser.m_key == "");
    }

private:
    bool tryStartParse(string line)
    {
        auto keyParser = new KeyParser();
        auto assignmentParser = new TokenParser('=');
        auto openParenthesisParser = new TokenParser('(');
        line = keyParser.parse(line);
        line = assignmentParser.parse(line);
        line = openParenthesisParser.parse(line);
        checkLineStartsWithWhitespace(line);

        auto res = !tryParseLine(line);
        checkParsedValuesNotEmpty();
        return res;
    }

    bool tryParseLine(string line)
    {
        try
        {
            tryParseValuesInLine(line);
        }
        catch (WordNotValidException e)
        {
            return false;
        }

        return true;
    }

    void tryParseValuesInLine(string line)
    {
        scope(failure) addValuesFromLine(line);
        scope(failure) m_isParsing = true;
        scope(success) m_isParsing = false;

        auto endingParenthesisParser = new UntilTokenParser(')');
        line = endingParenthesisParser.parseUntilToken(line);
        auto parsedLine = endingParenthesisParser.parsedLine;
        checkLineEndsWithWhitespace(parsedLine);
        addValuesFromLine(parsedLine);
    }

    static void checkLineStartsWithWhitespace(string line)
    {
        // A space is needed before the parenthesis
        if (line.length != 0 && !line[0].isWhite())
        {
            throw new LineNotValidWhileParsingException("");
        }
    }

    unittest
    {
        assertNotThrown!LineNotValidWhileParsingException(
            checkLineStartsWithWhitespace(" foo")
        );
    }

    unittest
    {
        assertThrown!LineNotValidWhileParsingException(
            checkLineStartsWithWhitespace("foo")
        );
    }

    static void checkLineEndsWithWhitespace(string line)
    {
        // A space is needed after the parenthesis
        if (line.length == 0 || !line[$-1].isWhite())
        {
            throw new LineNotValidWhileParsingException("");
        }
    }

    unittest
    {
        assertThrown!LineNotValidWhileParsingException(
            checkLineEndsWithWhitespace("")
        );
    }

    unittest
    {
        assertNotThrown!LineNotValidWhileParsingException(
            checkLineStartsWithWhitespace("        ")
        );
    }

    void addValuesFromLine(string line)
    {
        line.split!isWhite
            .filter!(s => s != "")
            .each!(s => m_values ~= s.strip());
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        auto line = "foo bar";
        parser.addValuesFromLine(line);
        assert(parser.values == ["foo", "bar"]);
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        auto line = "foo    bar ";
        parser.addValuesFromLine(line);
        assert(parser.values == ["foo", "bar"]);
    }

    void checkParsedValuesNotEmpty()
    {
        if (!isParsing() && m_values.length == 0)
        {
            throw new EmptyValueFoundWhileParsingException("");
        }
    }

    string m_key;
    string[] m_values;
    bool m_isParsing = false;
}

