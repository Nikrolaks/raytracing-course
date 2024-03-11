#pragma once

#include <math/ray.hpp>
#include <math/global_random.hpp>
#include <render/base.hpp>
#include <render/elementary.hpp>

#include <memory>
#include <random>
#include <vector>

namespace raytracing::abs {

class Scene {
public:
    Scene() = default;
    Scene(const Scene& another) = default;
    Scene(Scene&& another) = default;
    Scene& operator=(const Scene& another) = default;
    Scene& operator=(Scene&& another) noexcept = default;

    std::optional<render::color> color(const math::ray& ray);
protected:
    struct SceneIntersection {
        render::intersection info;
        std::shared_ptr<render::object> obj;
    };
    std::optional<SceneIntersection> intersect(const math::ray& ray, float maxDistance = render::INF);
    std::vector<std::shared_ptr<render::object>> objects_;
    size_t maxRecursionDepth_;
    render::color backgroundColor_;

    std::shared_ptr<math::distribution> directionGenForDiffuse_;

    math::uniform_sphere_distribution generator_;
    std::uniform_real_distribution<float> voicer_{ 0.f, 1.f };
};

} // namespace raytracing::abs
