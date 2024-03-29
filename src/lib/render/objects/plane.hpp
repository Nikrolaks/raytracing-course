#pragma once

#include "factory.hpp"

#include <render/elementary.hpp>

#include <math/base.hpp>

namespace raytracing::render::objects {

class plane final : public object {
public:
    plane(const math::vec3& normal);
    static std::shared_ptr<plane> fromStream(std::stringstream& stream);

    std::optional<intersection> intersect(const math::ray& ray) const override;

    bool isLight() const override { return false; }

    ~plane() override = default;
protected:
    math::vec3 at(const math::vec3& point) const override;

private:
    math::vec3 normal_;
};

REGISTRY_OBJECT(plane)

} // namespace raytracing::render::objects
