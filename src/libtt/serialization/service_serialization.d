module libtt.serialization.service_serialization;

import libtt.data;
import libtt.serialization.service_options_serialization : ServiceOptionsSerialization;

import std.conv : to;
import std.json;

class ServiceSerialization
{
public:
    this(Service service)
    {
        m_service = service;
    }

    @property JSONValue json()
    {
        JSONValue j = ["name" : m_service.name];
        j.object["polishName"] = JSONValue(m_service.polishName);
        j.object["description"] = JSONValue(m_service.description);
        j.object["path"] = JSONValue(m_service.path);

        auto options = new ServiceOptionsSerialization(m_service.options);
        j.object["options"] = options.json;

        return j;
    }

    @property Service object()
    {
        return m_service;
    }

private:
    Service m_service;
}
