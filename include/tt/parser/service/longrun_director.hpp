/*
 * Copyright (c) 2020 Danilo Spinella <oss@danyspin97.org>.
 *
 * This file is part of tt
 * (see https://github.com/danyspin97/tt).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <string> // for string

#include "tt/data/environment.hpp"                         // for Environment
#include "tt/data/longrun_options.hpp"                     // for LongrunOp...
#include "tt/data/service.hpp"                             // for Service
#include "tt/parser/section/environment_builder.hpp"       // for Environme...
#include "tt/parser/section/long_lived_script_builder.hpp" // for LongLived...
#include "tt/parser/section/longrun_options_builder.hpp"   // for LongrunOp...
#include "tt/parser/section/main_section.hpp"              // for MainSection
#include "tt/parser/section/main_section_builder.hpp"      // for MainSecti...
#include "tt/parser/section/script_builder.hpp"            // for ScriptBui...
#include "tt/parser/service/service_director.hpp"          // for ServiceDi...

namespace tt {
class SectionBuilder;

class LongrunDirector : public ServiceDirector {
public:
    LongrunDirector();
    ~LongrunDirector() override = default;

    auto InstanceService(std::string &&path) -> Service override;
    auto GetBuilderForSection(const std::string &section)
        -> SectionBuilder * override;

private:
    MainSection main_section_;
    Environment environment_;
    LongrunOptions options_;
    MainSectionBuilder main_section_builder_;
    LongLivedScriptBuilder run_script_builder_;
    ScriptBuilder finish_script_builder_;
    EnvironmentBuilder env_section_builder_;
    LongrunOptionsBuilder options_builder_;
};

} // namespace tt
