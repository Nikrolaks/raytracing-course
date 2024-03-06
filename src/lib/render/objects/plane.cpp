#include "plane.hpp"

#include "factory.hpp"

#include <utils/utils.hpp>

namespace raytracing::render::objects {

size_t Counter<plane>::count_ = 0;

plane::plane(const math::vec3& normal)
    : object(Counter<plane>::birth("PLANE"))
    , normal_(normal) { normal_.normalize(); }

std::shared_ptr<plane> plane::fromStream(std::stringstream& stream) {
    math::vec3 n;
    stream >> n;
    return std::make_shared<plane>(n);
}

std::optional<intersection> plane::intersect(const math::ray& ray) const {
    auto prepared = prepareRay(ray);

    auto t = -dot(prepared.origin(), normal_) / dot(prepared.direction(), normal_);

    if (t < 0.f) {
        return std::nullopt;
    }
    
    intersection result;
    result.distance = t;
    result.color = color_;
    result.normal = normal_;
    if (dot(ray.direction(), normal_) > 0.f) {
        result.inside = true;
        result.normal *= -1.f;
    }
    result.normal = rotate(result.normal, rotation_);

    return result;
}

math::vec3 plane::at(const math::vec3&) const {
    return normal_;
}

REGISTRY_OBJECT_IMPL(plane, "PLANE")

} // namespace raytracing::render::objects
