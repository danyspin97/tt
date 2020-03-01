// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.word.key_parser;

@safe:

import std.exception : enforce;
import std.uni : isWhite;

import libtt.exception : WordNotValidException;

class KeyParser
{
    @property string key()
    {
        return m_key;
    }

    /**
     * Returns: The rest of the line.
     */
    string parse(string line)
    {
        auto start = -1;
        auto i = 0;
        while (i != line.length)
        {
            if (line[i].isWhite)
            {
                i++;
                continue;
            }

            start = i;
            break;
        }

        while (i != line.length)
        {
            if (line[i].isWhite || line[i] == '=')
            {
                break;
            }

            i++;
        }

        enforce!WordNotValidException(start != i, "Key cannot be empty");

        m_key = line[start .. i];
        return line[i .. $];
    }

    unittest
    {
        auto parser = new KeyParser();
        assert(parser.parse(" foo") == "");
        assert(parser.key == "foo");
    }

    unittest
    {
        auto parser = new KeyParser();
        assert(parser.parse("foo=bar") == "=bar");
        assert(parser.key == "foo");
    }

    unittest
    {
        auto parser = new KeyParser();
        assert(parser.parse(" foo bar") == " bar");
        assert(parser.key == "foo");
    }

    unittest
    {
        auto parser = new KeyParser();
        import std.exception : assertThrown;
        assertThrown!WordNotValidException(parser.parse("=bar"));
        assertThrown!WordNotValidException(parser.parse(" =bar"));
    }

private:
    string m_key;
}
