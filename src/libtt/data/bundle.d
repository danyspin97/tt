// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.bundle;

import libtt.data.service : Service;
import libtt.data.bundle_options : BundleOptions;

class Bundle : Service
{
public:
    this(string name, string polishName, string description, string path,
            ref BundleOptions options,)
    {
        super(name, polishName, description, path, options);
    }
}
