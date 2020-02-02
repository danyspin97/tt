// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.multiline_code_parser;

import std.regex : ctRegex, matchFirst;
import std.string : strip;

class MultilineCodeParser
{
public:
    @property bool isParsing() { return m_isParsing; }
    @property string code()
    {
        if (isParsing())
        {
            // TODO: should this be caught by a ServiceDirector class?
            throw new Exception("");
        }

        return m_code;
    }

    this()
    {

    }

    bool startParsing(string line)
    {
        if (isParsing() || m_code != "")
        {
            throw new Exception("");
        }
        auto regex = ctRegex!r"^ *execute ?= ?\( *$";
        if (matchFirst(line, regex))
        {
            m_isParsing = true;
            return true;
        }

        return false;
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

private:
    string m_code;
    bool m_isParsing = false;
}

