// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.services.bundle;

import libtt.services.service : Service;
import libtt.services.bundle_options : BundleOptions;


class Bundle : Service
{
public:
    @property const(Service[]) contents() { return contents; }

    this (
        string name,
        string polishName,
        string description,
        string path,
        ref BundleOptions options,
        Service[] contents
    ) {
        super(
            name,
            polishName,
            description,
            path,
            options
        );

        m_contents = contents;
    }

private:
    Service[] m_contents;
}
