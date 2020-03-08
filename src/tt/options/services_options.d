// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module tt.options.services_options;

import tt.options.options : Options;
import tt.options.common_options : CommonOptions;

@safe:
nothrow:

abstract class ServicesOptions : Options
{
public:
    @property string[] services()
    {
        return m_services;
    }

    this(in CommonOptions commonOptions, string[] args)
    {
        super(commonOptions, args);
    }

protected:
    void parseServices()
    {
        m_services = args[2 .. $];
    }

private:
    string[] m_services;
}
