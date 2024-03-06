#include "box.hpp"

#include "factory.hpp"

#include <utils/utils.hpp>

namespace raytracing::render::objects {

box::box(const math::vec3& size)
    : size_(size) {}

std::shared_ptr<box> box::fromStream(std::stringstream& stream) {
    math::vec3 s;
    stream >> s;
    return std::make_shared<box>(s);
}

std::optional<intersection> box::intersect(const math::ray& ray) const {
    auto prepared = prepareRay(ray);

    auto
        neu = adamara(-size_ - prepared.origin(), prepared.direction().invert()),
        nev = adamara(size_ - prepared.origin(), prepared.direction().invert()),
        u = min(neu, nev), v = max(neu, nev);
    auto t1 = u.max(), t2 = v.min();

    if (t1 > t2 || t2 < 0.f) {
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

math::vec3 box::at(const math::vec3& point) const {
    math::vec3 result = adamara(point, size_.invert());
    return result.majorate();
}

REGISTRY_OBJECT_IMPL(box, "BOX")

} // namespace raytracing::render::objects
