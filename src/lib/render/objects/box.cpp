#include "box.hpp"

#include "factory.hpp"

#include <utils/utils.hpp>

namespace raytracing::render::objects {

box::box(const math::vec3& size)
    : size_(size)
{}

std::shared_ptr<box> box::fromStream(std::stringstream& stream) {
    math::vec3 s;
    stream >> s;
    return std::make_shared<box>(s);
}

std::optional<intersection> box::intersect(const math::ray& ray) const {
    auto prepared = prepareRay(ray);

    auto
        neu = adamara(-size_ - prepared.origin(), prepared.direction().invertWithNaNtoZeros()),
        nev = adamara(size_ - prepared.origin(), prepared.direction().invertWithNaNtoZeros()),
        u = min(neu, nev), v = max(neu, nev);
    auto t1 = u.max(), t2 = v.min();

    if (t1 > t2 || t2 < 0.f) {
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

math::vec3 box::at(const math::vec3& point) const {
    math::vec3 result = adamara(point, size_.invert());
    return result.majorate();
}

box::boxDistribution::boxDistribution(const box& params) 
    : params_(params)
    , w_(math::dot(params_.size_, math::vec3(1.f, 1.f, 1.f)))
    , facetGen_(0.f, w_) {}

math::vec3 box::boxDistribution::sample(const math::vec3& point, const math::vec3&) {
    float 
        u = facetGen_(math::GlobalRandomHolder::engine()),
        order = 1.f - 2.f * (float)(orderGen_(math::GlobalRandomHolder::engine())),
        ux = posGen_(math::GlobalRandomHolder::engine()),
        uy = posGen_(math::GlobalRandomHolder::engine());

    math::vec3 result;

    if (u <= params_.size_.x()) {
        result = math::vec3(order * params_.size_.x(), ux * params_.size_.y(), uy * params_.size_.z());
    }
    else if (u <= params_.size_.x() + params_.size_.y()) {
        result = math::vec3(ux * params_.size_.x(), order * params_.size_.y(), uy * params_.size_.z());
    }
    else {
        result = math::vec3(ux * params_.size_.x(), uy * params_.size_.y(), order * params_.size_.z());
    }

    return (math::rotate(result, params_.rotation_) + params_.position_ - point).normalize();
}

float box::boxDistribution::pdf(const math::vec3& point, const math::vec3&, const math::vec3& direction) {
    auto pt1maybe = params_.intersect(math::ray(point, direction));
    if (!pt1maybe) {
        return 0.f;
    }
    math::vec3 y1 = point + direction * pt1maybe->distance;
    float p1 = 0.f;
    if (pt1maybe->distance > 0.f) {
        p1 =
            (0.5f * 0.5f * 0.5f * std::abs(math::dot(params_.size_, pt1maybe->normal)) / w_)
            * (pt1maybe->distance * pt1maybe->distance) / std::abs(math::dot(direction, pt1maybe->normal));
    }
    
    auto pt2maybe = params_.intersect(math::ray(y1 + direction * 1e-4, direction));
    if (!pt2maybe) {
        return p1;
    }
    float distance2 = pt2maybe->distance + 1e-4 + pt1maybe->distance;
    float p2 =
        (0.5f * 0.5f * 0.5f * std::abs(math::dot(params_.size_, pt2maybe->normal)) / w_)
        * (distance2 * distance2) / std::abs(math::dot(direction, pt2maybe->normal));

    return p1 + p2;
}

REGISTRY_OBJECT_IMPL(box, "BOX")

} // namespace raytracing::render::objects
