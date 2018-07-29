#ifndef __PLUMA_REGISTRAR_INL_H__
#define __PLUMA_REGISTRAR_INL_H__

template<typename ProviderType>
bool Registrar::Register(int rank)
{
    return AddProvider(new ProviderType(), rank);
}

#endif // __PLUMA_REGISTRAR_INL_H__
