// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.bundle_options;

@safe:
nothrow:

import libtt.data.service_options : ServiceOptions;

class BundleOptions : ServiceOptions
{
public:
    @property string[] contents()
    {
        return m_contents;
    }

    @property contents(string[] contents)
    {
        m_contents = contents;
    }

private:
    string[] m_contents;
}
