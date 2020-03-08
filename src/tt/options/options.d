// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

module tt.options.options;

import std.array : join;
import std.exception : enforce;
import std.format : format;

import tt.exception : InsufficientArgLengthException, UnexpectedArgumentException;
import tt.options.debug_level : DebugLevel;
import tt.options.common_options : CommonOptions;

@safe:

abstract class Options
{
public:
    @property bool showHelp()
    {
        return commonOptions.showHelp;
    }

    @property bool showVersion()
    {
        return commonOptions.showVersion;
    }

    @property DebugLevel debugLevel()
    {
        return commonOptions.debugLevel;
    }

    @property string subcommand()
    {
        return commonOptions.subcommand;
    }

    this(in CommonOptions commonOptions, ref string[] args)
    {
        this.commonOptions = commonOptions;
        this.args = args;
        parseArgs();
    }

protected:
    void checkAtLeastNArgs(in ushort n)
    {
        enforce!InsufficientArgLengthException(args.length >= n,
                format("Expected a servicename after the subcommand '%s'", subcommand));
    }

    void checkExactlyNArgs(in ushort n)
    {
        enforce!UnexpectedArgumentException(args.length == n,
                format("Didn't expect the additional arguments %s to subcommand '%s'",
                    args[3 .. $].join(), subcommand));
    }

    abstract void parseArgs();

    string[] args;
private:
    CommonOptions commonOptions;
}
