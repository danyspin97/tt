// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.word.until_token_parser;

@safe:

import std.uni : isWhite;

import libtt.exception : WordNotValidException;

class UntilTokenParser
{
    @property string parsedLine() nothrow
    {
        return value;
    }

    this(char token) nothrow
    {
        this.token = token;
    }

    string parseUntilToken(string line)
    {
        auto i = 0;
        while (i != line.length)
        {
            if (line[i] == token)
            {
                break;
            }

            i++;
        }

        if (i == line.length)
        {
            throw new WordNotValidException("");
        }

        value = line[0 .. i];
        return line[i + 1 .. $];
    }

    unittest
    {
        auto parser = new UntilTokenParser(')');
        assert(parser.parseUntilToken("foo ) bar") == " bar");
        assert(parser.parsedLine == "foo ");
    }

    unittest
    {
        auto parser = new UntilTokenParser(')');
        import std.exception : assertThrown;

        assertThrown!WordNotValidException(parser.parseUntilToken("foo"));
    }

private:
    char token;
    string value;
}
