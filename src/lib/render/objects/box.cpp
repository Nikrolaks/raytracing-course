#include "box.hpp"

#include "factory.hpp"

#include <utils/utils.hpp>

namespace raytracing::render::objects {

std::shared_ptr<box> box::fromStream(std::stringstream& stream) {
    math::vec3 s;
    stream >> s;
    return std::make_shared<box>(s);
}

std::optional<long double> box::intersection(const math::ray& ray) const {
    auto prepared = prepareRay(ray);

    auto
        neu = adamara(-size_ - prepared.origin(), prepared.direction().invert()),
        nev = adamara(size_ - prepared.origin(), prepared.direction().invert()),
        u = min(neu, nev), v = max(neu, nev);
    auto t1 = u.max(), t2 = v.min();

    if (t1 > t2 || t2 < 0.f) {
        return std::nullopt;
    }

    if (t1 < 0.f) {
        t1 = t2;
    }
    return t1;
}

REGISTRY_OBJECT_IMPL(box, "BOX")

} // namespace raytracing::render::objects
