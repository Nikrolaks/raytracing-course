#pragma once

#include "factory.hpp"

#include <render/elementary.hpp>

#include <math/base.hpp>

namespace raytracing::render::objects {

class box : public object {
public:
    box(math::vec3 size) : size_(size) {}
    static std::shared_ptr<box> fromStream(std::stringstream& stream);

    std::optional<long double> intersection(const math::ray& ray) const override;
private:
    math::vec3 size_;
};

REGISTRY_OBJECT(box)

} // namespace raytracing::render::objects
