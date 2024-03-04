#pragma once

#include <math/base.hpp>
#include <math/coord_system.hpp>
#include <math/ray.hpp>

namespace raytracing::abs {

class Camera {
public:
    Camera() = default;

    math::ray sight(const math::vec2& pixel);
protected:
    math::vec3 position_;
    math::CoordSystem cs_;
    float forwardX_, forwardY_;
};

} // namespace raytracing::abs
