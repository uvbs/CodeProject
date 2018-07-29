#ifndef __PLUMA_PLUMA_INL_H__
#define __PLUMA_PLUMA_INL_H__

inline 
Pluma::Pluma() 
{
}

template<typename ProviderType>
void Pluma::AcceptProviderType() 
{
    PluginManager::RegisterType(
        ProviderType::kProviderType,
        ProviderType::kInterfaceVersion,
        ProviderType::kInterfaceLowestVersion);
}

template<typename ProviderType> 
bool Pluma::RegisterProvider(int rank)
{
    return PluginManager::AddProvider(new ProviderType(), rank);
}

template<typename ProviderType>
void Pluma::GetProviders(std::vector<ProviderType*>& providers)
{
    std::list<Provider*> lst;
    PluginManager::GetProviders(lst, ProviderType::kProviderType);

    providers.reserve(providers.size() + lst.size());

    std::list<Provider*>::const_iterator it_list;
    for (it_list = lst.begin() ; it_list != lst.end() ; ++it_list) {
        Provider* provider = *it_list;
        providers.push_back(static_cast<ProviderType*>(provider));
    }
}

template<typename ProviderType>
void Pluma::GetProviders(std::vector<ProviderType*>& providers, const std::string& tag)
{
    std::list<Provider*> lst;
    PluginManager::GetProviders(lst, ProviderType::kProviderType, tag);

    providers.reserve(providers.size() + lst.size());

    std::list<Provider*>::const_iterator it_list;
    for (it_list = lst.begin() ; it_list != lst.end() ; ++it_list) {
        Provider* provider = *it_list;
        providers.push_back(static_cast<ProviderType*>(provider));
    }
}

#endif // __PLUMA_PLUMA_INL_H__
