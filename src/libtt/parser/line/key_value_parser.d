// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.line.key_value_parser;

@safe:

import libtt.exception : WordNotValidException;
import libtt.parser.word : KeyParser, TokenParser, ValueParser, WhitespaceParser;

class KeyValueParser
{
public:
    @property string key() const
    {
        return m_key;
    }

    @property string value() const
    {
        return m_value;
    }

    @property bool lineValid() const
    {
        return m_valid;
    }

    this(in string line)
    {
        this.line = line;
        parseLine();
    }

private:
    void parseLine()
    {
        try
        {
            tryParseLine();
        }
        catch (WordNotValidException e)
        {
        }
    }

    void tryParseLine()
    {
        scope (success)
            m_valid = true;
        scope (failure)
            m_valid = false;

        auto keyParser = new KeyParser();
        line = keyParser.parse(line);
        line = (new TokenParser('=')).parse(line);
        auto valueParser = new ValueParser();
        line = valueParser.parse(line);
        (new WhitespaceParser).parse(line);

        m_key = keyParser.key;
        m_value = valueParser.value;
        m_valid = true;
    }

    unittest
    {
        auto parser = new KeyValueParser(`foo="bar"`);
        assert(parser.lineValid);
        assert(parser.key == "foo");
        assert(parser.value == "bar");
    }

    unittest
    {
        auto parser = new KeyValueParser(`foo=bar`);
        assert(parser.lineValid);
        assert(parser.key == "foo");
        assert(parser.value == "bar");
    }

    unittest
    {
        auto parser = new KeyValueParser(`foo= bar foobar`);
        assert(parser.lineValid);
        assert(parser.key == "foo");
        assert(parser.value == "bar foobar");
    }

    unittest
    {
        auto parser = new KeyValueParser(`foobar`);
        assert(!parser.lineValid);
    }

    string line;
    string m_key;
    string m_value;
    bool m_valid;
}
