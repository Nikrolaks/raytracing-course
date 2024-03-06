#pragma once

#include "factory.hpp"

#include <render/elementary.hpp>

#include <math/base.hpp>

namespace raytracing::render::objects {

class box final : public object {
public:
    box(math::vec3 size) : size_(size) {}
    static std::shared_ptr<box> fromStream(std::stringstream& stream);

    std::optional<float> intersection(const math::ray& ray) const override;

    ~box() override = default;
private:
    math::vec3 size_;
};

REGISTRY_OBJECT(box)

} // namespace raytracing::render::objects
