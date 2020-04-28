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

#include "tt/parser/service/oneshot_director.hpp"

#include <memory>

#include "tt/data/oneshot.hpp"
#include "tt/data/service.hpp"
#include "tt/exception.hpp"
#include "tt/parser/section/environment_builder.hpp"
#include "tt/parser/section/main_section_builder.hpp"
#include "tt/parser/section/oneshot_options_builder.hpp"
#include "tt/parser/section/script_builder.hpp"

using std::make_shared;
using std::shared_ptr;
using std::string;

using tt::Environment;
using tt::EnvironmentBuilder;
using tt::Exception;
using tt::MainSectionBuilder;
using tt::Oneshot;
using tt::OneshotDirector;
using tt::OneshotOptionsBuilder;
using tt::ScriptBuilder;
using tt::SectionBuilder;
using tt::Service;

OneshotDirector::OneshotDirector()
    : main_section_builder_(main_section_),
      start_script_builder_(environment_, "start"),
      stop_script_builder_(environment_, "stop"),
      env_section_builder_(environment_), options_builder_(options_) {}

shared_ptr<Service> OneshotDirector::InstanceService(const string &path) {
    if (!start_script_builder_.HasScript()) {
        throw Exception("Service '" + main_section_.name +
                        "' does not have a [start] section");
    }

    auto service =
        make_shared<Oneshot>(main_section_.name, main_section_.polish_name,
                             main_section_.description, path, options_,
                             start_script_builder_.script());

    if (stop_script_builder_.HasScript()) {
        service->stop(stop_script_builder_.script());
    }

    return service;
}

SectionBuilder *OneshotDirector::GetBuilderForSection(const string &section) {
    if (section == "main") {
        return &main_section_builder_;
    } else if (section == "start") {
        return &start_script_builder_;
    } else if (section == "stop") {
        return &stop_script_builder_;
    } else if (section == "config") {
        return &env_section_builder_;
    } else if (section == "options") {
        return &options_builder_;
    }
    auto msg = "Section '" + section + "' is not supported.";
    throw new Exception(msg);
}
