// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

module tt.main;

import std.exception : collectException;

import tt.action : Action, ActionFactory;
import tt.exception_handler : ExceptionHandler;
import tt.options : CommonOptions, CommonOptionsParser, DebugLevel, OptionsFactory;

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

Testing services:
  parse [SERVICENAME] ...       - Parse one or more services, check if they are valid and print their informations.

Help Options:
  -h, --help         - Show help options.

Application Options:
  -v, --version      - Print program version.
  -d, --debug [0-4]  - Specify the debug level.";

void executeTT(string[] args, ref DebugLevel debugLevel)
{
    CommonOptions commonOpt;
    auto commonOptParser = new CommonOptionsParser(&commonOpt, args);
    commonOptParser.parseCommonOptions();
    debugLevel = commonOpt.debugLevel;
    commonOptParser.checkAndSetSubcommand();
    auto optionsFactory = new OptionsFactory(commonOpt, args);
    auto opt = optionsFactory.options;
    debugLevel = opt.debugLevel;
    auto action = ActionFactory.getActionForOptions(opt);
    action.execute;
}

int main(string[] args)
{
    DebugLevel debugLevel;
    auto e = collectException(executeTT(args, debugLevel));
    if (e)
    {
        ExceptionHandler.writeExceptionMessage(e, debugLevel);
        return 1;
    }

    return 0;
}
