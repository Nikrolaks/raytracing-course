#include "elementary.hpp"

#include <render/objects/factory.hpp>
#include <utils/utils.hpp>

namespace raytracing::render {

math::ray object::prepareRay(const math::ray& ray) const {
    math::ray result(ray);
    return result.translate(-position_).rotate(rotation_.conjugate());
}

const objectBuilder objectBuilder::clearInstance{};

void objectBuilder::enrich(const std::string& line) {
    std::stringstream stream(line);
    std::string command;
    stream >> command;
    if (command == "POSITION") {
        stream >> object::position_;
        return;
    }
    if (command == "ROTATION") {
        math::vec4 rotation;
        stream >> rotation;
        object::rotation_ = rotation.normalize();
        return;
    }
    if (command == "COLOR") {
        math::vec3 vec;
        stream >> vec;
        object::color_ = vec;
        return;
    }
    if (command == "METALLIC") {
        type_ = ObjectType::METALLIC;
        return;
    }
    if (command == "DIELECTRIC") {
        type_ = ObjectType::DIELECTRIC;
        return;
    }
    if (command == "EMISSION") {
        stream >> object::emission_;
    }
    if (command == "IOR") {
        stream >> object::ior_;
        return;
    }
    if (!building) {
        building = objects::objectFactory::generate(std::stringstream(line));
    }
}

std::shared_ptr<object> objectBuilder::finalize() {
    if (building) {
        *building = *static_cast<object *>(this); // inheritance do the job
    }
    return building;
}

} // namespace raytracing::render
