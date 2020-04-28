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

#include <memory>
#include <string>
#include <vector>

#include "tt/parser/section/dummy_builder.hpp"

namespace tt {

class Service;
class SectionBuilder;

class ServiceDirector {
public:
    ServiceDirector() = default;

    virtual ~ServiceDirector() = default;

    std::shared_ptr<Service>
    ParseAndGetService(std::vector<std::string> &service_lines,
                       const std::string path);

protected:
    virtual std::shared_ptr<Service>
    InstanceService(const std::string &path) = 0;
    virtual SectionBuilder *
    GetBuilderForSection(const std::string &section) = 0;

private:
    void ParseSections();

    void TryParseSections();

    std::vector<std::string> service_lines_;
    DummyBuilder dummy_builder_;
};

} // namespace tt
