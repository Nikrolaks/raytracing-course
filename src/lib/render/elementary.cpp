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
    if (command == "IOR") {
        stream >> ior_;
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

const lightBuilder lightBuilder::clearInstance{};

void lightBuilder::enrich(const std::string& line) {
    std::stringstream stream(line);
    std::string command;
    stream >> command;
    if (command == "LIGHT_INTENSITY") {
        math::vec3 v;
        stream >> v;
        intensity_ = v;
        completeness_ |= LightHeaders::COMMON_INTENSITY;
        return;
    }
    if (command == "LIGHT_DIRECTION") {
        stream >> direction_;
        direction_.normalize();
        completeness_ |= LightHeaders::DIRECT_DIRECTION;
        return;
    }
    if (command == "LIGHT_POSITION") {
        stream >> position_;
        completeness_ |= LightHeaders::POINT_POSITION;
        return;
    }
    if (command == "LIGHT_ATTENUATION") {
        stream >> attenuation_;
        completeness_ |= LightHeaders::POINT_ATTENUATION;
        return;
    }
}

std::shared_ptr<light> lightBuilder::finalize() {
    light* ret = nullptr;
    if ((completeness_ & LightHeaders::DIRECT_MINIMAL_COMPLETE)
        == LightHeaders::DIRECT_MINIMAL_COMPLETE) {
        ret = new directLight(intensity_, direction_);
    }
    else if ((completeness_ & LightHeaders::POINT_MINIMAL_COMPLETE)
        == LightHeaders::POINT_MINIMAL_COMPLETE) {
        ret = new pointLight(intensity_, position_, attenuation_);
    }
    
    return std::shared_ptr<light>(ret);
}

} // namespace raytracing::render
