#include "camera.hpp"

namespace raytracing::abs {

math::ray Camera::sight(const math::vec2& pixel) {
    math::vec3 cofs{
        pixel.x() * std::tanl(forwardX_ / 2.f),
        pixel.y() * std::tanl(forwardY_ / 2.f),
        1.f
    };

    return math::ray{
        position_, cs_ * cofs
    };
}

} // namespace raytracing::abs
