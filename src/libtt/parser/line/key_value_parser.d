// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.line.key_value_parser;

@safe:

import libtt.exception : LineNotValidWhileParsingException, WordNotValidException;
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

    this(in string line, bool throwOnErrors = false)
    {
        this.line = line;
        this.throwOnErrors = throwOnErrors;
        keyParser = new KeyParser();
        valueParser = new ValueParser();
        tokenParser = new TokenParser('=');
        whitespaceParser = new WhitespaceParser();
        parseLine();
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

    unittest
    {
        import std.exception : assertThrown;

        assertThrown!LineNotValidWhileParsingException(new KeyValueParser("foo", true));
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
            if (throwOnErrors)
            {
                const auto msg = e.msg ~ ` in line "` ~ line ~ `"`;
                throw new LineNotValidWhileParsingException(msg);
            }
        }
    }

    void tryParseLine()
    {
        scope (success)
            m_valid = true;
        scope (failure)
            m_valid = false;

        line = keyParser.parse(line);
        m_key = keyParser.key;

        line = tokenParser.parse(line);

        line = valueParser.parse(line);
        m_value = valueParser.value;

        whitespaceParser.parse(line);

        m_valid = true;
    }

    string line;
    string m_key;
    string m_value;
    bool m_valid;
    bool throwOnErrors;

    KeyParser keyParser;
    TokenParser tokenParser;
    ValueParser valueParser;
    WhitespaceParser whitespaceParser;
}
