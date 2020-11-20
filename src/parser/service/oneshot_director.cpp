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

#include "tt/parser/service/oneshot_director.hpp"

#include <utility> // for move

#include "tt/data/oneshot.hpp"                           // for Oneshot
#include "tt/exception.hpp"                              // for Exception
#include "tt/parser/section/environment_builder.hpp"     // for Environment...
#include "tt/parser/section/main_section_builder.hpp"    // for MainSection...
#include "tt/parser/section/oneshot_options_builder.hpp" // for OneshotOpti...
#include "tt/parser/section/script_builder.hpp"          // for ScriptBuilder

auto tt::OneshotDirector::InstanceService(std::string &&path) -> tt::Service {
    auto &main_section = main_section_builder_.main_section();
    if (!start_script_builder_.HasScript()) {
        throw Exception("Service '" + main_section.name +
                        "' does not have a [start] section");
    }

    auto service = Oneshot(std::move(main_section.name),
                           std::move(main_section.description), std::move(path),
                           std::move(options_builder_.options()),
                           std::move(env_section_builder_.environment()),
                           start_script_builder_.main_script());

    if (stop_script_builder_.HasScript()) {
        service.stop(stop_script_builder_.script());
    }

    return service;
}

auto tt::OneshotDirector::GetBuilderForSection(const std::string &section)
    -> SectionBuilder * {
    if (section == "main") {
        return &main_section_builder_;
    }
    if (section == "start") {
        return &start_script_builder_;
    }
    if (section == "stop") {
        return &stop_script_builder_;
    }
    if (section == "config") {
        return &env_section_builder_;
    }
    if (section == "options") {
        return &options_builder_;
    }
    auto msg = "Section '" + section + "' is not supported";
    throw Exception(msg);
}
