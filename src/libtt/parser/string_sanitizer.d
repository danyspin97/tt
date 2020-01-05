// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.string_sanitizer;

import std.format : FormatException, formattedRead;
import std.string : strip;

class StringSanitizer
{
public:
    @property string key() { return m_key; }
    @property string value() { return m_value; }
    @property bool lineValid() { return m_valid; }

    this(string line)
    {
        this.line = line;
        parseLineAndValidate();
    }

private:
    void parseLineAndValidate()
    {
        try
        {
            parseLine();
            m_valid = true;
        }
        catch (FormatException e)
        {
            m_valid = false;
        }
    }

    void parseLine()
    {
        line.formattedRead!"%s=\"%s\""(m_key, m_value);
        m_key = strip(key);
        m_value = strip(value);
    }

    string line;
    string m_key;
    string m_value;
    bool m_valid;
}
