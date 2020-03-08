// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.action.action_factory;

import std.format : format;

import tt.action.action : Action;
import tt.action.parse_action : ParseAction;
import tt.exception : UnknownArgumentException;
import tt.options : Options, ParseOptions;

@safe:

class ActionFactory
{
public:
    static Action getActionForOptions(Options options)
    {
        switch (options.subcommand)
        {
        case "parse":
            return new ParseAction(cast(ParseOptions) options);
        default:
            throw new UnknownArgumentException(format("Unknown subcommand %s", options.subcommand));
        }
    }
}
