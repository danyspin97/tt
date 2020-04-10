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

#include "tt/data/oneshot.hpp"

using std::ostream;

using tt::Oneshot;

Oneshot::Oneshot(const std::string name, const std::string polish_name,
                 const std::string description, const std::string path,
                 OneshotOptions options, Script start)
    : Service(name, polish_name, description, path, options), start_(start) {}

ostream &Oneshot::Dump(ostream &oss) const {
    oss << "[main]\n";
    Service::Dump(oss);
    oss << "\ntype = oneshot";
    oss << "\n\n[start]\n" << start_;
    if (stop_) {
        oss << "\n\n[stop]\n" << stop().value();
    }
    oss << "\n\n[options]\n" << options();
    if (start_.environment().GetAll().size() != 0) {
        oss << "\n\n[config]\n" << start_.environment();
    }
    return oss;
}
