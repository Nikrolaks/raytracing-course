#pragma once

#include <math/base.hpp>

namespace raytracing::render {

struct color {
    long double red, green, blue;

    color& operator=(const math::vec3& vec) {
        red = vec.x();
        green = vec.y();
        blue = vec.z();

        return *this;
    }
};

#pragma pack(push, 1)
struct pixelColor {
    uint8_t red, green, blue;

    pixelColor& operator=(const color& clr) {
        red = clr.red * 255;
        green = clr.green * 255;
        blue = clr.blue * 255;

        return *this;
    }
};
#pragma pack(pop)

} // namespace raytracing::render
