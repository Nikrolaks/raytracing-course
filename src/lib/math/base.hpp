#pragma once

#include <algorithm>
#include <cmath>

namespace raytracing::math {

float sign(float value);

template <size_t dims>
class vec {
public:
    vec() {
        for (size_t i = 0; i < dims; ++i) {
            components_[i] = 0.0;
        }
    }
    vec(const vec& another) { std::copy_n(another.components_, dims, components_); }
    vec& operator=(const vec& another) { std::copy_n(another.components_, dims, components_); return *this; }

    float length2() const {
        float length2 = 0;
        for (size_t i = 0; i < dims; ++i) {
            length2 += components_[i] * components_[i];
        }
        return length2;
    }

    float length() const {
        return sqrt(length2());
    }

    vec& normalize() {
        auto len = length();
        for (size_t i = 0; i < dims; ++i) {
            components_[i] /= len;
        }
        return *this;
    }

    vec invert() const {
        vec result;
        for (size_t i = 0; i < dims; ++i) {
            result.components_[i] = 1.f / components_[i];
        }
        return result;
    }

    vec invertWithNaNtoZeros() const {
        vec result;
        for (size_t i = 0; i < dims; ++i) {
            if (components_[i] == 0.f) {
                result.components_[i] = 0.f;
            }
            else {
                result.components_[i] = 1.f / components_[i];
            }
        }
        return result;
    }

    static float dot(const vec& u, const vec& v) {
        float result = 0.f;
        for (size_t i = 0; i < dims; ++i) {
            result += u.components_[i] * v.components_[i];
        }
        return result;
    }

    static vec adamara(const vec& u, const vec& v) {
        vec result;
        for (size_t i = 0; i < dims; ++i) {
            result.components_[i] = u.components_[i] * v.components_[i];
        }
        return result;
    }

    static vec max(const vec& u, const vec& v) {
        vec result;
        for (size_t i = 0; i < dims; ++i) {
            result.components_[i] = std::max(u.components_[i], v.components_[i]);
        }
        return result;
    }

    static vec min(const vec& u, const vec& v) {
        vec result;
        for (size_t i = 0; i < dims; ++i) {
            result.components_[i] = std::min(u.components_[i], v.components_[i]);
        }
        return result;
    }

    float max() const {
        return *std::max_element(components_, components_ + dims);
    }

    float min() const {
        return *std::min_element(components_, components_ + dims);
    }

    vec& majorate() {
        auto major = std::max_element(components_, components_ + dims,
            [](float left, float right) {
                return std::abs(left) < std::abs(right); });
        size_t major_pos = major - components_;
        float major_value = *major;
        *this = vec<dims>();
        components_[major_pos] = sign(major_value);

        return *this;
    }

    vec& minorate() {
        auto minor = std::min_element(components_, components_ + dims,
            [](float left, float right) {
                return std::abs(left) < std::abs(right); });
        size_t minor_pos = minor - components_;
        float minor_value = *minor;
        *this = vec<dims>();
        components_[minor_pos] = sign(minor_value);

        return *this;
    }

    static vec pow(const vec& v, float q) {
        vec result;
        for (size_t i = 0; i < dims; ++i) {
            result.components_[i] = std::pow(v.components_[i], q);
        }
        return result;
    }

    vec& operator+=(const vec& other) {
        for (size_t i = 0; i < dims; ++i) {
            components_[i] += other.components_[i];
        }
        return *this;
    }

    vec& operator+=(float shift) {
        for (size_t i = 0; i < dims; ++i) {
            components_[i] += shift;
        }
        return *this;
    }

    vec& operator-=(const vec& other) {
        for (size_t i = 0; i < dims; ++i) {
            components_[i] -= other.components_[i];
        }
        return *this;
    }

    vec& operator-=(float shift) {
        return *this += -shift;
    }

    vec& operator*=(float scalar) {
        for (size_t i = 0; i < dims; ++i) {
            components_[i] *= scalar;
        }
        return *this;
    }

    vec& operator/=(float scalar) {
        return *this *= 1.f / scalar;
    }

    vec operator-() const { return *this * (-1); }

    vec operator+(const vec& oth) const { vec res(*this); res += oth; return res; }
    vec operator+(float shift) const { vec res(*this); res += shift; return res; }

    vec operator-(const vec& oth) const { vec res(*this); res -= oth; return res; }
    vec operator-(float shift) const { vec res(*this); res -= shift; return res; }

    vec operator*(float scalar) const { vec res(*this); res *= scalar; return res; }
    vec operator/(float scalar) const { vec res(*this); res /= scalar; return res; }

protected:
    float components_[dims];
};

class vec2 : public vec<2> {
public:
    vec2() = default;
    vec2(const vec<2>& v) : vec<2>(v) {}
    vec2(float x, float y) {
        components_[0] = x;
        components_[1] = y;
    }

    float x() const {
        return components_[0];
    }

    float y() const {
        return components_[1];
    }

    vec2 normal() const {
        vec2 res;
        res.components_[0] = components_[1];
        res.components_[1] = -components_[0];
        return res;
    }
};

class vec3 : public vec<3> {
public:
    vec3() = default;
    vec3(const vec<3>& v) : vec<3>(v) {}
    vec3(float x, float y, float z) {
        components_[0] = x;
        components_[1] = y;
        components_[2] = z;
    }

    float x() const {
        return components_[0];
    }

    float y() const {
        return components_[1];
    }

    float z() const {
        return components_[2];
    }
};

class vec4 : public vec<4> {
public:
    vec4() = default;
    vec4(const vec<4>& v) : vec<4>(v) {}
    vec4(float x, float y, float z, float w) {
        components_[0] = x;
        components_[1] = y;
        components_[2] = z;
        components_[3] = w;
    }

    float x() const {
        return components_[0];
    }

    float y() const {
        return components_[1];
    }

    float z() const {
        return components_[2];
    }

    float w() const {
        return components_[3];
    }
};

class quaternion {
public:
    quaternion(const vec4& vec) : ijk_(vec.x(), vec.y(), vec.z()), w_(vec.w()) {}
    quaternion(const vec3& v, float w) : ijk_(v), w_(w) {}
    quaternion(const quaternion& other) : ijk_(other.ijk_), w_(other.w_) {}

    quaternion& operator=(const quaternion& other) {
        ijk_ = other.ijk_;
        w_ = other.w_;
        return *this;
    }
    quaternion& operator=(const vec4& v) {
        ijk_ = vec3(v.x(), v.y(), v.z());
        w_ = v.w();
        return *this;
    }

    operator vec4() const {
        return vec4(
            ijk_.x(),
            ijk_.y(),
            ijk_.z(),
            w_
        );
    }

    vec3 imaginary() const {
        return ijk_;
    }

    float real() const {
        return w_;
    }

    quaternion conjugate() const;
    quaternion& normalize();

    quaternion operator*(float scalar) const;
    quaternion operator*(const vec3& v) const;
    quaternion operator*(const quaternion& q) const;
private:
    vec3 ijk_;
    float w_;
};

template<size_t dims>
float dot(const vec<dims>& u, const vec<dims>& v) { return vec<dims>::dot(u, v); }

template<size_t dims>
vec<dims> pow(const vec<dims>& u, float q) { return vec<dims>::pow(u, q); }

template<size_t dims>
vec<dims> adamara(const vec<dims>& u, const vec<dims>& v) { return vec<dims>::adamara(u, v); }

template<size_t dims>
vec<dims> max(const vec<dims>& u, const vec<dims>& v) { return vec<dims>::max(u, v); }

template<size_t dims>
vec<dims> min(const vec<dims>& u, const vec<dims>& v) { return vec<dims>::min(u, v); }

vec3 cross(const vec3& u, const vec3& v);

vec3 rotate(const vec3& u, const quaternion& rotation);
vec3 rotate(const vec3& u, const vec3& axis, float angle);

} // namespace raytracing::math
