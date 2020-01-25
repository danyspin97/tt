// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.multiline_code_parser;

import std.regex : matchFirst;
import std.string : strip;

class MultilineCodeParser
{
public:
    @property bool isParsing() { return m_isParsing; }

    this()
    {

    }

    bool startParsing(string line)
    {
        if (isParsing() || code != "")
        {
            throw new Exception("");
        }
        if (matchFirst(line, r"execute ?= ?("))
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

        code ~= line;
    }

private:
    string code;
    bool m_isParsing = false;
    uint parenthesesOpened;
}
