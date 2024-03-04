#pragma once

#include <math/base.hpp>

namespace raytracing::render {

struct color {
    float red, green, blue;

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
        red = uint8_t(clr.red * 255);
        green = uint8_t(clr.green * 255);
        blue = uint8_t(clr.blue * 255);

        return *this;
    }
};
#pragma pack(pop)

} // namespace raytracing::render
