module libtt.section_parser;

import std.conv: to;

import std.experimental.logger;

import confini : IniDispatch, ini_string_parse ;

import libtt.script_parser: parse_script;

import libtt.services.service : Service;
import libtt.services.oneshot : Oneshot;
import libtt.services.longrun : Longrun;
import libtt.services.bundle : Bundle;

void convert_service(Service* service, string type) {
    if (type == "oneshot") {
        *service = new Oneshot(*service);
    } else if (type == "longrun") {
        *service = new Longrun(*service);
    } else if (type == "bundle") {
        *service = new Bundle(*service);
    } else {
        criticalf("Type %s is not allowed for services", type);
    }
}

int main_section_parser(IniDispatch* dispatch, void* service_ptr) {
    ini_string_parse(dispatch.value, dispatch.format);
    auto key = to!string(dispatch.data);
    auto value = to!string(dispatch.value);
    auto service = cast(Service*)service_ptr;
    switch (key) {
        case "name":
            service.name = value;
            break;
        case "description":
            service.description = value;
            break;
        case "type":
            convert_service(service, value);
            break;
        default:
            criticalf("Key %s is not allowed in section [main]", key);
            return 1;
    }
    return 0;
}

int start_section_parser(IniDispatch* dispatch, void* service_ptr) {
    auto oneshot = cast(Oneshot*)service_ptr;
    return parse_script(dispatch, oneshot.start);
}

int stop_section_parser(IniDispatch* dispatch, void* service_ptr) {
    auto oneshot = cast(Oneshot*)service_ptr;
    return parse_script(dispatch, oneshot.stop);
}

int run_section_parser(IniDispatch* dispatch, void* service_ptr) {
    auto longrun = cast(Longrun*)service_ptr;
    return parse_script(dispatch, longrun.run);
}

int environment_section_parser(IniDispatch* dispatch, void* service_ptr) {
    return 0;
}

int logger_section_parser(IniDispatch* dispatch, void* service_ptr) {
    auto oneshot = cast(Longrun*)service_ptr;
    return 0;
}

