// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.options.common_options_parser;

import std.conv : ConvException, to;
import std.exception : enforce;
import std.format : format;
import std.getopt;

import tt.exception : BadDebugLevelException, InsufficientArgLengthException;
import tt.options.common_options : CommonOptions;
import tt.options.debug_level : DebugLevel;

@safe:

class CommonOptionsParser
{
    @property ref string[] args()
    {
        return m_args;
    }

    this(CommonOptions* options, ref string[] args)
    {
        this.m_args = args;
        assert(options);
        this.options = options;
    }

    void parseCommonOptions()
    {
        auto getoptResult = getopt(args, std.getopt.config.passThrough,
                "version|v", "Print version and exit.", &options.showVersion,
                "debug|d", "Set the debug level from 0 (Error) to 4 (Trace).", &debugLevelInt);

        options.getoptOptions = getoptResult.options;
        options.showHelp = getoptResult.helpWanted;

        convertDebugLevel();
    }

    unittest
    {
        auto parser = new CommonOptionsParser();
        parser.options = new CommonOptions();
        parser.args = ["tt", "-h"];
        parser.parseCommonOptions();
        assert(parser.options.showHelp);
        assert(parser.args.length == 1);
    }

    unittest
    {
        auto parser = new CommonOptionsParser();
        parser.options = new CommonOptions();
        parser.args = ["tt", "--version"];
        parser.parseCommonOptions();
        assert(parser.options.showVersion);
        assert(parser.args.length == 1);
    }

    unittest
    {
        auto parser = new CommonOptionsParser();
        parser.options = new CommonOptions();
        parser.args = ["tt", "--version", "--foo"];
        parser.parseCommonOptions();
        assert(parser.options.showVersion);
        assert(parser.args.length == 2);
        assert(parser.args == ["tt", "--foo"]);
    }

    void checkAndSetSubcommand()
    {
        if (!options.showHelp && !options.showVersion)
        {
            enforce!InsufficientArgLengthException(args.length >= 2, "Please specify a subcommand.");
        }

        if (options.showHelp && args.length == 1)
        {
            options.subcommand = "help";
            return;
        }

        if (options.showVersion)
        {
            options.subcommand = "version";
            return;
        }

        options.subcommand = args[1];
    }

    unittest
    {
        auto parser = new CommonOptionsParser();
        parser.options = new CommonOptions();
        parser.args = ["tt", "status"];
        parser.checkAndSetSubcommand();
        assert(parser.options.subcommand == "status");
    }

    unittest
    {
        auto parser = new CommonOptionsParser();
        parser.options = new CommonOptions();
        parser.args = ["tt"];
        import std.exception : assertThrown;

        assertThrown!InsufficientArgLengthException(parser.checkAndSetSubcommand());
    }

private:
    void convertDebugLevel()
    {
        try
        {
            options.debugLevel = debugLevelInt.to!DebugLevel;
        }
        catch (ConvException e)
        {
            throw new BadDebugLevelException(format("Unknown debug level %d", debugLevelInt));
        }
    }

    unittest
    {
        auto parser = new CommonOptionsParser();
        parser.options = new CommonOptions();
        const auto map = [
            0 : DebugLevel.Error, 1 : DebugLevel.Warn, 2 : DebugLevel.Info,
            3 : DebugLevel.Debug, 4 : DebugLevel.Trace
        ];
        static foreach (i; 0 .. map.length)
        {
            parser.debugLevelInt = i;
            parser.convertDebugLevel();
            assert(parser.options.debugLevel == map[i]);
        }
    }

    // Only used for unittest
    this()
    {
    }

    CommonOptions* options;
    string[] m_args;

    uint debugLevelInt;
}
