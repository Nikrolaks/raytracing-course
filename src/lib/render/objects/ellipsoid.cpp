#include "ellipsoid.hpp"

#include "factory.hpp"

#include <utils/utils.hpp>

namespace raytracing::render::objects {

ellipsoid::ellipsoid(const math::vec3& radius)
    : radius_(radius) {}

std::shared_ptr<ellipsoid> ellipsoid::fromStream(std::stringstream& stream) {
    math::vec3 r;
    stream >> r;
    return std::make_shared<ellipsoid>(r);
}

std::optional<intersection> ellipsoid::intersect(const math::ray& ray) const {
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

    intersection result;
    result.distance = t1;
    result.color = color_;

    if (t1 < 0.f) {
        result.inside = true;
        result.distance = t2;
    }

    result.normal = at(prepared.origin() + prepared.direction() * result.distance);
    if (result.inside) {
        result.normal *= -1.f;
    }
    result.normal = rotate(result.normal, rotation_);

    return result;
}

math::vec3 ellipsoid::at(const math::vec3& point) const {
    return adamara(point, adamara(radius_, radius_).invert()).normalize();
}

REGISTRY_OBJECT_IMPL(ellipsoid, "ELLIPSOID")

} // namespace raytracing::render::objects
