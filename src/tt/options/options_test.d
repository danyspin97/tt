// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.options.options_test;

import tt.options.common_options : CommonOptions;
import tt.options.options : Options;
import tt.options.debug_level : DebugLevel;

private class OptionsTest : Options
{
    this(in CommonOptions commonOptions)
    {
        super(commonOptions);
    }

    unittest
    {
        CommonOptions commonOptions;
        commonOptions.showHelp = true;
        commonOptions.showVersion = true;
        commonOptions.debugLevel = DebugLevel.Error;
        commonOptions.subcommand = "test_command";
        auto optionsTest = new OptionsTest(commonOptions);

        assert(optionsTest.showHelp);
        assert(optionsTest.showVersion);
        assert(optionsTest.debugLevel == DebugLevel.Error);
        assert(optionsTest.subcommand == "test_command");
    }

    unittest
    {
        import std.exception : assertThrown;
        import tt.exception : InsufficientArgLengthException, UnexpectedArgumentException;

        CommonOptions commonOptions;
        auto optionsTest = new OptionsTest(commonOptions);

        auto args = ["tt", "edit-config"];
        assertThrown!InsufficientArgLengthException(optionsTest.checkAtLeastNArgs(3, args));

        args ~= ["test-service"];
        optionsTest.checkAtLeastNArgs(3, args);

        args ~= ["unexpectedarg"];
        optionsTest.checkAtLeastNArgs(3, args);
        assertThrown!UnexpectedArgumentException(optionsTest.checkExactlyNArgs(3, args));
    }

}
