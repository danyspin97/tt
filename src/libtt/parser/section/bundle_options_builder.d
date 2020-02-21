// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.section.bundle_options_builder;

import libtt.parser.section.section_builder : SectionBuilder;
import libtt.data : BundleOptions;

class BundleOptionsBuilder : SectionBuilder
{
public:
    this(BundleOptions bundleOptions)
    {
        this.bundleOptions = bundleOptions;
    }

    override void parseLine(string line)
    {

    }

    override void endParsing()
    {

    }

private:
    BundleOptions bundleOptions;
}
