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

#include "tt/parser/service/services_parser.hpp"

#include <algorithm> // for transform
#include <cassert>   // for assert
#include <iterator>  // for back_insert...
#include <sstream>   // for basic_strin...
#include <utility>   // for move, pair
#include <variant>   // for holds_alter...

#include "fmt/format.h" // for format

#include "tt/dependency_graph/utils.hpp"                 // for ForEachDepe...
#include "tt/parser/parser_error.hpp"                    // for ParserError
#include "tt/parser/service/instance_service_parser.hpp" // for InstanceSer...
#include "tt/parser/service/service_parser.hpp"          // for ServiceParser
#include "tt/path/dirs.hpp"                              // for Dirs
#include "tt/utils/launch_async.hpp"                     // for LaunchAsync

tt::ServicesParser::ServicesParser(const std::shared_ptr<Dirs> &dirs)
    : suffix_(dirs->services_suffix()), paths_(dirs->servicedirs()) {}

auto tt::ServicesParser::ParseServices(
    const std::vector<std::string> &service_names)
    -> tl::expected<std::vector<Service>, ParserError> {
    std::vector<Service> services;
    for (const auto &name : service_names) {
        service_names_to_parse_.emplace_back(name);
    }
    service_to_parse_.store(service_names_to_parse_.size());

    auto launch_service_parser = std::async(std::launch::async, [this]() {
        while (service_to_parse_ != 0) {
            std::unique_lock lock(services_to_parse_mutex_);
            while (service_names_to_parse_.empty() && service_to_parse_ != 0) {
                services_cv_.wait(lock);
            }

            if (service_to_parse_ == 0) {
                return;
            }

            auto name = service_names_to_parse_.back();
            service_names_to_parse_.pop_back();
            lock.unlock();
            auto future = std::async(std::launch::async, [this, name]() {
                return ParseService(name);
            });
            std::unique_lock futures_lock(futures_mutex_);
            futures_.emplace_back(std::move(future));
            future_cv_.notify_one();
            futures_lock.unlock();
        }
    });

    while (service_to_parse_ != 0) {
        std::unique_lock lock(futures_mutex_);
        while (futures_.empty()) {
            future_cv_.wait(lock);
        }
        service_to_parse_--;

        auto future = std::move(futures_.back());
        futures_.pop_back();
        lock.unlock();
        future.wait();
        auto service = future.get();
        if (!service.has_value()) {
            return tl::unexpected(service.error());
        }
        services.emplace_back(service.value());
    }

    services_cv_.notify_one();
    launch_service_parser.wait();

    return services;
}

auto tt::ServicesParser::ParseService(const std::string &service_name)
    -> tl::expected<Service, ParserError> {
    static auto default_parser = std::make_shared<ServiceParser>();

    auto parser = default_parser;
    auto name = service_name;

    // Parse instance service
    if (size_t token_index = GetInstanceTokenIndex(service_name);
        token_index != std::string::npos) {
        std::string instance_name =
            SplitServiceNameFromInstance(name, token_index);
        parser = std::make_shared<InstanceServiceParser>(instance_name);
    }

    auto path = GetPathForServiceName(name);
    if (!path.has_value()) {
        return tl::unexpected(path.error());
    }
    auto service = parser->ParseService(path.value());
    if (!service.has_value()) {
        return chain_parser_error<Service>(
            service.error(), fmt::format(" for service '{}'", path.value()));
    }
    assert(!std::holds_alternative<std::monostate>(service.value()));
    ParseDependenciesOfService(service.value());
    return service.value();
}

auto tt::ServicesParser::GetInstanceTokenIndex(const std::string &service_name)
    -> size_t {
    return service_name.find('@');
}

auto tt::ServicesParser::SplitServiceNameFromInstance(std::string &service_name,
                                                      size_t token_index)
    -> std::string {
    auto string_iter = service_name.begin();
    advance(string_iter, token_index);
    std::string instance{string_iter, service_name.end()};
    service_name = service_name.substr(0, token_index);
    return instance;
}

auto tt::ServicesParser::GetPathForServiceName(const std::string &name)
    -> tl::expected<std::string, ParserError> {
    for (const auto &service_dir : paths_) {
        std::string service_path = service_dir / std::string{name + suffix_};
        // TODO: Check if file is readable
        if (std::filesystem::exists(service_path) &&
            std::filesystem::is_regular_file(service_path)) {
            return service_path;
        }
    }

    std::stringstream joined_paths;
    copy(paths_.begin(), paths_.end(),
         std::ostream_iterator<std::string>(joined_paths, ","));
    return make_parser_error<std::string>(
        ParserError::Type::ServiceNotFound,
        fmt::format("Service '{}{}' could not be found in directories ", name,
                    suffix_));
    ;
}

void tt::ServicesParser::ParseDependenciesOfService(const Service &service) {
    std::vector<std::string> deps;
    ForEachDependencyOfService(service,
                               [&deps](auto name) { deps.emplace_back(name); });
    service_to_parse_.fetch_add(deps.size());
    std::unique_lock lock(services_to_parse_mutex_);
    service_names_to_parse_.insert(service_names_to_parse_.end(), deps.begin(),
                                   deps.end());
    lock.unlock();
    services_cv_.notify_one();
}
