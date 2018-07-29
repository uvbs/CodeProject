#include <cstdio>

#include "pluma/provider.h"
#include "pluma/registry.h"
#include "pluma/registrar.h"

using namespace std;

namespace cloud_common {
namespace pluma {

Registry::Registry() 
{
}

Registry::~Registry() 
{
    registered_providers_.clear();
}

Registry::ProviderInfo::~ProviderInfo() 
{
    list<ProviderCluster*>::iterator iter = clusters.begin();
    for (; iter != clusters.end(); ++iter) {
        ProviderCluster* cluster = *iter;
        delete cluster;
    }
    clusters.clear();
}

Registry::ProviderCluster::~ProviderCluster()
{
    map<string, Provider*>::iterator iter = tags.begin();
    for (; iter != tags.end(); ++iter) {
        Provider* provider = iter->second;
        delete provider;
    }
    tags.clear();
}

void Registry::RegisterType(const string& type, uint32_t version, uint32_t lowest_version)
{
    if (IsRegistered(type)) {
        return;
    }

    ProviderInfo info;
    info.version = version;
    info.lowest_version = lowest_version;

    registered_providers_[type] = info;
}

void Registry::Register(const Registrar* registrar)
{
    list<Provider*>::const_iterator iter = registrar->providers().begin();
    for (; iter != registrar->providers().end(); ++iter) {
        Provider* provider = *iter;
        ProviderInfo& info = registered_providers_[provider->GetBaseType()];

        /*
         * 在相同PluginName下对相同Type的Provider做聚合
         * */
        string key = registrar->plugin_name() + "/" + provider->GetType();
        map<string, ProviderCluster*>::iterator type_iter = info.types.find(key);
        if (type_iter == info.types.end()) {
            ProviderCluster* cluster = new ProviderCluster;
            cluster->tags[registrar->plugin_tag()] = provider;

            /* info.types仅用做聚合使用 */
            info.types[key] = cluster;
            info.clusters.push_back(cluster);
        } else {
            ProviderCluster* cluster = type_iter->second;
            cluster->tags[registrar->plugin_tag()] = provider;
        }
    }
}

bool Registry::Validate(const Provider* provider) const
{
    string type = provider->GetBaseType();

    /* 检查是否是注册过的Provider */
    if (!IsRegistered(type)) {
        return false;
    }

    /* 检查当前Provider版本是否兼容 */
    if (!provider->IsCompatible(GetVersion(type), GetLowestVersion(type))) {
        return false;
    }

    return true;
}

bool Registry::IsRegistered(const string& type) const
{
    return registered_providers_.find(type) != registered_providers_.end();
}

void Registry::GetRegisteredType(vector<string>& types)
{
    map<string, ProviderInfo>::const_iterator iter = registered_providers_.begin();
    for (; iter != registered_providers_.end(); ++iter) {
        types.push_back(iter->first);
    }
}

uint32_t Registry::GetVersion(const string& type) const
{
    map<string, ProviderInfo>::const_iterator iter = registered_providers_.find(type);
    if (iter != registered_providers_.end()) {
        return iter->second.version;
    }
    return 0;
}

uint32_t Registry::GetLowestVersion(const string& type) const
{
    map<string, ProviderInfo>::const_iterator iter = registered_providers_.find(type);
    if (iter != registered_providers_.end()) {
        return iter->second.lowest_version;
    }
    return 0;
}

bool SortByRank(const Provider* left, const Provider* right)
{
    return left->rank() > right->rank();
}

void Registry::GetProviders(list<Provider*>& providers, const string& type, const string& tag) const
{
    map<string, ProviderInfo>::const_iterator iter = registered_providers_.find(type);
    if (iter != registered_providers_.end()) {
        const ProviderInfo& info = iter->second;

        list<ProviderCluster*>::const_iterator iter_cluster = info.clusters.begin();
        for (; iter_cluster != info.clusters.end(); ++iter_cluster) {
            ProviderCluster* cluster = *iter_cluster;

            if (!tag.empty()) {
                map<string, Provider*>::const_iterator iter_provider = cluster->tags.find(tag);
                if (iter_provider == cluster->tags.end()) {
                    /* 指定tag的provider不存在, 获取tag=default的provider */
                    iter_provider = cluster->tags.find("default");
                    if (iter_provider != cluster->tags.end()) {
                        providers.push_back(iter_provider->second);
                    }
                } else {
                    providers.push_back(iter_provider->second);
                }
            } else {
                map<string, Provider*>::const_iterator iter_provider = cluster->tags.begin();
                for (; iter_provider != cluster->tags.end(); ++iter_provider) {
                    providers.push_back(iter_provider->second);
                }
            }
        }
    }

    if (providers.size() != 0) {
        providers.sort(SortByRank);
    }
}

} //namespace pluma
} //namespace cloud_common
