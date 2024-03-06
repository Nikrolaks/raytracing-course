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

    virtual ~object() = default;
protected:
    object() = default;
    object(const object& other)
        : position_(other.position_)
        , rotation_(other.rotation_)
        , color_(other.color_)
        , type_(other.type_)
        , ior_(other.ior_) {}
    math::ray prepareRay(const math::ray& ray) const;
    virtual math::vec3 at(const math::vec3&) const { return math::vec3(); }

    math::vec3 position_ = { 0, 0, 0 };
    math::quaternion rotation_ = math::vec4{ 0, 0, 0, 1 };
    color color_{0, 0, 0};
    ObjectType type_ = ObjectType::DIFFUSE;
    float ior_ = 0.f;
};

class light {
public:
    virtual math::vec3 to(const math::vec3&) const { return math::vec3(); }
    virtual float distance(const math::vec3&) const { return 0.f; }
    virtual color coloring(float) const { return intensity_; }

    virtual ~light() = default;
protected:
    light(const color& intensity) : intensity_(intensity) {}
    color intensity_;
};

class directLight final : public light {
public:
    directLight(const color& intensity, const math::vec3& direction)
        : light(intensity)
        , direction_(direction) {}

    math::vec3 to(const math::vec3&) const override { return direction_; }
    float distance(const math::vec3&) const override { return INF; }

    ~directLight() override = default;
protected:
    math::vec3 direction_;
};

class pointLight final : public light {
public:
    pointLight(
        const color& intensity,
        const math::vec3& position,
        const math::vec3& attenuation)
        : light(intensity)
        , position_(position)
        , attenuation_(attenuation) {}

    math::vec3 to(const math::vec3& point) const override {
        return (position_ - point).normalize();
    }

    float distance(const math::vec3& point) const override {
        return (position_ - point).length();
    }

    color coloring(float distance) const override {
        return math::vec3(
            (math::vec3)(light::intensity_) * (1.f / math::dot(
                attenuation_, math::vec3(1, distance, distance * distance))));
    }

    ~pointLight() override = default;
protected:
    math::vec3 position_;
    math::vec3 attenuation_;
};

class lightBuilder {
public:
    void enrich(const std::string& line);
    std::shared_ptr<light> finalize();

    void clear() {
        *this = clearInstance;
    }
private:
    static const lightBuilder clearInstance;

    enum LightHeaders {
        COMMON_INTENSITY = (1 << 0),
        DIRECT_DIRECTION = (1 << 1),
        DIRECT_MINIMAL_COMPLETE = DIRECT_DIRECTION | COMMON_INTENSITY,
        POINT_POSITION = (1 << 2),
        POINT_ATTENUATION = (1 << 3),
        POINT_MINIMAL_COMPLETE = POINT_POSITION | POINT_ATTENUATION | COMMON_INTENSITY
    };

    size_t completeness_ = 0;
    // zaebalo, delau vtupuy
    color intensity_;
    math::vec3 direction_;
    math::vec3 position_;
    math::vec3 attenuation_;
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
