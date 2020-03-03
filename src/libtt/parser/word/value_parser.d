// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.word.value_parser;

@safe:

import std.exception : enforce;
import std.string : strip, stripRight;
import std.uni : isWhite;

import libtt.exception : WordNotValidException;

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

        while (isQuoted && i != line.length)
        {
            if (isQuoted && line[i] == '"')
            {
                break;
            }

            i++;
        }

        string ret;
        if (isQuoted)
        {
            enforce!WordNotValidException(i < line.length, "Double quotes have not been closed");
            m_value = line[start .. i];
            ret = line[i + 1 .. $];
        }
        else
        {
            enforce!WordNotValidException(start < line.length, "No value found");
            m_value = line[start .. $].stripRight;
            ret = "";
        }
        enforce!WordNotValidException(m_value.strip != "", "Value cannot be empty");
        return ret;
    }

    unittest
    {
        auto parser = new ValueParser();
        assert(parser.parse("foo  ") == "");
        assert(parser.value == "foo");
    }

    unittest
    {
        auto parser = new ValueParser();
        assert(parser.parse(` " foo " bar`) == " bar");
        assert(parser.value == " foo ");
    }

    unittest
    {
        auto parser = new ValueParser();
        import std.exception : assertThrown;

        assertThrown!WordNotValidException(parser.parse(`""`));
        assertThrown!WordNotValidException(parser.parse(`" "`));
        assertThrown!WordNotValidException(parser.parse(`    `));
    }

private:
    string m_value;
}
