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

#include "tt/parser/service/instance_service_parser.hpp"

#include <algorithm>
#include <utility>

using std::replace;
using std::string;
using std::vector;

using tt::InstanceServiceParser;

InstanceServiceParser::InstanceServiceParser(const string &path,
                                             string instance_name)
    : ServiceParser(path), instance_name_(std::move(instance_name)) {}

vector<string>
InstanceServiceParser::GenerateListFrom(const std::string &path) {
    auto lines = ServiceParser::GenerateListFrom(path);
    replace(lines.begin(), lines.end(), InstanceToken, instance_name_);
    return lines;
}
