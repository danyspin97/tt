// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.service;

@safe:
nothrow:

import std.exception : enforce;
import std.path : baseName, stripExtension;

import libtt.data.service_options : ServiceOptions;

abstract class Service
{
public:
    @property string name() const
    {
        return m_name;
    }

    @property string polishName() const
    {
        return m_polishName;
    }

    @property string description() const
    {
        return m_description;
    }

    @property string path() const
    {
        return m_path;
    }

    @property ServiceOptions options()
    {
        return m_options;
    }

protected:
    this(in string name, in string polishName, in string description, in string path,
            ServiceOptions options)
    {
        m_name = name;
        m_polishName = polishName;
        m_description = description;
        m_path = path;
        m_options = options;

        validatePath();
    }

private:
    void validatePath() const
    {
        immutable auto nameFromPath = stripExtension(baseName(path));
        auto enforceMessage = "The name camp must match the name of the file";
        enforce(name == nameFromPath, enforceMessage);
    }

    string m_name;
    string m_polishName;
    string m_description;
    string m_path;
    ServiceOptions m_options;
}
