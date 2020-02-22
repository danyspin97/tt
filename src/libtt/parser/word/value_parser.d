// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.word.value_parser;

@safe:
nothrow:

import std.uni : isWhite;

class ValueParser
{
    @property string value() nothrow
    {
        return m_value;
    }

    /**
     * Returns: The rest of the line.
     */
    string parse(string line)
    {
        auto start = -1;
        bool isQuoted = false;

        auto i = 0;
        while (i != line.length)
        {
            if (line[i].isWhite)
            {
                i++;
                continue;
            }

            if (line[i] == '"')
            {
                isQuoted = true;
                i++;
            }

            start = i;
            break;
        }

        i++;
        while (i != line.length)
        {
            if (!isQuoted && line[i].isWhite)
            {
                break;
            }

            if (isQuoted && line[i] == '"')
            {
                break;
            }

            i++;
        }

        m_value = line[start .. i];
        if (isQuoted)
        {
            i++;
        }
        return line[i .. $];
    }

    unittest
    {
        auto parser = new ValueParser();
        assert(parser.parse("foo  ") == "  ");
        assert(parser.value == "foo");
    }

    unittest
    {
        auto parser = new ValueParser();
        assert(parser.parse(` " foo " bar`) == " bar");
        assert(parser.value == " foo ");
    }

private:
    string m_value;
}
