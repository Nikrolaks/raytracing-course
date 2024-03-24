#pragma once

#include "factory.hpp"

#include <render/elementary.hpp>

#include <math/base.hpp>

namespace raytracing::render::objects {

class box final : public object, math::distribution {
public:
    box(const math::vec3& size);
    static std::shared_ptr<box> fromStream(std::stringstream& stream);

    std::optional<intersection> intersect(const math::ray& ray) const override;

    std::shared_ptr<math::distribution> distribution() override {
        (void)(object::distribution()); // just assert
        return std::make_shared<boxDistribution>(*this);
    }

    ~box() override = default;
protected:
    math::vec3 at(const math::vec3& point) const override;

private:
    class boxDistribution : public distribution {
    public:
        boxDistribution(const box& params);
        math::vec3 sample(const math::vec3& point, const math::vec3& normal) override;
        float pdf(const math::vec3& point, const math::vec3& normal, const math::vec3& direction) override;
    private:
        const box& params_;
        float w_;
        float helpfulValue_;
        std::uniform_real_distribution<float> facetGen_;
        std::uniform_int_distribution<int> orderGen_{ 0, 1 };
        std::uniform_real_distribution<float> posGen_{ -1.f, 1.f };
    };

    math::vec3 size_;
};

REGISTRY_OBJECT(box)

} // namespace raytracing::render::objects
