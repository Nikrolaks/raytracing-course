#include "ray.hpp"

namespace raytracing::math {

ray& ray::translate(const vec3& shift) {
    origin_ += shift;
    return *this;
}

ray& ray::rotate(const vec4& rotation) {
    origin_ = math::rotate(origin_, rotation);
    direction_ = math::rotate(direction_, rotation);
    return *this;
}

ray& ray::rotate(const vec3& axis, long double angle) {
    origin_ = math::rotate(origin_, axis, angle);
    direction_ = math::rotate(direction_, axis, angle);
    return *this;
}

} // namespace raytracing::math
