// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

module tt.options.options_factory;

import std.exception : enforce;
import std.format : format;
import std.getopt : getopt;

import tt.exception;
import tt.options.common_options : CommonOptions;
import tt.options.options : Options;

@safe:

/// Common CLI `Options` of `tt`
class OptionsFactory
{
    this(ref string[] args)
    {
        this.args = args;
        parseCommonOptions();
        m_options = getOptionsForSubcommand();
    }

private:
    version (unittest)
    {
        this()
        {
        }
    }

    void parseCommonOptions()
    {
        getopt(args, "help|h", &commonOptions.showHelp, "version|v",
                &commonOptions.showVersion, "debug|d", &commonOptions.debugLevel);

        if (!commonOptions.showHelp && !commonOptions.showVersion)
        {
            enforce!InsufficientArgLengthException(args.length >= 2, "Please specify a subcommand.");
        }

        if (commonOptions.showHelp)
        {
            commonOptions.subcommand = "help";
            return;
        }

        if (commonOptions.showVersion)
        {
            commonOptions.subcommand = "version";
            return;
        }

        commonOptions.subcommand = args[1];
    }

    unittest
    {
        import std.array : array;

        auto factory = new OptionsFactory();
        CommonOptions* commonOptions = &factory.commonOptions;
        factory.args = array(["tt", "-h"]);
        factory.parseCommonOptions();

        assert(commonOptions.showHelp);
        assert(factory.args.length == 1);
        factory.args ~= "--version";
        factory.parseCommonOptions();
        assert(commonOptions.showVersion);
        assert(factory.args.length == 1);
    }

    unittest
    {
        import std.exception : assertThrown;

        auto factory = new OptionsFactory();
        CommonOptions* commonOptions = &factory.commonOptions;
        factory.args = ["tt"];
        assertThrown!InsufficientArgLengthException(factory.parseCommonOptions());

        factory.args ~= "status";
        factory.parseCommonOptions();
        assert(commonOptions.subcommand == "status");
    }

    Options getOptionsForSubcommand()
    {
        switch (commonOptions.subcommand)
        {
        default:
            throw new UnknownArgumentException(format("Unknown subcommand %s",
                    commonOptions.subcommand));
        }
    }

    unittest
    {
        import std.exception : assertThrown;

        auto factory = new OptionsFactory();
        factory.commonOptions.subcommand = "unexpectedarg";
        assertThrown!UnknownArgumentException(factory.getOptionsForSubcommand());
    }

    CommonOptions commonOptions;
    Options m_options;
    string[] args;
}
