#pragma once

#include "factory.hpp"

#include <render/elementary.hpp>

#include <math/base.hpp>

namespace raytracing::render::objects {

class box final : public object {
public:
    box(const math::vec3& size);
    static std::shared_ptr<box> fromStream(std::stringstream& stream);

    std::optional<intersection> intersect(const math::ray& ray) const override;

    ~box() override = default;
protected:
    math::vec3 at(const math::vec3& point) const override;

private:
    math::vec3 size_;
};

REGISTRY_OBJECT(box)

} // namespace raytracing::render::objects
