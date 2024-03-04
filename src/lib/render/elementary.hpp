#pragma once

#include "base.hpp"

#include <math/base.hpp>
#include <math/ray.hpp>

#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <sstream>

namespace raytracing::render {

class object {
public:
    static std::shared_ptr<object> fromStream(std::stringstream& stream);

    virtual std::optional<float> intersection(const math::ray&) const { return std::nullopt; }

    color coloring() { return clr_; }

protected:
    math::ray prepareRay(const math::ray& ray) const;

    math::vec3 position_ = { 0, 0, 0 };
    math::vec4 rotation_ = { 0, 0, 0, 1 };
    color clr_;
};

} // namespace raytracing::render
