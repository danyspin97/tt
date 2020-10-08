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

#include "tt/parser/service/longrun_director.hpp"

#include <memory>

#include "tt/data/service.hpp"
#include "tt/exception.hpp"
#include "tt/parser/section/environment_builder.hpp"
#include "tt/parser/section/logger_script_builder.hpp"
#include "tt/parser/section/longrun_options_builder.hpp"
#include "tt/parser/section/main_section_builder.hpp"
#include "tt/parser/section/script_builder.hpp"
#include "tt/parser/section/section_builder.hpp"

using std::string;

using tt::Exception;
using tt::Longrun;
using tt::LongrunDirector;
using tt::SectionBuilder;

LongrunDirector::LongrunDirector()
    : main_section_builder_(main_section_), run_script_builder_("run"),
      finish_script_builder_("finish"),
      logger_script_builder_(main_section_.name),
      env_section_builder_(environment_), options_builder_(options_) {}

auto LongrunDirector::InstanceService(string &&path) -> tt::Service {
    if (!run_script_builder_.HasScript()) {
        throw Exception("Service '" + main_section_.name +
                        "' does not have a [run] section");
    }

    auto service = Longrun(
        std::move(main_section_.name), std::move(main_section_.description),
        std::move(path), std::move(options_), std::move(environment_),
        run_script_builder_.long_lived_script());

    if (finish_script_builder_.HasScript()) {
        service.finish(finish_script_builder_.script());
    }
    if (logger_script_builder_.HasScript()) {
        service.logger(logger_script_builder_.logger_script());
    }

    return service;
}

auto LongrunDirector::GetBuilderForSection(const string &section)
    -> SectionBuilder * {
    if (section == "main") {
        return &main_section_builder_;
    }
    if (section == "run") {
        return &run_script_builder_;
    }
    if (section == "finish") {
        return &finish_script_builder_;
    }
    if (section == "log") {
        return &logger_script_builder_;
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
