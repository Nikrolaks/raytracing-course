#pragma once

#include "base.hpp"

namespace raytracing::math {

class ray {
public:
    ray(const vec3& origin, const vec3& direction)
        : origin_(origin)
        , direction_(direction) {
        direction_.normalize();
    }
    ray(const ray& other)
        : origin_(other.origin_)
        , direction_(other.direction_)
        , deepness_(other.deepness_) {}

    ray deeper(const vec3& origin, const vec3& direction) const {
        ray next(origin, direction);
        next.deepness_ = deepness_ + 1;
        return next;
    }

    const vec3& origin() const { return origin_; }
    const vec3& direction() const { return direction_; }
    size_t deepness() const { return deepness_; }

    ray& translate(const vec3& shift);
    ray& rotate(const vec4& rotation);
    ray& rotate(const vec3& axis, float angle);

private:
    vec3 origin_;
    vec3 direction_;
    size_t deepness_ = 0;
};

} // raytracing::math