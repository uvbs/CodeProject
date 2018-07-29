#ifndef __PLUMA_REGISTRY_H__
#define __PLUMA_REGISTRY_H__

#include <stdint.h>
#include <vector>
#include <list>
#include <map>

namespace cloud_common {
namespace pluma {

class Provider;
class Registrar;

class Registry {
public:
    Registry();
    ~Registry();

    /* 验证provider的合法性 */
    bool Validate(const Provider* provider) const;

    /* 将新注册的Provider聚合到一起 */
    void Register(const Registrar* registrar);

    /*
     * BaseType下有多个ProviderCluster, 从每个ProviderCluster中选择一个Provider
     * 优先选择Tag匹配的Provider,如果Tag不存在,则返回Tag为default的Provider
     * 如果tag=defualt的Provider也不存在则当前Type下不返回Provider
     * */
    void GetProviders(std::list<Provider*>& providers, const std::string& type, const std::string& tag) const;

    uint32_t GetVersion(const std::string& type) const;
    uint32_t GetLowestVersion(const std::string& type) const;

    void RegisterType(const std::string& type, uint32_t version, uint32_t lowest_version);
    bool IsRegistered(const std::string& type) const;
    void GetRegisteredType(std::vector<std::string>& types);

private:

    /* 
     * 相同TypeName的Provider组成一个簇 
     * 一个簇中的Provider通过TagName来区分
     * */
    struct ProviderCluster {
        ~ProviderCluster();
        std::map<std::string, Provider*> tags;
    };

    /*
     * ProviderInfo用来存储BaseType的信息
     * providers存储了所有BaseType的实现
     * */
    struct ProviderInfo {

        ~ProviderInfo();

        uint32_t version;
        uint32_t lowest_version;

        /* 使用list来保证provider的调用顺序 */
        std::list<ProviderCluster*> clusters;

        /* 辅助map,用于找到相同Type的Provider */
        std::map<std::string, ProviderCluster*> types;
    };

    std::map<std::string, ProviderInfo> registered_providers_;
};

} // namespace pluma
} // namespace cloud_common

#endif // __PLUMA_REGISTRY_H__
