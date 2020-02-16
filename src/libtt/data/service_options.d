// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.data.service_options;

import libtt.data.service : Service;

abstract class ServiceOptions
{
public:
    @property ref string[] dependencies() { return m_depends; }
    @property void dependencies(ref string[] depends) { m_depends = depends; }

private:
    string[] m_depends;
}
