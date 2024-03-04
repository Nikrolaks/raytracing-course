#include "elementary.hpp"

#include <render/objects/factory.hpp>
#include <utils/utils.hpp>

namespace raytracing::render {

std::shared_ptr<object> object::fromStream(std::stringstream& stream) {
    object base;
    std::shared_ptr<object> result;
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.size()) {
            continue;
        }
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        if (command == "NEW_PRIMITIVE") {
            break;
        }
        if (command == "POSITION") {
            ss >> base.position_;
            continue;
        }
        if (command == "ROTATION") {
            math::vec4 rotation;
            ss >> rotation;
            base.rotation_ = rotation.normalize();
            continue;
        }
        if (command == "COLOR") {
            math::vec3 vec;
            ss >> vec;
            base.clr_ = vec;
            continue;
        }
        if (!result) {
            result = objects::objectFactory::generate(std::stringstream(line));
        }
    }
    if (result) {
        *result = base; // inheritance do the job
    }
    return result;
}

math::ray object::prepareRay(const math::ray& ray) const {
    math::ray result(ray);
    return result.translate(-position_).rotate(rotation_.conjugate());
}

} // namespace raytracing::render
