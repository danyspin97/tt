// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.string_sanitizer;

import std.exception : enforce;
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
        parseLine();
    }

private:
    void parseLine()
    {
        try
        {
            tryParseLine();
        }
        catch (FormatException e)
        {
        }
    }

    void tryParseLine()
    {
        scope(success) m_valid = true;
        scope(failure) m_valid = false;

        line.formattedRead!"%s=\"%s\""(m_key, m_value);
        m_key = strip(key);
        m_value = strip(value);
        enforce(m_key != "", "Key cannot be empty.");
        enforce(m_value != "", "Value cannot be empty.");
    }

    string line;
    string m_key;
    string m_value;
    bool m_valid;
}
