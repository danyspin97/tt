// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.utils;

void setFailsIfNotEmpty(string* param, string newValue)
{
    if (*param != "")
    {
        throw new Exception("");
    }

    *param = newValue;
}

unittest
{
    auto mystring = "oldValue";
    string* param = &mystring;
    import std.exception : assertThrown;
    assertThrown!Exception(setFailsIfNotEmpty(param, "newValue"));
}

unittest
{
    string mystring;
    string* param = &mystring;
    auto newValue = "newValue";
    setFailsIfNotEmpty(param, newValue);
    assert(*param == newValue);
}

