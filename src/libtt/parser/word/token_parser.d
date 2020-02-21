// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.word.token_parser;

import std.uni : isWhite;

import libtt.exception : WordNotValidException;

class TokenParser
{
    this(char token)
    {
        this.token = token;
    }

    /**
     * Returns: The rest of the line.
     */
    string parse(string line)
    {
        auto i = getIndexOfTokenInLine(line);
        return line[i + 1 .. $];
    }

    unittest
    {
        auto parser = new TokenParser('=');
        assert(parser.parse("=") == "");
        assert(parser.parse(" = foo") == " foo");
    }

    unittest
    {
        auto parser = new TokenParser('=');
        import std.exception : assertThrown;

        assertThrown!WordNotValidException(parser.parse(""));
        assertThrown!WordNotValidException(parser.parse("foo bar"));
    }

protected:
    uint getIndexOfTokenInLine(string line)
    {
        auto i = 0;
        while (i != line.length)
        {
            if (line[i].isWhite)
            {
                i++;
                continue;
            }

            if (line[i] == token)
            {
                break;
            }

            throw new WordNotValidException("");
        }

        if (i == line.length)
        {
            throw new WordNotValidException("");
        }
        return i;
    }

    char token;
}
