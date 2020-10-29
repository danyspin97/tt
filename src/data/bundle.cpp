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

#include "tt/data/bundle.hpp"

#include <utility> // for move

#include "tt/data/service_options.hpp" // for operator<<

using std::ostream;

using tt::Bundle;

Bundle::Bundle(std::string &&name, std::string &&description,
               std::string &&path, BundleOptions &&options)
    : ServiceImpl(std::move(name), std::move(description), std::move(path)),
      options_(std::move(options)) {}

auto Bundle::Dump(ostream &oss) const -> ostream & {
    oss << "[main]\n"
        << "\ntype = bundle";
    ServiceImpl::Dump(oss);
    return oss << "\n\n[options]\n" << options();
}

auto operator<<(std::ostream &oss, const Bundle &bundle) -> std::ostream & {
    return bundle.Dump(oss);
}
