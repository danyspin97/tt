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

#include "tt/data/service_impl.hpp"

#include <filesystem>
#include <utility>

#include "tt/data/exception.hpp"

using std::ostream;

tt::ServiceImpl::ServiceImpl(std::string name, std::string description,
                             std::string path, tt::ServiceOptions &options)
    : name_(std::move(name)), description_(std::move(description)),
      path_(std::move(path)), options_(options) {
    ValidatePath();
}

void tt::ServiceImpl::ValidatePath() const {
    std::filesystem::path file(path());
    if (file.stem() != name()) {
        throw ServiceNameDoNotMatchFileExecption();
    }
}

ostream &tt::ServiceImpl::Dump(ostream &oss) const {
    oss << "name = " << name();
    if (!description().empty()) {
        oss << "\ndescription = " << description();
    }
    return oss;
}
