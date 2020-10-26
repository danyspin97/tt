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

#include "tt/dependency_graph/dependency_reader.hpp"

#include <assert.h> // for assert

#include "tt/data/bundle.hpp"          // for Bundle
#include "tt/data/bundle_options.hpp"  // for BundleOptions
#include "tt/data/longrun.hpp"         // for Longrun
#include "tt/data/longrun_options.hpp" // for LongrunOptions
#include "tt/data/oneshot.hpp"         // for Oneshot
#include "tt/data/oneshot_options.hpp" // for OneshotOptions

using tt::Bundle;
using tt::DependencyReader;
using tt::Longrun;
using tt::Oneshot;

void DependencyReader::operator()(const Bundle &bundle) {
    const auto &options_ = bundle.options();
    dependencies_ = options_.contents();
}

void DependencyReader::operator()(const Longrun &longrun) {
    const auto &options_ = longrun.options();
    dependencies_ = options_.dependencies();
}

void DependencyReader::operator()(const Oneshot &oneshot) {
    const auto &options_ = oneshot.options();
    dependencies_ = options_.dependencies();
}

void DependencyReader::operator()(std::monostate /*unused*/) const {
    assert(false);
}
