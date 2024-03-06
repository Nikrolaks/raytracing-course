#pragma once

#include <math/base.hpp>

namespace raytracing::render {

struct intersection {
    float distance;
    math::vec3 normal;
    render::color color;
    bool inside = false;
};

} // namespace raytracing::render
