#include "scene.hpp"
#include <iostream>

namespace raytracing::abs {

namespace {

constexpr float INDENT = float(1e-4);

} // namespace

std::optional<Scene::SceneIntersection> Scene::intersect(
    const math::ray& ray, float maxDistance, bool onlyExisting) {

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
        if (onlyExisting) {
            break;
        }
    }

    if (!nearest) {
        return std::nullopt;
    }

    return Scene::SceneIntersection{ last, nearest };
}

std::optional<render::color> Scene::color(const math::ray& ray) {
    if (ray.deepness() >= maxRecursionDepth_) {
        return render::color{0.f, 0.f, 0.f}; // total black
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
        ret = math::adamara<3>(nearest->coloring(), ambientLight_);
        break;
    case render::object::ObjectType::METALLIC:
        reflectDir =
            ray.direction() - info.normal * 2 * math::dot(info.normal, ray.direction());
        reflectDir.normalize();
        reflectColor = color(ray.deeper(point + reflectDir * INDENT, reflectDir))
            .value_or(backgroundColor_);

        ret = math::adamara<3>(nearest->coloring(), reflectColor);
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

        float reflectionCoefficient;
        if (osin >= 1.f) {
            reflectionCoefficient = 1.f;
        }
        else {
            float rho = std::pow((phi - psi) / (phi + psi), 2.f);
            reflectionCoefficient = rho + (1 - rho) * std::pow(1 - icos, 5.f);
        }

        // reflection
        reflectDir =
            ray.direction() - info.normal * 2 * math::dot(info.normal, ray.direction());
        reflectDir.normalize();
        reflectColor = color(ray.deeper(point + reflectDir * INDENT, reflectDir))
            .value_or(backgroundColor_);

        ret = (math::vec3)((math::vec3)(reflectColor) * reflectionCoefficient);

        if (osin >= 1.f) {
            break;
        }

        // refraction
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
        ret = (math::vec3)(ret) + totalRefractColor;
        return ret;
    }

    for (const auto& light : lights_) {
        math::vec3 dirToLight = light->to(point);
        float distance = light->distance(point);
        auto blocking = intersect(
            math::ray(point + dirToLight * INDENT, dirToLight), distance, true);
        if (blocking) {
            continue;
        }
        auto totalColor =
            math::adamara<3>(nearest->coloring(), light->coloring(distance))
            * std::max(0.f, math::dot(dirToLight, info.normal));
        ret = (math::vec3)(ret) + totalColor;
    }

    return ret;
}

} // namespace raytracing::abs
