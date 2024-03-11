#include "ellipsoid.hpp"

#include "factory.hpp"

#include <utils/utils.hpp>

namespace raytracing::render::objects {

ellipsoid::ellipsoid(const math::vec3& radius) : radius_(radius) {}

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

ellipsoid::ellipsoidDistribution::ellipsoidDistribution(const ellipsoid& params)
    : params_(params)
    , r_(params_.radius_.x() * params_.radius_.y() * params_.radius_.z())
{}

math::vec3 ellipsoid::ellipsoidDistribution::sample(const math::vec3& point, const math::vec3&) {
    return 
        (math::rotate(math::adamara(gen_(math::GlobalRandomHolder::engine()), params_.radius_), params_.rotation_)
            + params_.position_ - point).normalize();
}

float ellipsoid::ellipsoidDistribution::pdf(const math::vec3& point, const math::vec3&, const math::vec3& direction) {
    auto pt1maybe = params_.intersect(math::ray(point, direction));
    if (!pt1maybe) {
        return 0.f;
    }
    math::vec3
        y1 = point + direction * pt1maybe->distance,
        y1c = math::adamara(math::rotate(y1 - params_.position_, params_.rotation_.conjugate()), params_.radius_.invert()), // Nx, Ny, Nz
        spec1 = math::adamara(math::adamara(math::vec3(r_, r_, r_), params_.radius_.invert()), y1c); // NxRyRz, RxNyRz, RxRyNz

    float p1 =
        (0.5f * 0.5f * 0.5f / (4.f * (float)(M_PI) * spec1.length()))
        * (pt1maybe->distance * pt1maybe->distance) / std::abs(math::dot(direction, pt1maybe->normal));

    auto pt2maybe = params_.intersect(math::ray(y1 + direction * 1e-4, direction));
    if (!pt2maybe) {
        return p1;
    }
    math::vec3 y2 = y1 + direction * (1e-4 + pt2maybe->distance),
        y2c = math::adamara(math::rotate(y2 - params_.position_, params_.rotation_.conjugate()), params_.radius_.invert()), // Nx, Ny, Nz
        spec2 = math::adamara(math::adamara(math::vec3(r_, r_, r_), params_.radius_.invert()), y2c); // NxRyRz, RxNyRz, RxRyNz;
    float distance2 = pt2maybe->distance + 1e-4 + pt1maybe->distance;
    float p2 =
        (0.5f * 0.5f * 0.5f / (4.f * (float)(M_PI) * spec2.length()))
        * (distance2 * distance2) / std::abs(math::dot(direction, pt2maybe->normal));

    return p1 + p2;
}

REGISTRY_OBJECT_IMPL(ellipsoid, "ELLIPSOID")

} // namespace raytracing::render::objects
