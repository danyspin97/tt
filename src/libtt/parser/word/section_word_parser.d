// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.word.section_word_parser;

import std.uni : isWhite;

import libtt.exception : WordNotValidException;

class SectionWordParser
{
    @property string section()
    {
        return m_section;
    }

    /**
     * Returns: The rest of the line.
     */
    string parse(string line)
    {
        auto start = -1, end = -1;
        bool bracketsOpened = false;
        auto i = 0;
        while (i != line.length)
        {
            if (line[i].isWhite)
            {
                i++;
                continue;
            }

            if (line[i] == '[')
            {
                bracketsOpened = true;
                i++;
                continue;
            }

            if (!bracketsOpened)
            {
                throw new WordNotValidException("");
            }

            start = i;
            break;
        }

        while (i != line.length)
        {
            if (end == -1 && line[i].isWhite)
            {
                end = i;
                i++;
                continue;
            }

            if (line[i] == ']')
            {
                if (end == -1)
                {
                    end = i;
                }

                i++;
                break;
            }

            i++;
        }

        if (i == line.length && end == -1)
        {
            throw new WordNotValidException("");
        }

        m_section = line[start .. end];
        return line[i .. $];
    }

    unittest
    {
        auto parser = new SectionWordParser();
        assert(parser.parse("[foo] ") == " ");
        assert(parser.section == "foo");
    }

    unittest
    {
        auto parser = new SectionWordParser();
        assert(parser.parse("[ foo ]") == "");
        assert(parser.section == "foo");
    }

    unittest
    {
        auto parser = new SectionWordParser();
        import std.exception : assertThrown;

        assertThrown!WordNotValidException(parser.parse("foo"));
        assertThrown!WordNotValidException(parser.parse("[foo"));
    }

private:
    string m_section;
}
