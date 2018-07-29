#include <cstdio>
#include <vector>

#include "pluma/plugin_manager.h"
#include "pluma/registry.h"
#include "pluma/dlibrary.h"

using namespace std;

namespace cloud_common {
namespace pluma {

PluginManager::PluginManager()
    : registry_(NULL)
{
    registry_ = new Registry;
}

PluginManager::~PluginManager()
{
    delete registry_;
    Unload();
}

bool PluginManager::Load(const string& path, const string& plugin_tag, const string& plugin_conf)
{
    string plugin_name = GetPluginName(path);

    /* plugin_tag不能为空 */
    if (plugin_tag.empty()) {
        printf("plugin_tag is empty!\n");
        return false;
    }

    /* 如果plugin_name下存在同名plugin_tag, 则当前插件Load失败 */
    if (libraries_.find(make_pair(plugin_name, plugin_tag)) != libraries_.end()) {
        printf("plugin_name exist\n");
        return false;
    }

    DLibrary* lib = DLibrary::Load(path);
    if (lib == NULL) {
        printf("path lib is null\n");
        return false;
    }

    RegisterPluginFn* register_function;
    register_function = reinterpret_cast<RegisterPluginFn*>(lib->GetSymbol("Register"));
    if (register_function == NULL) {
        printf("function \'Register\' not find\n");
        delete lib;
        return false;
    }

    Registrar registrar(registry_, plugin_name, plugin_tag, plugin_conf);
    if (!register_function(registrar)) {
        printf("registar fail\n");
        delete lib;
        return false;
    }

    /* 
     * 将新注册的Provider聚合在一起 
     * */
    registry_->Register(&registrar);

    libraries_[make_pair(plugin_name, plugin_tag)] = lib;

    return true;
}

void PluginManager::Unload()
{
    LibraryMap::iterator it;
    for (it = libraries_.begin(); it != libraries_.end(); ++it) {
        delete it->second;
    }
    libraries_.clear();
}

string PluginManager::GetPluginName(const string& path)
{
    size_t last_dash = path.find_last_of("/");
    if (last_dash == string::npos) {
        last_dash = 0;
    } else {
        last_dash++;
    }

    if (path.length() > last_dash + 3) {
        if (path[last_dash] == 'l' && path[last_dash + 1] == 'i' && path[last_dash + 2] == 'b') {
            last_dash += 3;
        }
    }

    size_t so_pos = path.find(".so");
    if (so_pos == string::npos || so_pos < last_dash) {
        so_pos = path.length();
    }

    return path.substr(last_dash, so_pos - last_dash);
}

bool PluginManager::AddProvider(Provider* provider, int rank)
{
    if (provider == NULL) {
        return false;
    }

    Registrar registrar(registry_);

    if (!registrar.AddProvider(provider, rank)) {
        return false;
    }

    registry_->Register(&registrar);

    return true;
}

void PluginManager::GetLoadedPlugins(vector<pair<const string*, const string*> >& plugins) const
{
    plugins.reserve(plugins.size() + libraries_.size());

    LibraryMap::const_iterator it;
    for (it = libraries_.begin(); it != libraries_.end(); ++it) {
        plugins.push_back(make_pair(&(it->first.first), &(it->first.second)));
    }
}

bool PluginManager::IsLoaded(const string& plugin_path, const string& plugin_tag) const 
{
    string plugin_name = GetPluginName(plugin_path);
    return libraries_.find(make_pair(plugin_name, plugin_tag)) != libraries_.end();
}

void PluginManager::RegisterType(const string& type, uint32_t version, uint32_t lowest_version)
{
    registry_->RegisterType(type, version, lowest_version);
}

void PluginManager::GetProviders(list<Provider*>& providers, const string& type) const
{
    registry_->GetProviders(providers, type, "");
}

void PluginManager::GetProviders(list<Provider*>& providers, const string& type, const string& tag) const
{
    string plugin_tag = tag;
    if (tag.empty()) {
        plugin_tag = "default";
    }
    registry_->GetProviders(providers, type, plugin_tag);
}

void PluginManager::GetRegisteredType(vector<string>& types)
{
    registry_->GetRegisteredType(types);
}

} // namespace pluma
} // namespace cloud_common
