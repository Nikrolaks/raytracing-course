#pragma once

#include "base.hpp"

#define _USE_MATH_DEFINES
#include "math.h"
#include <random>


namespace raytracing::math {

class uniform_sphere_distribution {
public:
    uniform_sphere_distribution()
        : angle_(0.f, 2.f * float(M_PI))
        , z_(-1.f, 1.f) {}

    template<class Generator>
    vec3 operator()(Generator& g) {
        float angle = angle_(g), z = z_(g), height = std::sqrt(1.f - z * z);

        return { height * std::cos(angle), height * std::sin(angle), z };
    }
private:
    std::uniform_real_distribution<float> angle_;
    std::uniform_real_distribution<float> z_;
};

class GlobalRandomHolder {
public:
    static std::minstd_rand& engine() { return engine_; }
private:
    static GlobalRandomHolder instance_;
    static thread_local std::minstd_rand engine_;
};

} // namespace raytracing::math
