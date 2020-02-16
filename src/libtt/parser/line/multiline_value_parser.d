// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.line.multiline_value_parser;

import std.array : split;
import std.regex : ctRegex, matchAll, splitter;
import std.string: strip;
import std.uni : isWhite;

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
            throw new Exception("");
        }

        // Match:
        // test = ( foo
        // test=(
        // test = ( foo )
        // test = ( foo bar
        // test = (  )
        // test = ( foo bar )
        auto regex = ctRegex!r" *(\w+) *= *\( +([\w- ]*) +(\))? *";
        auto match = matchAll(line, regex);
        if (!match)
        {
            return false;
        }

        m_key = match.front[1];
        splitAndSetValuesForLine(match.front[2]);

        // match[3] is ")" char if found
        if (!match.front[3] || m_values.length == 0)
        {
            m_isParsing = true;
        }

        return true;
    }

    void parseLine(string line)
    {
        auto regex = ctRegex!r"^([\w ]*)(\))? *$";
        auto match = matchAll(line, regex);
        if (!match)
        {
            // TODO: handle this at a higher level
            throw new Exception("");
        }

        splitAndSetValuesForLine(match.front[1]);

        if (match.front[2])
        {
            m_isParsing = false;
        }
    }

    void reset()
    {
        m_values = [];
        m_isParsing = false;
        m_key = "";
    }

private:
    void splitAndSetValuesForLine(string line)
    {
        m_values ~= line.split!isWhite;
    }

    string m_key;
    string[] m_values;
    bool m_isParsing = false;
}

