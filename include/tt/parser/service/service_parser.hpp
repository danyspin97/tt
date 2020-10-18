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

#include <memory>
#include <string>
#include <vector>

#include "tt/data/service.hpp"
#include "tt/parser/service/parser_factory.hpp"

namespace tt {

class ServiceParser {
public:
    explicit ServiceParser(std::string path);

    [[nodiscard]] auto service() const -> Service;

protected:
    static auto GenerateListFrom(const std::string &path)
        -> std::vector<std::string>;

private:
    auto GetTypeFromService(const std::vector<std::string> &service_lines)
        -> std::string;

    [[nodiscard]] auto
    GetMainSectionIndex(const std::vector<std::string> &service_lines) const
        -> size_t;
    [[nodiscard]] auto ParseService() -> Service;

    std::string path_;
    Service service_;
};

} // namespace tt
