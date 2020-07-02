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

#include "tt/data/longrun.hpp"

#include "tt/service_visitor.hpp"

using std::ostream;

using tt::Longrun;

Longrun::Longrun(const std::string &name, const std::string &description,
                 const std::string &path, LongrunOptions options,
                 Environment environment, const Script &run)
    : ServiceImpl(name, description, path), options_(options),
      environment_(std::move(environment)), run_(run) {}

auto Longrun::Dump(ostream &oss) const -> ostream & {
    oss << "[main]\n";
    ServiceImpl::Dump(oss);
    oss << "\ntype = longrun";
    oss << "\n\n[run]\n" << run();
    if (finish()) {
        oss << "\n\n[finish]\n" << finish().value();
    }
    oss << "\n\n[logger]\n" << logger().value();
    oss << "\n\n[options]\n" << options();
    if (!environment_.GetAll().empty()) {
        oss << "\n\n[config]\n" << environment_;
    }
    return oss;
}

auto operator<<(std::ostream &oss, Longrun const &longrun) -> std::ostream & {
    return longrun.Dump(oss);
}
