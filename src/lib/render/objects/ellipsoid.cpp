#include "ellipsoid.hpp"

#include "factory.hpp"

#include <utils/utils.hpp>

namespace raytracing::render::objects {

std::shared_ptr<ellipsoid> ellipsoid::fromStream(std::stringstream& stream) {
    math::vec3 r;
    stream >> r;
    return std::make_shared<ellipsoid>(r);
}

std::optional<float> ellipsoid::intersection(const math::ray& ray) const {
    auto prepared = prepareRay(ray);

    auto
        neradius = radius_.invert(),
        u = adamara(prepared.origin(), neradius),
        v = adamara(prepared.direction(), neradius);

    auto
        a = v.length2(),
        b = dot(u, v),
        c = u.length2(),
        d = b * b - a * (c - 1.f);

    if (d < 0.f) {
        return std::nullopt;
    }

    d = std::sqrt(d);
    auto 
        t1 = (-b - d) / a,
        t2 = (-b + d) / a;

    if (t2 < 0.f) {
        return std::nullopt;
    }

    if (t1 < 0.f) {
        t1 = t2;
    }
    return t1;
}

REGISTRY_OBJECT_IMPL(ellipsoid, "ELLIPSOID")

} // namespace raytracing::render::objects
