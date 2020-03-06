// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

module tt.options;

import std.algorithm.mutation : remove;

import std.conv;
import std.exception;
import std.format;
import std.getopt;
import std.stdio;
import std.typecons : tuple;

import tt.exception;

immutable helpText = "
Usage:
  tt <subcommand> [OPTION...]

Interact with system services and query their status

Managing Services:
  edit-config [SERVICENAME]     - Edit the configuration of a service identified by SERVICENAME.
  disable [SERVICENAME] ...     - Disable one or more services identified by SERVICENAME.
  enable [SERVICENAME]  ...     - Enable one or more services identified by SERVICENAME.
  start [SERVICENAME] ...       - Start one or more services identified by SERVICENAME.
  stop [SERVICENAME] ...        - Stop one or more services identified by SERVICENAME.

Querying Service Information:
  query [SERVICENAME] ...       - Query the status of one or more services identified by SERVICENAME.
  show-config [SERVICENAME] ... - Show the configuration of one or more services identified by SERVICENAME.
  status                        - Show the status of the system.

Help Options:
  -h, --help         - Show help options.

Application Options:
  -v, --version      - Print program version.
  -d, --debug [0-4]  - Specify the debug level.";


/// The debug level to run the `tt` binary in
enum DebugLevel
{
    Error,
    Warn,
    Info,
    Debug,
    Trace,
}

/// CLI `Options` of `tt`
class Options
{
    /// Whether to show the version of tt
    bool showVersion;
    /// Whether to show the helpText of tt
    bool showHelp;
    /// The level of debugging to enable in tt
    DebugLevel debugLevel;
    /// The services we want to run the subcommand on
    string[] servicenames;
    /// What subcommand to run
    const string subcommand;
    // FIXME: This is only here so this() can be safe since we can't
    // take a pointer to a locale variable in @safe functions for getopt.
    private int debugLevelInt;

    this(ref string[] args) @safe
    {

        getopt(args, "help|h", &this.showHelp, "version|v",
                &this.showVersion, "debug|d", &this.debugLevelInt);

        try
        {
            this.debugLevel = debugLevelInt.to!DebugLevel;
        }
        catch (ConvException e)
        {
            throw new BadDebugLevelException(format("Unknown debug level %d", this.debugLevelInt));
        }

        if (showHelp || showVersion)
        {
            return;
        }

        enforce!InsufficientArgLengthException(args.length >= 2, "Please specify a subcommand.");

        this.subcommand = args[1];
        switch (this.subcommand)
        {
            // All subcommands which don't expect any additional arguments
        case "status":
            enforce!UnexpectedArgumentException(args.length == 2,
                    format("Didn't expect the additional arguments %s to subcommand 'status'",
                        args.remove(tuple(0, 2))));
            break;
            // All subcommands which expect exactly one service name
        case "edit-config":
            enforce!InsufficientArgLengthException(args.length >= 3,
                    "Expected a servicename after the subcommand 'edit-config'");
            enforce!UnexpectedArgumentException(args.length == 3,
                    format("Didn't expect the additional arguments %s to subcommand 'edit-config'",
                        args.remove(tuple(0, 3))));
            this.servicenames ~= args[2];
            break;
            // All subcommands which expected at least one service name
        case "disable":
        case "enable":
        case "start":
        case "stop":
        case "query":
        case "show-config":
            enforce!InsufficientArgLengthException(args.length >= 3,
                    format("Expected at least one servicename to the subcommand '%s'",
                        this.subcommand));

            // Start counting from 2 since 0 is the binary name and 1 the subcommand
            for (int i = 2; i < args.length; i++)
            {
                this.servicenames ~= args[i];
            }
            break;
        default:
            throw new UnknownArgumentException(format("Unknown subcommand %s", this.subcommand));
        }
    }
}

@safe unittest
{
    import std.array : array;

    auto args = array(["tt", "-h"]);
    assert(new Options(args).showHelp);
    assert(args.length == 1);
    args ~= "--version";
    assert(new Options(args).showVersion);
    assert(args.length == 1);

    assertThrown!InsufficientArgLengthException(new Options(args));

    args ~= "status";
    assert(new Options(args).subcommand == "status");
    args ~= "unexpectedarg";
    assertThrown!UnexpectedArgumentException(new Options(args));

    args = array(["tt", "edit-config", "test"]);
    auto optionsEC = new Options(args);
    assert(optionsEC.servicenames == array(["test"]));
    assert(optionsEC.subcommand == "edit-config");
    args ~= "unexpectedarg";
    assertThrown!UnexpectedArgumentException(new Options(args));

    args = array(["tt", "enable", "test", "test2"]);
    auto optionsEnable = new Options(args);
    assert(optionsEnable.servicenames == array(["test", "test2"]));
    assert(optionsEnable.subcommand == "enable");

    args = array(["tt", "disable", "test", "test2"]);
    auto optionsDisable = new Options(args);
    assert(optionsDisable.servicenames == array(["test", "test2"]));
    assert(optionsDisable.subcommand == "disable");

    args = array(["tt", "start", "test", "test2"]);
    auto optionsStart = new Options(args);
    assert(optionsStart.servicenames == array(["test", "test2"]));
    assert(optionsStart.subcommand == "start");

    args = array(["tt", "stop", "test", "test2"]);
    auto optionsStop = new Options(args);
    assert(optionsStop.servicenames == array(["test", "test2"]));
    assert(optionsStop.subcommand == "stop");

    args = array(["tt", "query", "test", "test2"]);
    auto optionsQuery = new Options(args);
    assert(optionsQuery.servicenames == array(["test", "test2"]));
    assert(optionsQuery.subcommand == "query");

    args = array(["tt", "show-config", "test", "test2"]);
    auto optionsSC = new Options(args);
    assert(optionsSC.servicenames == array(["test", "test2"]));
    assert(optionsSC.subcommand == "show-config");
}
