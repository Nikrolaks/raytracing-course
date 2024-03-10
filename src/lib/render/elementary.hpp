#pragma once

#include "base.hpp"
#include "intersection.hpp"

#include <math/base.hpp>
#include <math/ray.hpp>

#include <cassert>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <sstream>

namespace raytracing::render {

class object {
public:
    object& operator=(const object& other) = default;

    enum ObjectType {
        DIFFUSE,
        METALLIC,
        DIELECTRIC
    };
    static std::shared_ptr<object> fromStream(std::stringstream&) { return nullptr; }

    virtual std::optional<intersection> intersect(const math::ray&) const { return std::nullopt; }

    color coloring() const { return color_; }

    ObjectType type() const { return type_; }

    float ior() const {
        assert(type_ == ObjectType::DIELECTRIC);
        return ior_;
    }

    const math::vec3& emission() const {
        return emission_;
    }

    virtual ~object() = default;

protected:
    object() = default;
    math::ray prepareRay(const math::ray& ray) const;
    virtual math::vec3 at(const math::vec3&) const { return math::vec3(); }

    math::vec3 position_ = { 0.f, 0.f, 0.f };
    math::quaternion rotation_ = math::vec4{ 0.f, 0.f, 0.f, 1.f };
    color color_{ 0.f, 0.f, 0.f };
    ObjectType type_ = ObjectType::DIFFUSE;
    float ior_ = 0.f;
    math::vec3 emission_{ 0.f, 0.f, 0.f };
};

class objectBuilder : private object {
public:
    objectBuilder() {}
    void enrich(const std::string& line);
    std::shared_ptr<object> finalize();

    void clear() {
        *this = clearInstance;
        building = nullptr;
    }
private:
    static const objectBuilder clearInstance;
    std::shared_ptr<object> building = nullptr;
};

} // namespace raytracing::render
