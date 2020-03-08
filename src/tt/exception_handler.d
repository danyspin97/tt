// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.exception_handler;

import std.stdio : writeln;
import std.string : splitLines;

import tt.options : DebugLevel;

class ExceptionHandler
{
public:
    static void writeExceptionMessage(T)(T e, DebugLevel debugLevel)
    {
        switch (debugLevel)
        {
        case DebugLevel.Error:
            goto case;
        case DebugLevel.Warn:
            goto case;
        case DebugLevel.Info:
            writeln(e.msg);
            break;
        case DebugLevel.Debug:
            writeln(e.toString().splitLines()[0]);
            break;
        case DebugLevel.Trace:
            throw e;
        default:
            break;
        }
    }
}
