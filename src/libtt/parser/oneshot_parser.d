// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

module libtt.parser.oneshot_parser;

import libtt.parser.main_section : MainSection;
import libtt.services.environment : Environment;
import libtt.services.oneshot : Oneshot;
import libtt.services.oneshot_options : OneshotOptions;
import libtt.services.script : Script;
import libtt.services.service : Service;

class OneshotParser
{
public:
    @property Service service() { return m_service; }

    this(MainSection mainSection)
    {
        this.mainSection = mainSection;
        parseSections();
        createOneshot();
    }

    void createOneshot()
    {

    }

    void parseSections()
    {

    }

private:
    MainSection mainSection;
    Script start;
    Script stop;
    Environment environment;
    OneshotOptions options;

    Oneshot m_service;
}
