#pragma once

#include "factory.hpp"

#include <render/elementary.hpp>

#include <math/base.hpp>

namespace raytracing::render::objects {

class plane final : public object {
public:
    plane(math::vec3 normal) : normal_(normal) {}
    static std::shared_ptr<plane> fromStream(std::stringstream& stream);

    std::optional<float> intersection(const math::ray& ray) const override;

    ~plane() override = default;
private:
    math::vec3 normal_;
};

REGISTRY_OBJECT(plane)

} // namespace raytracing::render::objects
