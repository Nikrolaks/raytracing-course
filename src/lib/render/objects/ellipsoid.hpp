#pragma once

#include "factory.hpp"

#include <render/elementary.hpp>

#include <math/base.hpp>

namespace raytracing::render::objects {

class ellipsoid final : public object, math::distribution {
public:
    ellipsoid(const math::vec3& radius);
    static std::shared_ptr<ellipsoid> fromStream(std::stringstream& stream);

    std::optional<intersection> intersect(const math::ray& ray) const override;

    std::shared_ptr<math::distribution> distribution() override {
        (void)(object::distribution()); // just assert
        return std::make_shared<ellipsoidDistribution>(*this);
    }

    ~ellipsoid() override = default;
protected:
    math::vec3 at(const math::vec3& point) const override;

private:
    class ellipsoidDistribution : public math::distribution {
    public:
        ellipsoidDistribution(const ellipsoid& params);
        math::vec3 sample(const math::vec3& point, const math::vec3& normal) override;
        float pdf(const math::vec3& point, const math::vec3& normal, const math::vec3& direction) override;
    private:
        const ellipsoid& params_;
        float r_;
        math::vec3 helpfulVec3_;
        math::uniform_sphere_distribution gen_;
    };

    math::vec3 radius_;
};

REGISTRY_OBJECT(ellipsoid)

} // namespace raytracing::render::objects
