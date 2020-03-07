// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

@safe:
nothrow:

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

void main()
{

}
