#include <stdint.h>

#include "pluma/provider.h"

using namespace std;

namespace cloud_common {
namespace pluma {

Provider::Provider()
    : rank_(0)
{
}

Provider::~Provider()
{
}

bool Provider::IsCompatible(uint32_t current_version, uint32_t lowest_version) const
{
    uint32_t version = GetVersion();
    return lowest_version <= version && version <= current_version;
}

} // namespace pluma
} // namespace cloud_common
