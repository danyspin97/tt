// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.format;

import std.ascii : newline;

string formatAssertMessage(string test, string expected, string found)
{
    return "Expected " ~ test ~ ":" ~ newline ~ expected ~ newline ~ "Found:" ~ newline ~ found;
}
