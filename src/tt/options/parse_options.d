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

    this(CommonOptions commonOptions, ref string[] args)
    {
        super(commonOptions, args);
    }

protected:
    override void parseArgs()
    {
        helpInformation = getopt(args, "quiet|q", "Do not print services information on screen.", &m_quiet,
                "file|f", "Treat <services...> as file instead of services name.", &m_isFile);

        if (!showHelp)
        {
            checkAtLeastNArgs(3);
            parseServices();
        }
    }

    override const string usageText()
    {
        return "Usage:
  tt parse [options] <services...>
Options:";
    }

private:
    bool m_isFile;
    bool m_quiet;
}
