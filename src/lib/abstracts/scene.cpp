#include "scene.hpp"

namespace raytracing::abs {

render::color Scene::color(const math::ray& ray, const render::color& alternative) {
    render::color result = alternative;
    long double distance = 1e12;
    for (const auto& object : objects_) {
        auto point = object->intersection(ray);
        if (point && *point < distance) {
            result = object->coloring();
            distance = *point;
        }
    }

    return result;
}

} // namespace raytracing::abs
