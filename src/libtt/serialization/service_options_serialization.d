module libtt.serialization.service_options_serialization;

@safe:
nothrow:

import libtt.data;

import std.conv : to;
import std.json;
import std.algorithm;
import std.array;

class ServiceOptionsSerialization
{
public:
    this(ServiceOptions serviceOptions)
    {
        m_serviceOptions = serviceOptions;
    }

    @property JSONValue json() @system
    {
        JSONValue j = ["dependencies" : m_serviceOptions.dependencies];

        return j;
    }

    ServiceOptions get_object()
    {
        return m_serviceOptions;
    }

protected:
    static void add_super_properties(ref ServiceOptions serviceOptions, JSONValue j) @system
    {
        auto arr = array(map!(a => a.str)(j["dependencies"].array));
        serviceOptions.dependencies(arr);
    }

private:
    ServiceOptions m_serviceOptions;
}
