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

#include "tt/request/reply/service_info.hpp"

#include "Base64.h" // for Decode

#include "tl/expected.hpp" // for expected

#include "tt/utils/deserialize.hpp" // for Deserialize

tt::request::reply::ServiceInfo::ServiceInfo(std::string encoded_info)
    : encoded_service_info_(std::move(encoded_info)) {}

tt::request::reply::ServiceInfo::ServiceInfo(const LiveService &live_service) {
    using Buffer = std::vector<uint8_t>;
    Buffer buffer;

    auto written_size =
        bitsery::quickSerialization<bitsery::OutputBufferAdapter<Buffer>>(
            buffer, live_service);
    std::string serialized_live_service;
    auto end = buffer.cbegin();
    std::advance(end, written_size);
    std::copy(buffer.cbegin(), end,
              std::back_inserter(serialized_live_service));
    encoded_service_info_ = macaron::Base64::Encode(serialized_live_service);
}

auto tt::request::reply::ServiceInfo::Decode()
    -> tl::expected<LiveService, std::string> {
    std::string encoded_status;
    macaron::Base64::Decode(encoded_service_info_, encoded_status);
    std::vector<uint8_t> buffer;
    buffer.reserve(encoded_status.size());
    std::copy(encoded_status.begin(), encoded_status.end(),
              std::back_insert_iterator(buffer));

    LiveService live_service;
    auto state = bitsery::quickDeserialization<
        bitsery::InputBufferAdapter<std::vector<uint8_t>>>(
        {buffer.begin(), buffer.size()}, live_service);
    if (state.first == bitsery::ReaderError::NoError && state.second) {
        return live_service;
    }

    return tl::make_unexpected(
        "There was an error while deserializing the service info");
}
