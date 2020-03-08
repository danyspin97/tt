// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.action.parse_action;

import tt.action.action : Action;
import tt.options : ParseOptions;

@system:

class ParseAction : Action
{
public:
    this(ParseOptions options) @safe
    {
        this.options = options;
    }

    override void execute()
    {
    }

private:
    ParseOptions options;
}
