#pragma once

#include "factory.hpp"

#include <render/elementary.hpp>

#include <math/base.hpp>

namespace raytracing::render::objects {

class ellipsoid final : public object {
public:
    ellipsoid(math::vec3 radius) : radius_(radius) {}
    static std::shared_ptr<ellipsoid> fromStream(std::stringstream& stream);

    std::optional<intersection> intersect(const math::ray& ray) const override;

    ~ellipsoid() override = default;
protected:
    math::vec3 at(const math::vec3& point) const override;

private:
    math::vec3 radius_;
};

REGISTRY_OBJECT(ellipsoid)

} // namespace raytracing::render::objects
