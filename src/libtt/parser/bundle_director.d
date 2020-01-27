// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.bundle_director;

import libtt.parser.bundle_options_builder : BundleOptionsBuilder;
import libtt.parser.main_section : MainSection;
import libtt.parser.main_section_builder : MainSectionBuilder;
import libtt.parser.section_builder : SectionBuilder;
import libtt.parser.service_director : ServiceDirector;
import libtt.services.bundle : Bundle;
import libtt.services.bundle_options : BundleOptions;
import libtt.services.service : Service;

class BundleDirector : ServiceDirector
{
public:
    override Service instanceService(string path)
    {
        return new Bundle(
            mainSection.name,
            mainSection.polishName,
            mainSection.description,
            mainSection.path,
            options
        );
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

