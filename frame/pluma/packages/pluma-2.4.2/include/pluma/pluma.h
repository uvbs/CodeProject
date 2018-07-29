#ifndef __PLUMA_PLUMA_H__
#define __PLUMA_PLUMA_H__

#include <stdint.h>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <pthread.h>

#include "pluma/provider.h"
#include "pluma/plugin_manager.h"
#include "pluma/registrar.h"

/* Andy macro to convert parameter to string */
#define PLUMA_2STRING(X) #X

/* provider classes */
#define PLUMA_PROVIDER_HEADER(TYPE)\
    PLUMA_PROVIDER_HEADER_BEGIN(TYPE)\
        virtual TYPE* Create() const = 0;\
        virtual TYPE* Get() = 0;\
        virtual void Reset(TYPE* type = NULL) = 0;\
        virtual TYPE* ThreadGet() = 0;\
        virtual void ThreadReset(TYPE* type = NULL) = 0;\
    PLUMA_PROVIDER_HEADER_END

/* Macro that generate first part of the provider definition */
#define PLUMA_PROVIDER_HEADER_BEGIN(TYPE)\
class TYPE##Provider: public cloud_common::pluma::Provider {\
    friend class cloud_common::pluma::Pluma;\
private:\
    static const uint32_t    kInterfaceVersion;\
    static const uint32_t    kInterfaceLowestVersion;\
    static const std::string kProviderType;\
public:\
    virtual ~TYPE##Provider() {}\
    std::string GetType() const { return kProviderType; }\
    std::string GetBaseType() const { return "Provider"; }\
    unsigned int GetVersion() const { return kInterfaceVersion; }\
/* Macro that generate last part of the provider definition */
#define PLUMA_PROVIDER_HEADER_END };

/* Macro that generate the provider declaration */
#define PLUMA_PROVIDER_SOURCE(TYPE, Version, LowestVersion)\
const std::string TYPE##Provider::kProviderType = PLUMA_2STRING(TYPE);\
const unsigned int TYPE##Provider::kInterfaceVersion = Version;\
const unsigned int TYPE##Provider::kInterfaceLowestVersion = LowestVersion;


/* 
 * Macro that helps plugins generating their provider implementations 
 * PRE: SPECIALIZED_TYPE must inherit from BASE_TYPE
 * */
#define PLUMA_INHERIT_PROVIDER(SPECIALIZED_TYPE, BASE_TYPE, VERSION)\
class SPECIALIZED_TYPE##Provider: public BASE_TYPE##Provider {\
private:\
    uint32_t version_;\
    BASE_TYPE* instance_;\
    std::map<pthread_t, BASE_TYPE*> thread_instances_;\
    pthread_mutex_t mutex_;\
public:\
    SPECIALIZED_TYPE##Provider() : version_(VERSION), instance_(NULL) {\
        pthread_mutex_init(&mutex_, NULL);\
    }\
    ~SPECIALIZED_TYPE##Provider() {\
        delete instance_;\
        std::map<pthread_t, BASE_TYPE*>::iterator iter = thread_instances_.begin();\
        for (; iter != thread_instances_.end(); ++iter) {\
            delete iter->second;\
        }\
        thread_instances_.clear();\
        pthread_mutex_destroy(&mutex_);\
    }\
    BASE_TYPE* Create() const { return new SPECIALIZED_TYPE(); }\
    BASE_TYPE* Get() { return instance_; }\
    BASE_TYPE* ThreadGet() {\
        pthread_t pid = pthread_self();\
        std::map<pthread_t, BASE_TYPE*>::iterator iter = thread_instances_.find(pid);\
        if (iter != thread_instances_.end()) {\
            return iter->second;\
        }\
        return NULL;\
    }\
    void Reset(BASE_TYPE* instance) {\
        pthread_mutex_lock(&mutex_);\
        if (instance != instance_) {\
            delete instance_;\
            instance_ = instance;\
        }\
        pthread_mutex_unlock(&mutex_);\
    }\
    void ThreadReset(BASE_TYPE* instance) {\
        pthread_t pid = pthread_self();\
        pthread_mutex_lock(&mutex_);\
        std::map<pthread_t, BASE_TYPE*>::iterator iter = thread_instances_.find(pid);\
        if (iter != thread_instances_.end()) {\
            if (iter->second != instance) {\
                delete iter->second;\
                iter->second = instance;\
            }\
        } else {\
            thread_instances_[pid] = instance;\
        }\
        pthread_mutex_unlock(&mutex_);\
    }\
    uint32_t GetVersion() const { return version_; }\
    std::string GetType() const { return PLUMA_2STRING(SPECIALIZED_TYPE); }\
    std::string GetBaseType() const { return PLUMA_2STRING(BASE_TYPE); }\
}; 

namespace cloud_common {
namespace pluma {

class Pluma: public PluginManager {
public:
    Pluma();

    /*
     * 限定只接受哪些ProviderType
     * */
    template<typename ProviderType> 
    void AcceptProviderType();

    /*
     * 注册Provider到Registray中
     * 和Registrar::Register函数类似,不同的是可以不以动态库加载的方式注册Provider
     * */
    template<typename ProviderType> 
    bool RegisterProvider(int rank = 0);

    /* 
     * 获取Provider对象
     * 如果没有指定第二个参数,则返回所有Provider对象
     * 如果指定第二个参数,则返回指定TAG的Provider对象
     * 如果指定TAG对象不存在,返回default的Provider对象
     * */
    template<typename ProviderType>
    void GetProviders(std::vector<ProviderType*>& providers);

    template<typename ProviderType>
    void GetProviders(std::vector<ProviderType*>& providers, const std::string& tag);
};

#include "pluma/pluma-inl.h"

} // namespace pluma
} // namespace cloud_common

#endif // __PLUMA_PLUMA_H__
