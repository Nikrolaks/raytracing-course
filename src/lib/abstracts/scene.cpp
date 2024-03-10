#include "scene.hpp"
#include <iostream>

namespace raytracing::abs {

namespace {

constexpr float INDENT = float(1e-4);

} // namespace

std::optional<Scene::SceneIntersection> Scene::intersect(const math::ray& ray, float maxDistance) {
    std::shared_ptr<render::object> nearest = nullptr;
    render::intersection last;
    last.distance = maxDistance;

    for (const auto& object : objects_) {
        auto match = object->intersect(ray);
        if (!match || match->distance > last.distance) {
            continue;
        }
        last = *match;
        nearest = object;
    }

    if (!nearest) {
        return std::nullopt;
    }

    return Scene::SceneIntersection{ last, nearest };
}

std::optional<render::color> Scene::color(const math::ray& ray) {
    if (ray.deepness() >= maxRecursionDepth_) {
        return std::nullopt;
    }

    auto result = intersect(ray);
    if (!result) {
        return std::nullopt;
    }
    auto& [info, nearest] = *result;

    math::vec3 point = ray.origin() + ray.direction() * info.distance;

    render::color ret{ 0.f, 0.f, 0.f };

    math::vec3 reflectDir;
    render::color reflectColor;

    switch (nearest->type()) {
    case render::object::ObjectType::DIFFUSE:
        reflectDir = generator_(math::GlobalRandomHolder::engine());
        if (math::dot(reflectDir, info.normal) < 0.f) {
            reflectDir *= -1.f;
        }
        reflectColor = color(ray.deeper(point + reflectDir * INDENT, reflectDir)).value_or(backgroundColor_);
        ret = nearest->emission() + math::adamara<3>(nearest->coloring(), reflectColor) * 2.f * math::dot(reflectDir, info.normal);
        break;
    case render::object::ObjectType::METALLIC:
        reflectDir =
            ray.direction() - info.normal * 2 * math::dot(info.normal, ray.direction());
        reflectDir.normalize();
        reflectColor = color(ray.deeper(point + reflectDir * INDENT, reflectDir))
            .value_or(backgroundColor_);

        ret = nearest->emission() + math::adamara<3>(nearest->coloring(), reflectColor);
        return ret;
    case render::object::ObjectType::DIELECTRIC:
        float icos = -math::dot(info.normal, ray.direction());
        float phi = 1.f;
        float psi = nearest->ior();
        if (info.inside) {
            std::swap(phi, psi);
        }
        float reflectiveness = phi / psi;
        float osin = reflectiveness * std::sqrt(1 - icos * icos);

        float effectVoice;
        float reflectionCoefficient;
        if (osin >= 1.f) {
            effectVoice = 0.f;
            reflectionCoefficient = 1.f;
        }
        else {
            float rho = std::pow((phi - psi) / (phi + psi), 2.f);
            reflectionCoefficient = rho + (1 - rho) * std::pow(1 - icos, 5.f);
            effectVoice = voicer_(math::GlobalRandomHolder::engine());
        }

        if (effectVoice < reflectionCoefficient) {
            // reflection win
            reflectDir =
                ray.direction() - info.normal * 2 * math::dot(info.normal, ray.direction());
            reflectDir.normalize();
            reflectColor = color(ray.deeper(point + reflectDir * INDENT, reflectDir))
                .value_or(backgroundColor_);

            ret = nearest->emission() + (math::vec3)((math::vec3)(reflectColor)*reflectionCoefficient);
        }
        else {
            // refraction win
            float ocos = std::sqrt(1 - osin * osin);
            float coff = phi / psi;

            auto refractionDir = ray.direction() * coff + info.normal * (coff * icos - ocos);
            refractionDir.normalize();
            auto refractColor = color(ray.deeper(point + refractionDir * INDENT, refractionDir));
            auto totalRefractColor =
                (math::vec3)(refractColor.value_or(backgroundColor_)) * (1.f - reflectionCoefficient);
            if (refractColor && !info.inside) {
                totalRefractColor = math::adamara<3>(nearest->coloring(), totalRefractColor);
            }
            ret = nearest->emission() + totalRefractColor;
        }
        break;
    }

    return ret;
}

} // namespace raytracing::abs
