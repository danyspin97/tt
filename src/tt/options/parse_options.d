// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.options.parse_options;

import std.getopt : getopt;

import tt.options.common_options : CommonOptions;
import tt.options.services_options : ServicesOptions;

@safe:

class ParseOptions : ServicesOptions
{
public:
    @property bool quiet()
    {
        return m_quiet;
    }

    @property bool isFile()
    {
        return m_isFile;
    }

    this(in CommonOptions commonOptions, string[] args)
    {
        super(commonOptions, args);
    }

protected:
    override void parseArgs()
    {
        getopt(args, "quiet|q", &m_quiet, "file|f", &m_isFile);
        checkAtLeastNArgs(3);
        parseServices();
    }

private:
    bool m_isFile;
    bool m_quiet;
}
