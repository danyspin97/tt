// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.line.multiline_value_parser;

import std.algorithm : each, endsWith, filter, startsWith;
import std.array : array;
import std.exception : assertNotThrown, assertThrown;
import std.range : popFrontN, popBackN;
import std.regex : ctRegex, matchAll, splitter;
import std.string: strip;
import std.uni : isWhite;

import libtt.exception : EmptyValueFoundWhileParsingException,
                         LineNotValidWhileParsingException,
                         ParserHasNotBeenInitialized,
                         ParserIsStillParsingException,
                         ValueNotValidWhileParsing;

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

        auto regex = ctRegex!r" *(\w+) *= *(\( *)(\w+(:?-\w+)? +)*( *\))? *";
        auto match = matchAll(line, regex);
        if (!match)
        {
            return false;
        }

        auto sub = match.front;
        // The first value (sub[0]) is the entire line
        m_key = sub[1];
        auto openingParenthesis = sub[2];
        sub.popFrontN(3);
        setValues(sub);
        m_isParsing = true;
        if (m_values.length != 0)
        {
            checkEndingParenthesis();
        }
        checkOpeningParenthesis(openingParenthesis);

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

    // unittest
    // {
    //     auto parser = new MultilineValueParser();
    //     assertThrown!LineNotValidWhileParsingException(
    //         parser.startParsing(`foo = ( foo)`)
    //     );
    // }

    unittest
    {
        auto linesWithEmptyValues =
        [
            "foo = ()",
            "foo = ( )",
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

        auto regex = ctRegex!r"^ *(\w+(:?-\w+)? +)*( *\))? *$";
        auto match = matchAll(line, regex);
        if (!match)
        {
            throw new LineNotValidWhileParsingException("");
        }

        auto sub = match.front;
        sub.popFrontN(1);
        auto previousNumOfValues = m_values.length;
        setValues(sub);
        if (previousNumOfValues != m_values.length)
        {
            checkEndingParenthesis();
        }
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

    unittest
    {
        auto parser = new MultilineValueParser();
        auto line = "foo = bar";
        parser.m_isParsing = true;
        assertThrown!LineNotValidWhileParsingException(parser.parseLine(line));
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
    static void checkOpeningParenthesis(string openingParenthesis)
    {
        if (openingParenthesis.startsWith("(") && openingParenthesis == "(")
        {
            auto msg = "A space is needed after the parenthesis";
            throw new LineNotValidWhileParsingException(msg);
        }
    }

    unittest
    {
        assertNotThrown!LineNotValidWhileParsingException(
            checkOpeningParenthesis("( ")
        );
    }

    void setValues(R)(R matches)
    {
        matches.filter!(s => s != "").
                each!(s => m_values ~= s.strip());
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        auto line = ["foo", "bar"];
        parser.setValues(line[]);
        assert(parser.values == ["foo", "bar"]);
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        auto line = ["foo ", "", "bar"];
        parser.setValues(line[]);
        assert(parser.values == ["foo", "bar"]);
    }

    void checkEndingParenthesis()
    {
        if (m_values.length == 0)
        {
            return;
        }

        string last = m_values[$-1];
        if (last.endsWith(")"))
        {
            if (m_values.length == 1)
            {
                throw new EmptyValueFoundWhileParsingException("");
            }
            if (last == ")")
            {
                auto msg = "A space is needed before the parenthesis";
                throw new LineNotValidWhileParsingException(msg);
            }
            m_values = m_values[0 .. $-1];
            m_isParsing = false;
        }
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        auto testValues = [
            [ "foo", " )" ],
            [ "foo" ]
        ];
        foreach (values ; testValues)
        {
            parser.m_values = values;
            assertNotThrown!EmptyValueFoundWhileParsingException(
                parser.checkEndingParenthesis()
            );
            parser.m_values = values;
            assertNotThrown!LineNotValidWhileParsingException(
                parser.checkEndingParenthesis()
            );
            assert(parser.values == ["foo"]);
        }
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        parser.m_isParsing = true;
        auto testValues = [
            [ "foo" ],
            []
        ];
        foreach (values ; testValues)
        {
            parser.m_values = values;
            parser.checkEndingParenthesis();
            assert(parser.isParsing());
        }
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        parser.m_values = ["foo", ")"];
        parser.checkEndingParenthesis();
        assert(!parser.isParsing());
    }

    unittest
    {
        auto testValues = [
            [ ")" ],
            [ " )" ],
        ];
        foreach (values ; testValues)
        {
            auto parser = new MultilineValueParser();
            parser.m_values = values;
            assertThrown!EmptyValueFoundWhileParsingException(
                parser.checkEndingParenthesis()
            );
        }
    }

    unittest
    {
        auto parser = new MultilineValueParser();
        parser.m_values = ["foo", ")"];
        assertThrown!LineNotValidWhileParsingException(
            parser.checkEndingParenthesis()
        );
    }

    string m_key;
    string[] m_values;
    bool m_isParsing = false;
}

