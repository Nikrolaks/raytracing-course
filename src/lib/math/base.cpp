#include "base.hpp"

namespace raytracing::math {

vec3 cross(const vec3& u, const vec3& v) {
    return {
        u.y() * v.z() - v.y() * u.z(),
        u.z() * v.x() - v.z() * u.x(),
        u.x() * v.y() - v.x() * u.y()
    };
}

vec3 rotate(const vec3& u, const vec4& rotation) {
    vec3 v{ rotation.x(), rotation.y(), rotation.z() };
    float t = rotation.w();

    return
        v * (v * u) * 2.f
        + u * (t * t - v.length2())
        + cross(v, u) * 2.f * t;
}

vec3 rotate(const vec3& u, const vec3& axis, float angle) {
    float sin = std::sin(angle) / 2.f;
    return rotate(u, {
        axis.x() * sin,
        axis.y()* sin,
        axis.z()* sin,
        std::cos(angle / 2.f)});
}

} // namespace raytracing::math
