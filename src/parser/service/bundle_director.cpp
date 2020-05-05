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

#include "tt/parser/service/bundle_director.hpp"

#include "tt/data/bundle.hpp"
#include "tt/exception.hpp"
#include "tt/parser/section/bundle_options_builder.hpp"
#include "tt/parser/section/main_section_builder.hpp"

using std::make_shared;
using std::shared_ptr;
using std::string;

using tt::Bundle;
using tt::BundleDirector;
using tt::Exception;
using tt::SectionBuilder;
using tt::Service;

BundleDirector::BundleDirector()
    : main_section_builder_(main_section_), options_builder_(options_) {}

shared_ptr<Service> BundleDirector::InstanceService(const string &path) {
    return make_shared<Bundle>(main_section_.name, main_section_.polish_name,
                               main_section_.description, path, options_);
}

SectionBuilder *
BundleDirector::GetBuilderForSection(const std::string &section) {
    if (section == "main") {
        return &main_section_builder_;
    }
    if (section == "options") {
        return &options_builder_;
    }
    auto msg = "Section '" + section + "' is not supported.";
    throw Exception(msg);
}
