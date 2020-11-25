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

#include <string>  // for allocator, string
#include <utility> // for move

#include "tt/data/bundle.hpp"                     // for Bundle
#include "tt/data/bundle_options.hpp"             // for BundleOptions
#include "tt/data/service.hpp"                    // for Service
#include "tt/parser/parser_error.hpp"             // for ParserError
#include "tt/parser/section/section_builder.hpp"  // for SectionBuilder
#include "tt/parser/service/service_director.hpp" // for ServiceDirector

namespace tt {

class BuilderTest : public SectionBuilder {
public:
    BuilderTest() : SectionBuilder("test") {}

    auto ParseLine(const std::string & /*line*/) noexcept
        -> tl::expected<void, ParserError> override {
        time_parsed_++;
        return {};
    }

    auto EndParsing() noexcept -> tl::expected<void, ParserError> override {
        return SectionBuilder::EndParsing();
    }

    auto GetValidAttributes() const noexcept
        -> std::vector<std::string> override {
        return {};
    }
    auto GetValidArrayAttributes() const noexcept
        -> std::vector<std::string> override {
        return {};
    }

    int time_parsed_{};
};

class ServiceDirectorTest : public ServiceDirector {
public:
    auto InstanceService(std::string && /*path*/) noexcept
        -> tl::expected<Service, ParserError> override {
        tt::BundleOptions options;
        return tt::Bundle("mybundle", "", "/mybundle", std::move(options));
    }

    auto GetBuilderForSection(const std::string & /*section*/) noexcept
        -> tl::expected<SectionBuilder *, ParserError> override {
        return &builder_test_;
    }

    BuilderTest builder_test_;
};

} // namespace tt
