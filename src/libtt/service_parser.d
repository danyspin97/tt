module libtt.service_parser;

import std.conv : to;
import std.file;
import std.string : toStringz ;

import std.experimental.logger;

import confini;

import libtt.service;
import libtt.oneshot;
import libtt.longrun;
import libtt.bundle;

import libtt.define;
import libtt.dirs;

import libtt.section_parser;

int parse_all_services(bool system_mode = true) {
    immutable string[] dirs_to_read = [ dirs.service, dirs.admin_service ];

    string[] services;

    foreach(dir; dirs_to_read) {
        foreach(string name; dirEntries(dir, SpanMode.shallow)) {
            services ~= name;
        }
    }
    return 0;
}

auto parse_services(string [] service_names, bool system_mode = true) {
    immutable string[] dirs_to_read = [ dirs.service, dirs.admin_service ];

    Service[] services;

    foreach(dir; dirs_to_read) {
        foreach(name; service_names) {
            auto path = dir ~ "/" ~ name;
            if (exists(path) && isFile(path)) {
                services ~= parse_single_service(path);
            }
        }
    }
    return services;
}

extern (C) int service_dispatcher (IniDispatch* dispatch, void* service_ptr) {
    const auto key_parse_dispatch = [
        "main": &main_section_parser,
        "start": &start_section_parser,
        "stop": &stop_section_parser,
        "run": &run_section_parser,
        "environment": &environment_section_parser,
        "logger": &logger_section_parser
    ];

    // dispatch.type does always contains 0, breaking this
    //if (dispatch.type == IniNodeType.INI_SECTION) {
    auto key = to!string(dispatch.data);
    if ((key in key_parse_dispatch) != null) {
            // criticalf("Section named %s is not allowed", section);
       // }
        return 0;
    } else {

    // if (dispatch.type == IniNodeType.INI_KEY) {
        auto section = to!string(dispatch.append_to);
        return key_parse_dispatch[section](dispatch, service_ptr);
    }

    return 1;
}

IniFormat get_service_format() {
    IniFormat f;

    f.delimiter_symbol = IniDelimiters.INI_EQUALS;
    f.case_sensitive = 1;
    f.semicolon_marker = IniCommentMarker.INI_IGNORE;
    f.hash_marker = IniCommentMarker.INI_IGNORE;
    f.multiline_nodes = IniMultiline.INI_NO_MULTILINE;
    f.section_paths = IniSectionPaths.INI_ABSOLUTE_ONLY;
    f.no_single_quotes = 1;
    f.no_double_quotes = 0;
    f.no_spaces_in_names = 1;
    f.implicit_is_not_empty = 1;

    return f;
}

Service parse_single_service(string path) {
    Service service = new Service();

    uint ret = load_ini_path(
                toStringz(path),
                get_service_format(),
                null,
                &service_dispatcher,
                &service);

    if (ret == CONFINI_ERROR) {
        criticalf("Something went wrong when parsing file %s", path);
    } else if(ret == ConfiniInterruptNo.CONFINI_ENOMEM) {
        criticalf("Could not alloc memory when reading file %s", path);
    } else if (ret == ConfiniInterruptNo.CONFINI_ENOENT) {
        logf("No file at: %s", path);
    } else if (ret == ConfiniInterruptNo.CONFINI_EIO) {
        criticalf("Could not read file at: %s", path);
    }

    log(service.name);
    return service;
}

