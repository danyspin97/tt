/*
 * Copyright (c) 2020 Danilo Spinella <danyspin97@protonmail.com>.
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

#include <optional>

#include "tt/data/environment.hpp"
#include "tt/data/oneshot_options.hpp"
#include "tt/data/script.hpp"
#include "tt/parser/section/environment_builder.hpp"
#include "tt/parser/section/main_section_builder.hpp"
#include "tt/parser/section/oneshot_options_builder.hpp"
#include "tt/parser/section/script_builder.hpp"
#include "tt/parser/service/service_director.hpp"

namespace tt {

class OneshotDirector : public ServiceDirector {
public:
    OneshotDirector();
    ~OneshotDirector() override = default;

    std::shared_ptr<Service> InstanceService(const std::string &path) override;
    SectionBuilder *GetBuilderForSection(const std::string &section) override;

private:
    MainSection main_section_;
    Environment environment_;
    OneshotOptions options_;
    MainSectionBuilder main_section_builder_;
    ScriptBuilder start_script_builder_;
    ScriptBuilder stop_script_builder_;
    EnvironmentBuilder env_section_builder_;
    OneshotOptionsBuilder options_builder_;
};

} // namespace tt
