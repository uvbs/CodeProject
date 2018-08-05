#include <iostream>
#include <assert.h>
#include "test_jconfig.h"

using namespace std;

int test_jconfig()
{
    boost::shared_ptr<cloud_search::Config> conf(new cloud_search::Config());
    boost::shared_ptr<cloud_search::ConfigEngines> conf_engines(new cloud_search::ConfigEngines());

    if (!conf->Init("./config/config_test.conf"))
    {
        cout << "config init failed!" << endl;
        return -1;
    }

    if (!conf_engines->Init(conf))
    {
        cout << "config engine init failed!" << endl;
        return -1;
    }

    string out;
    conf->PrintToString(&out);
    cout << out << endl;

    vector<string> sections = conf->sections();
    for (size_t i = 0; i < sections.size(); ++i)
    {
        assert(conf->HasSection(sections[i]));
        cout << "section " << i << " " << sections[i] << " option size=" << conf->Section(sections[i])->OptionSize() << endl;
    }

    assert(conf->HasSection("rank1"));
    assert(conf->HasSection("rank2"));

    assert(conf->Section("rank1")->Option()->HasOption("enable"));
    assert(conf->Section("rank1")->Option()->HasOption("version"));
    assert(conf->Section("rank1")->Option()->HasOption("params"));
    assert(conf->Section("rank1")->Option()->HasOption("params", "q"));
    assert(conf->Section("rank1")->Option()->HasOption("params", "s"));
    assert(conf->Section("rank1")->Option()->HasOption("params", "s"));
    assert(conf->Section("rank1")->Option()->HasOption("hosts"));
    assert(conf->Section("rank1")->Option()->HasOptionValue("params", "query"));
    assert(conf->Section("rank1")->Option()->HasOptionValue("params", "count"));
    assert(conf->Section("rank1")->Option()->HasOptionValue("params", "start"));
    assert(conf->Section("rank1")->Option()->HasOptionValue("hosts", "192.168.0.1"));
    assert(conf->Section("rank1")->Option()->HasOptionValue("hosts", "192.168.0.2"));

    assert(conf->Section("rank2")->Option(0)->HasOption("enable"));
    assert(conf->Section("rank2")->Option(0)->HasOption("version"));
    assert(conf->Section("rank2")->Option(0)->HasOption("params"));
    assert(conf->Section("rank2")->Option(0)->HasOption("params", "q"));
    assert(conf->Section("rank2")->Option(0)->HasOption("params", "s"));
    assert(conf->Section("rank2")->Option(0)->HasOption("params", "c"));
    assert(conf->Section("rank2")->Option(0)->HasOption("hosts"));
    assert(conf->Section("rank2")->Option(1)->HasOption("enable"));
    assert(conf->Section("rank2")->Option(1)->HasOption("version"));
    assert(conf->Section("rank2")->Option(1)->HasOption("params"));
    assert(conf->Section("rank2")->Option(1)->HasOption("hosts"));

    assert(conf->section("rank1")->GetInt("enable") == 1);
    assert(conf->section("rank1")->GetStr("version") == "1.0");
    assert(conf->section("rank1")->GetStr("params", "q") == "query");
    assert(conf->section("rank1")->GetStr("params", "c") == "count");
    assert(conf->section("rank1")->GetStr("params", "s") == "start");
    assert(conf->section("rank1")->GetStr("hosts", 0) == "192.168.0.1");

    assert(conf->Section("rank1")->Option()->GetInt("enable") == 1);
    assert(conf->Section("rank1")->Option()->GetStr("version") == "1.0");
    assert(conf->Section("rank1")->Option()->GetStr("params", "q") == "query");
    assert(conf->Section("rank1")->Option()->GetStr("params", "c") == "count");
    assert(conf->Section("rank1")->Option()->GetStr("params", "s") == "start");
    assert(conf->Section("rank1")->Option()->GetStr("hosts", 0) == "192.168.0.1");
    assert(conf->Section("rank1")->Option()->GetSize("hosts") == 4);

    assert(conf->Section("rank2")->Option(0)->GetInt("enable") == 1);
    assert(conf->Section("rank2")->Option(0)->GetStr("version") == "1.1");

    assert(conf->Section("rank2")->Option(1)->GetInt("enable") == 1);
    assert(conf->Section("rank2")->Option(1)->GetStr("version") == "1.2");

    assert(conf->Section("rank3")->Option()->GetStr("version", "", "default") == "default");

    boost::shared_ptr<cloud_search::ConfigSection> section = conf->Section("rank2");
    for (size_t i = 0; i < section->OptionSize(); ++i)
    {
        string output;
        section->Option(i)->PrintToString(&output);
        cout << i << "\n" << output << endl;
    }

    conf_engines->Print();
    assert(conf_engines->engine("engine1")->GetStr("version") == "engine_1.1.0");

    return 0;
}

