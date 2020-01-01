module libtt.script_parser;

import std.conv : to;
import std.string : strip, stripLeft, stripRight;

import std.experimental.logger;

import confini : IniDispatch;

import libtt.services.script : /*BuildType,*/ Script;
import libtt.services.oneshot : Oneshot;

int start_parsing_execute(string value, ref Script script) {
    if (value[0] != '(') {
        return 1;
    }

    if (value[$ - 1] == ')') {
        script.execute = strip(value[1 .. $ - 1]);
        return 0;
    }

    script.execute = stripLeft(value[1 .. $]) ~ '\n';
    script.is_parsing = true;
    return 0;
}

int parse_execute(string key, ref Script script) {
    if (key == ")") {
        script.is_parsing = false;
        return 0;
    }

    if (key[$ - 1] == ')') {
        script.execute ~= stripRight(key[0 .. $ - 1]);
        script.is_parsing = false;
        return 0;
    }

    script.execute ~= key ~ '\n';
    return 0;
}

int parse_script(IniDispatch* dispatch, ref Script script) {
    auto key = to!string(dispatch.data);
    auto value = to!string(dispatch.value);
    switch (key) {
        case "type":
            /*
            if (value == "auto") {
                script.build = BuildType.Auto;
            }
            break;
            */
        case "execute":
            return start_parsing_execute(value, script);
            break;
        default:
            if (value != "") {
                criticalf("Key %s is not allowed in section [start]", key);
                return 1;
            }
            // Assume that if there is no value, then this key is part
            // of the current Script.execute
            return parse_execute(key, script);
    }

    return 0;
}
