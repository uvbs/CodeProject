#ifndef __PLUMA_PLUGIN_MANAGER_H__
#define __PLUMA_PLUGIN_MANAGER_H__

#include <string>
#include <map>

#include "pluma/registrar.h"
#include "pluma/registry.h"

namespace cloud_common {
namespace pluma {

class DLibrary;

/* Manages loaded plugins. */
class PluginManager {
public:

    ~PluginManager();

    /*
     * Load a plugin given it's path
     * Path for the plugin, including plugin name.
     * */
    bool Load(const std::string& path, 
              const std::string& plugin_tag = "default", 
              const std::string& plugin_conf = "");

    /* Directly add a new provider. */
    bool AddProvider(Provider* provider, int rank);

    /* Get the name of all loaded plugins. */
    void GetLoadedPlugins(std::vector<std::pair<const std::string*, const std::string*> >& plugins) const;

    /* Check if a plug-in is loaded. */
    bool IsLoaded(const std::string& plugin_name, const std::string& plugin_tag = "default") const;

protected:
    /* 
     * Default constructor.  
     * PluginManager cannot be publicly instantiated.
     * */
    PluginManager();

    /* Unload all loaded plugins. */
    void Unload();

    /*
     * Register a provider type 
     *
     * type Provider type.  
     * version Current version of that provider type.  
     * lowestVersion Lowest compatible version of that provider type.
     * */
    void RegisterType(const std::string& type, unsigned int version, unsigned int lowestVersion);
    void GetRegisteredType(std::vector<std::string>& types);

    /* Get providers of a certain type.  */
    void GetProviders(std::list<Provider*>& providers, const std::string& type) const;
    void GetProviders(std::list<Provider*>& providers, const std::string& type, const std::string& tag) const;

private:
    /* Get the plugin name (without extension) from its path */
    static std::string GetPluginName(const std::string& path);

private:
    /* Signature for the plugin's registration function */
    typedef bool RegisterPluginFn(Registrar&);

    /*
     * 支持相同PluginName的多个不同版本动态库加载
     * 使用TagName来区分不同版本的动态库
     * 该机制可以用来实现A/B版本的运行时动态调用
     * */
    typedef std::map<std::pair<std::string, std::string>, DLibrary*> LibraryMap;

    /* Map containing the loaded libraries */
    LibraryMap libraries_;   
    Registry*  registry_;
};

} // namespace pluma
} // namespace cloud_common

#endif // __PLUMA_PLUGIN_MANAGER_H__
