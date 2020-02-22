module libtt.serialization.longrun_serialization;

@safe:
nothrow:

import libtt.data;
import libtt.serialization.longrun_options_serialization : LongrunOptionsSerialization;
import libtt.serialization.service_serialization : ServiceSerialization;
import libtt.serialization.script_serialization : ScriptSerialization;

import std.conv : to;
import std.json;

class LongrunSerialization : ServiceSerialization
{
public:
    this(Longrun service)
    {
        super(service);
        m_service = service;
    }

    @property override JSONValue json() @system
    {
        JSONValue j = super.json;

        j.object["run"] = new ScriptSerialization(m_service.run).json;
        j.object["finish"] = new ScriptSerialization(m_service.finish).json;

        return j;
    }

    @property override Longrun object()
    {
        return m_service;
    }

private:
    Longrun m_service;
}
