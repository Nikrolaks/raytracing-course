#include "distributions.hpp"

#include <cassert>
#include <numeric>

namespace raytracing::math::distributions {

vec3 cosine::sample(const vec3&, const vec3& normal) {
    vec3 v = generator_(GlobalRandomHolder::engine());
    return (v + normal).normalize();
}

float cosine::pdf(const vec3&, const vec3& normal, const vec3& direction) {
    return std::max(0.f, dot(direction, normal) / (float)(M_PI));
}

mix::mix(std::vector<std::shared_ptr<distribution>>&& list)
    : distributions_(list)
    , chooser_(0LL, distributions_.size() - 1) {

    assert(distributions_.size() > 0);
}

vec3 mix::sample(const vec3& point, const vec3& normal) {
    return distributions_[chooser_(GlobalRandomHolder::engine())]->sample(point, normal);
}

float mix::pdf(const vec3& point, const vec3& normal, const vec3& direction) {
    return std::accumulate(distributions_.begin(), distributions_.end(), 0.f, [&](float z, std::shared_ptr<distribution>& d) {
        return z + d->pdf(point, normal, direction);
        }) / (float)(distributions_.size());
}

} // namespace raytracing::math::distributions
