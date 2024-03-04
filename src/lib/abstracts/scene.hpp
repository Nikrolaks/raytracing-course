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
    Scene(const Scene& another) : objects_(another.objects_) {}
    Scene(Scene&& another) : objects_(std::move(another.objects_)) {}
    Scene& operator=(const Scene& another) { objects_ = another.objects_; return *this; }
    Scene& operator=(Scene&& another) { objects_ = std::move(another.objects_); return *this; }

    render::color color(const math::ray& ray, const render::color& alternative);
protected:
    std::vector<std::shared_ptr<render::object>> objects_;
    size_t maxRecursionDepth_;
};

} // namespace raytracing::abs
