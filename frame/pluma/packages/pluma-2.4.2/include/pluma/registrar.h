#ifndef __PLUMA_REGISTRAR_H__
#define __PLUMA_REGISTRAR_H__

#include <string>
#include <list>

#include "registry.h"
#include "provider.h"

namespace cloud_common {
namespace pluma {

class PluginManager;

class Registrar {
    friend class PluginManager;
public:
    Registrar(Registry* registry);
    Registrar(Registry* registry, 
              const std::string& plugin_name, 
              const std::string& plugin_tag, 
              const std::string& plugin_conf);
    ~Registrar();

    template<typename ProviderType>
    bool Register(int rank = 0);

    const std::string& plugin_name() const { return plugin_name_; }
    const std::string& plugin_tag() const { return plugin_tag_; }
    const std::string& plugin_conf() const { return plugin_conf_; }
    const std::list<Provider*>& providers() const { return providers_; }

private:
    bool AddProvider(Provider* provider, int rank);

private:
    Registry* registry_;
    std::string plugin_name_;
    std::string plugin_tag_;
    std::string plugin_conf_;
    std::list<Provider*> providers_;
};

#include "pluma/registrar-inl.h"

} // namespace pluma
} // namespace cloud_common

#endif // __PLUMA_REGISTRAR_H__
