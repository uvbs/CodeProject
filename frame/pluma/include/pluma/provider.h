#ifndef __PLUMA_PROVIDER_H__
#define __PLUMA_PROVIDER_H__

#include <stdint.h>
#include <string>

namespace cloud_common {
namespace pluma {

class Registrar;
class Provider {
    friend class Registrar;
public:
    Provider();
    virtual ~Provider();
    virtual uint32_t GetVersion() const = 0;
    virtual std::string GetType() const = 0;
    virtual std::string GetBaseType() const = 0;

    int rank() const { return rank_; }
    const std::string& conf() const { return conf_; }

    bool IsCompatible(uint32_t current_version, uint32_t lowest_version) const;

private:
    void set_rank(int rank) { rank_ = rank; }
    void set_conf(const std::string& conf) { conf_ = conf; }

private:
    int rank_;
    std::string conf_;
};

} // namespace pluma
} // namespace cloud_common

#endif // __PLUMA_PROVIDER_H__
