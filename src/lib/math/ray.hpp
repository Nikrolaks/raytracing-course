#pragma once

#include "base.hpp"

namespace raytracing::math {

class ray {
public:
    ray(vec3 origin, vec3 direction)
        : origin_(origin)
        , direction_(direction.normalize()) {}

    const vec3& origin() const { return origin_; }
    const vec3& direction() const { return direction_; }

    ray& translate(const vec3& shift);
    ray& rotate(const vec4& rotation);
    ray& rotate(const vec3& axis, long double angle);

private:
    vec3 origin_;
    vec3 direction_;
};

} // raytracing::math