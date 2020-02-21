// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.service.bundle_director;

import libtt.parser.section : BundleOptionsBuilder, MainSection,
    MainSectionBuilder, SectionBuilder;
import libtt.parser.service.service_director : ServiceDirector;
import libtt.data : Bundle, BundleOptions, Service;

class BundleDirector : ServiceDirector
{
public:
    override Service instanceService(string path)
    {
        return new Bundle(mainSection.name, mainSection.polishName,
                mainSection.description, mainSection.path, options);
    }

    override SectionBuilder getBuilderForSection(string section)
    {
        switch (section)
        {
        case "main":
            return new MainSectionBuilder(mainSection);
        case "options":
            return new BundleOptionsBuilder(options);
        default:
            auto msg = `Section "` ~ section ~ `" is not supported.`;
            throw new Exception(msg);
        }
    }

private:
    MainSection mainSection;
    BundleOptions options = new BundleOptions();
}
