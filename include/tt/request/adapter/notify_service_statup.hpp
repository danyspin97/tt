
#pragma once

#include <nlohmann/json.hpp> // for json, adl_serializer

#include "tt/request/notify_service_status.hpp" // for NotifyServiceStatus

namespace nlohmann {
template <> struct adl_serializer<tt::request::NotifyServiceStatus> {
    static tt::request::NotifyServiceStatus from_json(const json &j) {
        auto status = tt::ParseServiceStatusFromString(j.at("status"));
        return {j.at("service"), status};
    }

    // Here's the catch! You must provide a to_json method! Otherwise you
    // will not be able to convert move_only_type to json, since you fully
    // specialized adl_serializer on that type
    static void
    to_json(json &j, const tt::request::NotifyServiceStatus &notify_service) {
        j["service"] = notify_service.service();
        j["status"] = tt::GetServiceStatusName(notify_service.status());
    }
};
} // namespace nlohmann
