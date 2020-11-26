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

#include <utility> // for move

#include "fmt/format.h" // for format

#include "tt/data/long_lived_script.hpp"                 // for LongLivedSc...
#include "tt/data/longrun.hpp"                           // for Longrun
#include "tt/data/service.hpp"                           // for Service
#include "tt/parser/parser_error.hpp"                    // for ParserError
#include "tt/parser/section/environment_builder.hpp"     // for Environment...
#include "tt/parser/section/longrun_options_builder.hpp" // for LongrunOpti...
#include "tt/parser/section/main_section_builder.hpp"    // for MainSection...
#include "tt/parser/section/script_builder.hpp"          // for ScriptBuilder

auto tt::LongrunDirector::InstanceService(std::string &&path)
    -> tl::expected<Service, ParserError> {
    auto &main_section = main_section_builder_.main_section();
    if (!run_script_builder_.SectionParsed()) {
        return make_parser_error<Service>(
            ParserError::Type::MissingRunSection,
            fmt::format("Missing [run] section for service '{}'",
                        main_section.name));
    }

    auto service = Longrun(std::move(main_section.name),
                           std::move(main_section.description), std::move(path),
                           std::move(options_builder_.options()),
                           std::move(env_section_builder_.environment()),
                           std::move(run_script_builder_.long_lived_script()));

    if (finish_script_builder_.SectionParsed()) {
        service.finish(finish_script_builder_.script());
    }

    return service;
}

auto tt::LongrunDirector::GetBuilderForSection(const std::string &section)
    -> tl::expected<SectionBuilder *, ParserError> {
    if (section == "main") {
        return &main_section_builder_;
    }
    if (section == "run") {
        return &run_script_builder_;
    }
    if (section == "finish") {
        return &finish_script_builder_;
    }
    if (section == "config") {
        return &env_section_builder_;
    }
    if (section == "options") {
        return &options_builder_;
    }

    return make_parser_error<SectionBuilder *>(
        ParserError::Type::InvalidSection,
        fmt::format("Section '{}' is not supported", section));
}
