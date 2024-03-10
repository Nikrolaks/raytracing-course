#include "base.hpp"

#include <cassert>

namespace raytracing::render {

namespace {

constexpr float GAMMA = 1.f / 2.2f;

math::vec3 saturate(const math::vec3& clr) {
    return {
        std::clamp(clr.x(), 0.f, 1.f),
        std::clamp(clr.y(), 0.f, 1.f),
        std::clamp(clr.z(), 0.f, 1.f)
    };
}

math::vec3 aces_tonemap(const math::vec3& v) {
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;

    return saturate(
        adamara(
            adamara(v, v * a + b),
            (adamara(v, v * c + d) + e).invert()
        )
    );
}

} // namespace

void integrableColor::enrich(const math::vec3& clr) {
    result_ += clr;
    ++bins_;
}

color integrableColor::get() const {
    assert(bins_ > 0);
    return color(result_ * (1.f / (float)(bins_)));
}

pixelColor::pixelColor(const integrableColor& clr) {
    auto corrected = math::vec3(pow(aces_tonemap(clr.get()), GAMMA));
    red = uint8_t(std::clamp(corrected.x() * 255.f, 0.f, 255.f));
    green = uint8_t(std::clamp(corrected.y() * 255.f, 0.f, 255.f));
    blue = uint8_t(std::clamp(corrected.z() * 255.f, 0.f, 255.f));
}

} // namespace raytracing::render
