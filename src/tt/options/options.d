// Copyright 2020 Rasmus Thomsen <oss@cogitri.dev>
// Distributed under the terms of the GNU General Public License v2

module tt.options.options;

import tt.options.debug_level : DebugLevel;
import tt.options.common_options : CommonOptions;

class Options
{
public:
    @property bool showHelp() { return commonOptions.showHelp; }
    @property bool showVersion() { return commonOptions.showVersion; }
    @property DebugLevel debugLevel() { return commonOptions.debugLevel; }
    @property string subcommand() { return commonOptions.subcommand; }

    this(const CommonOptions commonOptions)
    {
        this.commonOptions = commonOptions;
    }

private:
    const CommonOptions commonOptions;
}

