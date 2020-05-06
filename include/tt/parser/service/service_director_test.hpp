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

#include "tt/data/service.hpp"
#include "tt/parser/section/section_builder.hpp"
#include "tt/parser/service/service_director.hpp"

namespace tt {

class BuilderTest : public SectionBuilder {
public:
    void ParseLine(const std::string &line) {
        auto msg = line;
        time_parsed_++;
    }

    void EndParsing() { finished_ = true; }

    int time_parsed_;
    bool finished_ = false;
};

class ServiceDirectorTest : public ServiceDirector {
public:
    std::shared_ptr<Service> InstanceService(const std::string &path) {
        auto msg = path;
        return std::unique_ptr<Service>{};
    }

    SectionBuilder *GetBuilderForSection(const std::string &section) {
        auto msg = section;
        return &builder_test_;
    }

    BuilderTest builder_test_;
};

} // namespace tt
