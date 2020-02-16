// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.key_value_parser;

import std.regex : ctRegex, matchAll;
import std.string : chomp, chompPrefix, strip;

class KeyValueParser
{
public:
    @property string key() { return m_key; }
    @property string value() { return m_value; }
    @property bool lineValid() { return m_valid; }

    this(string line)
    {
        this.line = line;
        parseLine();
    }

private:
    // Only used for unit testing
    this()
    {

    }

    void parseLine()
    {
        // Match
        // key = value
        // key = "value1 value2"
        auto keyValueRegex = ctRegex!(`^ *(\w+) *= *(\".+\"|\w+)$`);
        auto match = matchAll(line, keyValueRegex);
        if (!match)
        {
            m_valid = false;
            return;
        }
        m_key = match.captures[1];
        m_value = match.captures[2].chomp(`"`).chompPrefix(`"`).strip;
        m_valid = true;
    }

    unittest
    {
        auto parser = new KeyValueParser();
        parser.line = `foo="bar"`;
        parser.parseLine();
        assert(parser.lineValid);
        assert(parser.key == "foo");
        assert(parser.value == "bar");
    }

    unittest
    {
        auto parser = new KeyValueParser();
        parser.line = `foo=bar`;
        parser.parseLine();
        assert(parser.lineValid);
        assert(parser.key == "foo");
        assert(parser.value == "bar");
    }

    unittest
    {
        auto parser = new KeyValueParser();
        parser.line = `foobar`;
        parser.parseLine();
        assert(!parser.lineValid);
    }

    string line;
    string m_key;
    string m_value;
    bool m_valid;
}
