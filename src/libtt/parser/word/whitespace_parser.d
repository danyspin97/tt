// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.word.whitespace_parser;

@safe:

import std.uni : isWhite;

import libtt.exception : WordNotValidException;

class WhitespaceParser
{
    /**
     * Returns: The rest of the line.
     */
    string parse(in string line) const
    {
        auto i = 0;
        while (i != line.length)
        {
            if (!line[i].isWhite)
            {
                break;
            }

            i++;
        }

        if (i != line.length)
        {
            throw new WordNotValidException("");
        }

        return line[i .. $];
    }

    unittest
    {
        auto parser = new WhitespaceParser();
        assert(parser.parse("    ") == "");
    }

    unittest
    {
        auto parser = new WhitespaceParser();
        import std.exception : assertThrown;

        assertThrown!WordNotValidException(parser.parse(" foo"));
    }
}
