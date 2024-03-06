#include "base.hpp"

namespace raytracing::math {

float sign(float value) {
    if (value > 0.f) {
        return 1.f;
    }
    if (value < 0.f) {
        return -1.f;
    }
    return 0.f;
}

quaternion quaternion::conjugate() const {
    return quaternion(-ijk_, w_);
}

quaternion& quaternion::normalize() {
    return *this = vec4(ijk_.x(), ijk_.y(), ijk_.z(), w_).normalize();
}

quaternion quaternion::operator*(float scalar) const {
    return quaternion(ijk_ * scalar, w_ * scalar);
}

quaternion quaternion::operator*(const vec3& v) const {
    // (  w   -z  y   ) ( a )
    // (  z   w   -x  ) ( b )
    // (  -y  x   w   ) ( c )
    // (  -x  -y  -z  )
    return vec4(
        dot(vec3(w_, -ijk_.z(), ijk_.y()), v),
        dot(vec3(ijk_.z(), w_, -ijk_.x()), v),
        dot(vec3(-ijk_.y(), ijk_.x(), w_), v),
        dot(-ijk_, v)
    );
}

quaternion quaternion::operator*(const quaternion& q) const {
    // (  w  -z   y   x  ) ( qx )
    // (  z   w   -x  y  ) ( qy )
    // (  -y  x   w   z  ) ( qz )
    // (  -x  -y  -z  w  ) ( qw )
    vec4 qcolumn = q;
    return vec4(
        dot(vec4(w_, -ijk_.z(), ijk_.y(), ijk_.x()), qcolumn),
        dot(vec4(ijk_.z(), w_, -ijk_.x(), ijk_.y()), qcolumn),
        dot(vec4(-ijk_.y(), ijk_.x(), w_, ijk_.z()), qcolumn),
        dot((vec4)(conjugate()), qcolumn)
    );
}

vec3 cross(const vec3& u, const vec3& v) {
    return {
        u.y() * v.z() - v.y() * u.z(),
        u.z() * v.x() - v.z() * u.x(),
        u.x() * v.y() - v.x() * u.y()
    };
}

vec3 rotate(const vec3& u, const quaternion& rotation) {
    return (rotation * u * rotation.conjugate()).imaginary();
}

vec3 rotate(const vec3& u, const vec3& axis, float angle) {
    float sin = std::sin(angle) / 2.f;
    return rotate(u, quaternion(
        (axis * sin).normalize(),
        std::cos(angle / 2.f)
    ));
}

} // namespace raytracing::math
