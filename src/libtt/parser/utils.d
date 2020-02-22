// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.utils;

@safe:

import libtt.exception : BooleanParseException;

void setFailsIfNotEmpty(string* param, in string newValue)
{
    if (*param != "")
    {
        throw new Exception("");
    }

    *param = newValue;
}

@system unittest
{
    auto mystring = "oldValue";
    string* param = &mystring;
    import std.exception : assertThrown;

    assertThrown!Exception(setFailsIfNotEmpty(param, "newValue"));
}

@system unittest
{
    string mystring;
    string* param = &mystring;
    auto newValue = "newValue";
    setFailsIfNotEmpty(param, newValue);
    assert(*param == newValue);
}

bool parseBoolean(in string value)
{
    if (value == "yes")
    {
        return true;
    }
    if (value == "no")
    {
        return false;
    }

    auto msg = `A boolean value shall be either "yes" or "no";`;
    msg ~= `the value given was "` ~ value ~ `"`;
    throw new BooleanParseException(msg);
}

unittest
{
    assert(parseBoolean("yes"));
}

unittest
{
    assert(!parseBoolean("no"));
}

unittest
{
    import std.exception : assertThrown;

    assertThrown!Exception(parseBoolean("foo"));
}
