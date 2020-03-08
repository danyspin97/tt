// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

module tt.options.options_factory;

import std.format : format;

import tt.exception;
import tt.options.common_options : CommonOptions;
import tt.options.options : Options;
import tt.options.parse_options : ParseOptions;

@safe:

/// Common CLI `Options` of `tt`
class OptionsFactory
{
    this(CommonOptions commonOptions, ref string[] args)
    {
        this.args = args;
        this.commonOptions = commonOptions;
        m_options = getOptionsForSubcommand();
    }

private:
    // Only used for unit testing
    this()
    {
    }

    Options getOptionsForSubcommand()
    {
        switch (commonOptions.subcommand)
        {
        case "parse":
            return new ParseOptions(commonOptions, args);
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
