#pragma once

#include <math/ray.hpp>
#include <render/base.hpp>
#include <render/elementary.hpp>

#include <memory>
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
    std::optional<SceneIntersection> intersect(
        const math::ray& ray, float maxDistance = render::INF, bool onlyExisting = false);
    std::vector<std::shared_ptr<render::object>> objects_;
    std::vector<std::shared_ptr<render::light>> lights_;
    size_t maxRecursionDepth_;
    render::color backgroundColor_;
    render::color ambientLight_;
};

} // namespace raytracing::abs
