// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.dependency_reader;

import libtt.services.bundle : Bundle;
import libtt.services.bundle_options : BundleOptions;
import libtt.services.longrun : Longrun;
import libtt.services.longrun_options : LongrunOptions;
import libtt.services.oneshot : Oneshot;
import libtt.services.oneshot_options : OneshotOptions;
import libtt.services.service : Service;

class DependencyReader
{
public:
    string[] getDependenciesForService(Service service)
    {
        if (auto bundle = cast(Bundle)service)
        {
            return getDependenciesForBundle(bundle);
        }
        if (auto longrun = cast(Longrun)service)
        {
            return getDependenciesForLongrun(longrun);
        }
        if (auto oneshot = cast(Oneshot)service)
        {
            return getDependenciesForOneshot(oneshot);
        }

        throw new Exception("");
    }

    string[] getDependenciesForBundle(Bundle bundle)
    {
        auto options = cast(BundleOptions)bundle.options;
        return options.contents;
    }

    string[] getDependenciesForLongrun(Longrun longrun)
    {
        auto options = cast(LongrunOptions)longrun.options;
        return options.dependencies;
    }

    string[] getDependenciesForOneshot(Oneshot oneshot)
    {
        auto options = cast(OneshotOptions)oneshot.options;
        return options.dependencies;
    }
}

