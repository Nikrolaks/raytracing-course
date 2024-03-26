#pragma once

#include <math/base.hpp>

namespace raytracing::render {

struct intersection {
    float distance = 0.f;
    math::vec3 normal = math::vec3(0, 0, 0);
    bool inside = false;
};

} // namespace raytracing::render
