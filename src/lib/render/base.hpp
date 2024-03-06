#pragma once

#include <math/base.hpp>

namespace raytracing::render {

constexpr float INF = 1e9;

struct color {
    float red, green, blue;

    color() = default;
    color(float r, float g, float b) : red(r), green(g), blue(b) {}
    color(const math::vec3& v) : red(v.x()), green(v.y()), blue(v.z()) {}

    color& operator=(const math::vec3& v) {
        red = v.x();
        green = v.y();
        blue = v.z();

        return *this;
    }

    operator math::vec3() const {
        return { red, green, blue };
    }
};

#pragma pack(push, 1)
struct pixelColor {
    uint8_t red, green, blue;

    pixelColor& operator=(const color& clr);
};
#pragma pack(pop)

} // namespace raytracing::render
