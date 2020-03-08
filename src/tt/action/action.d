// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.action.action;

nothrow:

abstract class Action
{
public:
    abstract void execute();
}
