// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.services.service_options;

import libtt.services.service : Service;

abstract class ServiceOptions
{
public:
    @property ref Service[] dependencies() { return m_depends; }
    @property void dependencies(ref Service[] depends) { m_depends = depends; }

private:
    Service[] m_depends;
}
