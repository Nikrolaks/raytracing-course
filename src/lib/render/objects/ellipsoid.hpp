#pragma once

#include "factory.hpp"

#include <render/elementary.hpp>

#include <math/base.hpp>

namespace raytracing::render::objects {

class ellipsoid : public object {
public:
    ellipsoid(math::vec3 radius) : radius_(radius) {}
    static std::shared_ptr<ellipsoid> fromStream(std::stringstream& stream);

    std::optional<long double> intersection(const math::ray& ray) const override;
private:
    math::vec3 radius_;
};

REGISTRY_OBJECT(ellipsoid)

} // namespace raytracing::render::objects
