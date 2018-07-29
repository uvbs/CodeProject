#include "pluma/registrar.h"

#include <string>

#include "pluma/provider.h"
#include "pluma/registry.h"

using namespace std;

namespace cloud_common {
namespace pluma {

Registrar::Registrar(Registry* registry)
    : registry_(registry)
{
    plugin_tag_ = "default";
}

Registrar::Registrar(Registry* registry, 
                     const string& plugin_name, 
                     const string& plugin_tag,
                     const string& plugin_conf)
    : registry_(registry)
    , plugin_name_(plugin_name)
    , plugin_tag_(plugin_tag)
    , plugin_conf_(plugin_conf)
{
}

Registrar::~Registrar()
{
}

bool Registrar::AddProvider(Provider* provider, int rank)
{
    if (provider == NULL) {
        return false;
    }

    if (!registry_->Validate(provider)) {
        delete provider;
        return false;
    }

    provider->set_rank(rank);
    provider->set_conf(plugin_conf_);
    providers_.push_back(provider);

    return true;
}

} //namespace pluma
} //namespace cloud_common
