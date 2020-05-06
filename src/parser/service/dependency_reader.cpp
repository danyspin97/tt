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

#include "tt/parser/service/dependency_reader.hpp"

#include "tt/data/bundle.hpp"
#include "tt/data/bundle_options.hpp"
#include "tt/data/longrun.hpp"
#include "tt/data/longrun_options.hpp"
#include "tt/data/oneshot.hpp"
#include "tt/data/oneshot_options.hpp"

using tt::Bundle;
using tt::BundleOptions;
using tt::DependencyReader;
using tt::Longrun;
using tt::LongrunOptions;
using tt::Oneshot;
using tt::OneshotOptions;

void DependencyReader::Visit(Bundle *bundle) {
    auto &options_ = dynamic_cast<BundleOptions &>(bundle->options());
    dependencies_ = options_.contents();
}

void DependencyReader::Visit(Longrun *longrun) {
    auto &options_ = dynamic_cast<LongrunOptions &>(longrun->options());
    dependencies_ = options_.dependencies();
}

void DependencyReader::Visit(Oneshot *oneshot) {
    auto &options_ = dynamic_cast<OneshotOptions &>(oneshot->options());
    dependencies_ = options_.dependencies();
}
