// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.dependency_reader;

import libtt.data : Bundle, BundleOptions, Longrun, LongrunOptions, Oneshot,
    OneshotOptions, Service;

class DependencyReader
{
public:
    static const(string[]) getDependenciesForService(Service service)
    {
        if (auto bundle = cast(Bundle) service)
        {
            return getDependenciesForBundle(bundle);
        }
        if (auto longrun = cast(Longrun) service)
        {
            return getDependenciesForLongrun(longrun);
        }
        if (auto oneshot = cast(Oneshot) service)
        {
            return getDependenciesForOneshot(oneshot);
        }

        throw new Exception("");
    }

private:
    static const(string[]) getDependenciesForBundle(Bundle bundle)
    {
        auto options = cast(BundleOptions) bundle.options;
        return options.contents;
    }

    static const(string[]) getDependenciesForLongrun(Longrun longrun)
    {
        auto options = cast(LongrunOptions) longrun.options;
        return options.dependencies;
    }

    static const(string[]) getDependenciesForOneshot(Oneshot oneshot)
    {
        auto options = cast(OneshotOptions) oneshot.options;
        return options.dependencies;
    }
}
