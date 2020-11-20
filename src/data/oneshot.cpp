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

#include "tt/data/oneshot.hpp"

#include <ostream> // for operator<<, ostream, basic_os...
#include <utility> // for move

#include "tt/data/service_options.hpp" // for operator<<

tt::Oneshot::Oneshot(std::string &&name, std::string &&description,
                     std::string &&path, OneshotOptions &&options,
                     Environment &&environment, MainScript &&start)
    : ServiceImpl(std::move(name), std::move(description), std::move(path)),
      options_(std::move(options)), environment_(std::move(environment)),
      start_(std::move(start)) {}

auto tt::Oneshot::operator==(const Oneshot &rhs) const -> bool {
    return ServiceImpl::operator==(rhs) && options_ == rhs.options_ &&
           environment_ == rhs.environment_ && start_ == rhs.start_ &&
           stop_ == rhs.stop_;
}

auto tt::Oneshot::Dump(std::ostream &oss) const -> std::ostream & {
    oss << "[main]\n";
    ServiceImpl::Dump(oss);
    oss << "\ntype = oneshot";
    oss << "\n\n[start]\n" << start_;
    if (stop_) {
        oss << "\n\n[stop]\n" << stop().value();
    }
    oss << "\n\n[options]\n" << options();
    if (environment_.CountKeys() != 0) {
        oss << "\n\n[config]\n" << environment_;
    }
    return oss;
}

auto operator<<(std::ostream &oss, const tt::Oneshot &oneshot)
    -> std::ostream & {
    return oneshot.Dump(oss);
}
