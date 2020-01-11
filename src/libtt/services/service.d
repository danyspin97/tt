module libtt.services.service;

import libtt.services.service_options;

abstract class Service
{
public:
    @property string name() { return m_name; }
    @property string polishName() { return m_polishName; }
    @property string description() { return m_description; }
    @property string path() { return m_path; }
    @property ServiceOptions options() { return m_options; }

    this (
        string name,
        string polishName,
        string description,
        string path,
        ServiceOptions options
    ) {
        m_name = name;
        m_polishName = polishName;
        m_description = description;
        m_path = path;
        m_options = options;
    }

    this (Service s)
    {
        m_name = s.name;
        m_polishName = s.polishName;
        m_description = s.description;
        m_path = s.path;
    }

private:
    string m_name;
    string m_polishName;
    string m_description;
    string m_path;
    ServiceOptions m_options;
}
