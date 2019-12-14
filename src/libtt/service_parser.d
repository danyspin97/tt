module libtt.service_parser;

import std.conv : to;
import std.file;

import std.experimental.logger;

import confini;

import libtt.service;
import libtt.oneshot;
import libtt.longrun;
import libtt.bundle;

import libtt.define;
import libtt.dirs;

alias RowParser = const int function(string key, string value, Service service);

int main_section_parser(string key, string value, Service service) {
    return 0;
}

int service_parser(bool system_mode = true) {
    immutable string[] dirs_to_read = [ dirs.service, dirs.admin_service ];

    string[] services;

    foreach(dir; dirs_to_read) {
        foreach(string name; dirEntries(dir, SpanMode.shallow)) {
            services ~= name;
        }
    }
    return 0;
}


extern (C) int service_dispatcher (IniDispatch* dispatch, void* service_ptr) {
    const RowParser[string] key_parse_dispatch = [
        "main": &main_section_parser,
        "start": &start_section_parser,
        "stop": &stop_section_parser,
        "run": &run_section_parser,
        "environment": &environment_section_parser,
        "logger": &logger_section_parser
    ];

    static string current_section;

    auto service = cast(Service)service_ptr;

    if (dispatch.type == IniNodeType.INI_SECTION) {
        auto section = to!string(dispatch.data);
        if ((section in key_parse_dispatch) != null) {
            criticalf("Section named %s is not allowed", section);
        }
        current_section = section;
        return 0;
    }

    if (dispatch.type == IniNodeType.INI_KEY) {
        ini_unquote(dispatch.data, dispatch.format);
        ini_string_parse(dispatch.value, dispatch.format);

        auto key = to!string(dispatch.data);
        auto value = to!string(dispatch.value);
        return key_parse_dispatch[current_section](key, value, service);
    }

    return 1;
}

