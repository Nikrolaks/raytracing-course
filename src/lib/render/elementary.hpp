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
    static std::shared_ptr<object> fromStream(std::stringstream&) { return nullptr; }

    virtual std::optional<float> intersection(const math::ray&) const { return std::nullopt; }

    color coloring() { return clr_; }

    virtual ~object() = default;
protected:
    object() = default;
    object(const object& other)
        : position_(other.position_)
        , rotation_(other.rotation_)
        , clr_(other.clr_) {}
    math::ray prepareRay(const math::ray& ray) const;

    math::vec3 position_ = { 0, 0, 0 };
    math::quaternion rotation_ = math::vec4{ 0, 0, 0, 1 };
    color clr_;
};

class objectBuilder : private object {
public:
    void enrich(const std::string& line);
    std::shared_ptr<object> finalize();

    void clear() {
        *static_cast<object*>(this) = clearInstance;
        building = nullptr;
    }
private:
    static const objectBuilder clearInstance;
    std::shared_ptr<object> building = nullptr;
};

} // namespace raytracing::render
