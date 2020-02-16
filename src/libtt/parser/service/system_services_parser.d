// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.system_services_parser;

import libtt.dirs : dirs;
import libtt.parser.service.services_parser : ServicesParser;

class SystemServicesParser : ServicesParser
{
public:
    this()
    {
        paths = [ dirs.service, dirs.adminService ];
        suffix = ".system";
    }
}
