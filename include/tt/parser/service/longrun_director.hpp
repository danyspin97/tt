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

#include "tt/data/environment.hpp"
#include "tt/data/logger_script.hpp"
#include "tt/data/longrun_options.hpp"
#include "tt/data/script.hpp"
#include "tt/parser/section/environment_builder.hpp"
#include "tt/parser/section/logger_script_builder.hpp"
#include "tt/parser/section/longrun_options_builder.hpp"
#include "tt/parser/section/main_section_builder.hpp"
#include "tt/parser/service/service_director.hpp"

namespace tt {

class LongrunDirector : public ServiceDirector {
public:
    LongrunDirector();
    ~LongrunDirector() = default;

    std::shared_ptr<Service> InstanceService(const std::string &path);
    SectionBuilder *GetBuilderForSection(const std::string &section);

private:
    MainSectionBuilder main_section_builder_;
    ScriptBuilder run_script_builder_;
    ScriptBuilder finish_script_builder_;
    LoggerScriptBuilder logger_script_builder_;
    EnvironmentBuilder env_section_builder_;
    LongrunOptionsBuilder options_builder_;
    Environment environment_;
    MainSection main_section_;
    LongrunOptions options_;
};

} // namespace tt
