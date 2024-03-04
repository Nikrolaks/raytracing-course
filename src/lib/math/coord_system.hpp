#pragma once

#include "base.hpp"

namespace raytracing::math {

class CoordSystem {
public:
    CoordSystem() = default;
    CoordSystem(vec3 right, vec3 up, vec3 forward)
        : right_(right)
        , up_(up)
        , forward_(forward) {}
    CoordSystem(const CoordSystem& other)
        : right_(other.right_)
        , up_(other.up_)
        , forward_(other.forward_) {}
    CoordSystem(CoordSystem&& other) noexcept
        : right_(std::move(other.right_))
        , up_(std::move(other.up_))
        , forward_(std::move(other.forward_)) {}
    CoordSystem& operator=(const CoordSystem& other) {
        right_ = other.right_;
        up_ = other.up_;
        forward_ = other.forward_;
        return *this;
    }
    CoordSystem& operator=(CoordSystem&& other) noexcept {
        right_ = std::move(other.right_);
        up_ = std::move(other.up_);
        forward_ = std::move(other.forward_);
        return *this;
    }

    vec3 operator*(vec3 vectorString) {
        return
            right_ * vectorString.x() +
            up_ * vectorString.y() +
            forward_ * vectorString.z();
    }
private:
    vec3 right_, up_, forward_;
};

} // namespace raytracing::math
