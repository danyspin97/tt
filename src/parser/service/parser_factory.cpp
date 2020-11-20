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

#include "tt/parser/service/parser_factory.hpp"

#include "tt/exception.hpp"                       // for Exception
#include "tt/parser/service/bundle_director.hpp"  // for BundleDirector
#include "tt/parser/service/longrun_director.hpp" // for LongrunDirector
#include "tt/parser/service/oneshot_director.hpp" // for OneshotDirector
#include "tt/parser/service/service_director.hpp" // for ServiceDirector

auto tt::ParserFactory::GetDirectorPerType(const std::string &type)
    -> std::unique_ptr<ServiceDirector> {
    if (type == "bundle") {
        return std::make_unique<BundleDirector>();
    }
    if (type == "longrun") {
        return std::make_unique<LongrunDirector>();
    }
    if (type == "oneshot") {
        return std::make_unique<OneshotDirector>();
    }

    auto msg = "Type '" + type + "' is not supported.";
    throw Exception(msg);
}
