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

#include "tt/data/environment.hpp"                       // for Environment
#include "tt/data/oneshot_options.hpp"                   // for OneshotOptions
#include "tt/data/service.hpp"                           // for Service
#include "tt/parser/section/environment_builder.hpp"     // for Environment...
#include "tt/parser/section/main_script_builder.hpp"     // for MainScriptB...
#include "tt/parser/section/main_section.hpp"            // for MainSection
#include "tt/parser/section/main_section_builder.hpp"    // for MainSection...
#include "tt/parser/section/oneshot_options_builder.hpp" // for OneshotOpti...
#include "tt/parser/section/script_builder.hpp"          // for ScriptBuilder
#include "tt/parser/service/service_director.hpp"        // for ServiceDire...

namespace tt {
class SectionBuilder;

class OneshotDirector : public ServiceDirector {
public:
    OneshotDirector() = default;

    auto InstanceService(std::string &&path)
        -> tl::expected<Service, ParserError> override;
    auto GetBuilderForSection(const std::string &section)
        -> tl::expected<SectionBuilder *, ParserError> override;

private:
    MainSectionBuilder main_section_builder_;
    MainScriptBuilder start_script_builder_{"start"};
    ScriptBuilder stop_script_builder_{"stop"};
    EnvironmentBuilder env_section_builder_;
    OneshotOptionsBuilder options_builder_;
};

} // namespace tt
