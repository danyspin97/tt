// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.signal;

import libtt.exception : SignalNotValidException;

@safe:

enum Signal
{
    SIGHUP = 1,
    SIGINT = 2,
    SIGQUIT = 3,
    SIGKILL = 9,
    SIGTERM = 15
}

Signal parseSignalFromString(string signal)
{
    switch (signal)
    {
    case "SIGHUP":
        return Signal.SIGHUP;
    case "SIGINT":
        return Signal.SIGINT;
    case "SIGQUIT":
        return Signal.SIGQUIT;
    case "SIGKILL":
        return Signal.SIGKILL;
    case "SIGTERM":
        return Signal.SIGTERM;
    default:
        throw new SignalNotValidException(signal ~ " is not a valid signal");
    }
}

unittest
{
    assert(parseSignalFromString("SIGHUP") == Signal.SIGHUP);
    assert(parseSignalFromString("SIGINT") == Signal.SIGINT);
    assert(parseSignalFromString("SIGQUIT") == Signal.SIGQUIT);
    assert(parseSignalFromString("SIGKILL") == Signal.SIGKILL);
    assert(parseSignalFromString("SIGTERM") == Signal.SIGTERM);
}

unittest
{
    import std.exception : assertThrown;

    assertThrown!SignalNotValidException(parseSignalFromString(""));
}
