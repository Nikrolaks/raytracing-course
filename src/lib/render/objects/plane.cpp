#include "plane.hpp"

#include "factory.hpp"

#include <utils/utils.hpp>

namespace raytracing::render::objects {

std::shared_ptr<plane> plane::fromStream(std::stringstream& stream) {
    math::vec3 n;
    stream >> n;
    return std::make_shared<plane>(n);
}

std::optional<long double> plane::intersection(const math::ray& ray) const {
    auto prepared = prepareRay(ray);

    auto t = -(prepared.origin() * normal_) / (prepared.direction() * normal_);

    if (t < 0.f) {
        return std::nullopt;
    }
    return t;
}

REGISTRY_OBJECT_IMPL(plane, "PLANE")

} // namespace raytracing::render::objects
