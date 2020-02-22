module libtt.serialization.oneshot_serialization;

@safe:

import libtt.data;
import libtt.serialization.oneshot_options_serialization : OneshotOptionsSerialization;
import libtt.serialization.service_serialization : ServiceSerialization;
import libtt.serialization.script_serialization : ScriptSerialization;

import std.conv : to;
import std.json;

class OneshotSerialization : ServiceSerialization
{
public:
    this(Oneshot service) @system
    {
        super(service);
        m_service = service;
    }

    @property override JSONValue json() @system
    {
        JSONValue j = super.json;

        j.object["start"] = new ScriptSerialization(m_service.start).json;
        j.object["stop"] = new ScriptSerialization(m_service.stop).json;

        return j;
    }

    @property override Oneshot object()
    {
        return m_service;
    }

private:
    Oneshot m_service;
}
