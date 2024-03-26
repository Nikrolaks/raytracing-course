#pragma once

#include <math/base.hpp>
#include <math/global_random.hpp>

#include <initializer_list>
#include <memory>
#include <vector>

namespace raytracing::math {

class distribution {
public:
    virtual vec3 sample(const vec3&, const vec3&) { return vec3(); }
    virtual float pdf(const vec3&, const vec3&, const vec3&) { return 0.f; }
};

namespace distributions {

class cosine : public distribution {
public:
    vec3 sample(const vec3& point, const vec3& normal) override;
    float pdf(const vec3& point, const vec3& normal, const vec3& direction) override;

private:
    uniform_sphere_distribution generator_;
};

class mix : public distribution {
public:
    mix(std::vector<std::shared_ptr<distribution>>&& list);
    vec3 sample(const vec3& point, const vec3& normal) override;
    float pdf(const vec3& point, const vec3& normal, const vec3& direction) override;

private:
    std::vector<std::shared_ptr<distribution>> distributions_;
    std::uniform_int_distribution<size_t> chooser_;
};

} // namespace distributions

} // namespace raytracing::math::distributions
