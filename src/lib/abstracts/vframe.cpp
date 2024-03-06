#include "vframe.hpp"

#include <utils/utils.hpp>

#include <cassert>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>

namespace raytracing::abs {

namespace {

enum VFrameHeaders {
    DIMENSIONS = (1 << 0),
    BG_COLOR = (1 << 1),
    CAMERA_POSITION = (1 << 2),
    CAMERA_RIGHT = (1 << 3),
    CAMERA_UP = (1 << 4),
    CAMERA_FORWARD = (1 << 5),
    CAMERA_FOV_X = (1 << 6),
    AMBIENT_LIGHT = (1 << 7),
    RAY_DEPTH = (1 << 8),

    MINIMAL_COMPLETE = (1 << 9) - 1
};

} // namespace

VFrame VFrame::fromGLTF(const std::filesystem::path& file) {
    VFrame result;
    size_t frameWidth = 0, frameHeight = 0;
    math::vec3 cameraRight, cameraUp, cameraForward;
    size_t completeness = 0;
    std::ifstream entry(file);
    std::stringstream stream(std::string{
        std::istreambuf_iterator<char>(entry),
        std::istreambuf_iterator<char>()});
    std::string line;
    bool newPrimitive = false, newLight = false;
    render::objectBuilder bObject;
    render::lightBuilder bLight;

    while (std::getline(stream, line)) {
        if (!line.size()) {
            continue;
        }
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        if (command == "NEW_PRIMITIVE" || command == "NEW_LIGHT") {
            if (newPrimitive) {
                auto res = bObject.finalize();
                if (res) {
                    result.objects_.push_back(res);
                }
                bObject.clear();
            }
            if (newLight) {
                auto res = bLight.finalize();
                if (res) {
                    result.lights_.push_back(res);
                }
                bLight.clear();
            }
            newPrimitive = command == "NEW_PRIMITIVE";
            newLight = command == "NEW_LIGHT";
            continue;
        }
        if (command == "DIMENSIONS") {
            ss >> frameWidth >> frameHeight;
            result.canvas_ = render::Canvas(frameWidth, frameHeight);
            completeness |= VFrameHeaders::DIMENSIONS;
            continue;
        }
        if (command == "RAY_DEPTH") {
            ss >> result.maxRecursionDepth_;
            completeness |= VFrameHeaders::RAY_DEPTH;
            continue;
        }
        if (command == "BG_COLOR") {
            math::vec3 vec;
            ss >> vec;
            result.backgroundColor_ = vec;
            completeness |= VFrameHeaders::BG_COLOR;
            continue;
        }
        if (command == "AMBIENT_LIGHT") {
            math::vec3 vec;
            ss >> vec;
            result.ambientLight_ = vec;
            completeness |= VFrameHeaders::AMBIENT_LIGHT;
            continue;
        }
        if (command == "CAMERA_POSITION") {
            ss >> result.Camera::position_;
            completeness |= VFrameHeaders::CAMERA_POSITION;
            continue;
        }
        if (command == "CAMERA_RIGHT") {
            ss >> cameraRight;
            completeness |= VFrameHeaders::CAMERA_RIGHT;
            continue;
        }
        if (command == "CAMERA_UP") {
            ss >> cameraUp;
            completeness |= VFrameHeaders::CAMERA_UP;
            continue;
        }
        if (command == "CAMERA_FORWARD") {
            ss >> cameraForward;
            completeness |= VFrameHeaders::CAMERA_FORWARD;
            continue;
        }
        if (command == "CAMERA_FOV_X") {
            ss >> result.Camera::forwardX_;
            completeness |= VFrameHeaders::CAMERA_FOV_X;
            continue;
        }
        if (newPrimitive) {
            bObject.enrich(line);
            continue;
        }
        if (newLight) {
            bLight.enrich(line);
            continue;
        }
    }
    if (newPrimitive) {
        auto res = bObject.finalize();
        if (res) {
            result.objects_.push_back(res);
        }
        bObject.clear();
    }
    if (newLight) {
        auto res = bLight.finalize();
        if (res) {
            result.lights_.push_back(res);
        }
        bLight.clear();
    }
    (void)(completeness); // macOS ti che rugaeshsya na unused???
    assert((completeness & VFrameHeaders::MINIMAL_COMPLETE) == VFrameHeaders::MINIMAL_COMPLETE);
    result.Camera::cs_ = math::CoordSystem(cameraRight, cameraUp, cameraForward);
    float ratio = (float)(frameHeight) / frameWidth;
    result.Camera::forwardY_ = 2.f * std::atan(std::tan(result.Camera::forwardX_ * 0.5f) * ratio);
    return result;
}

const render::Canvas& VFrame::render() {
    for (size_t i = 0; i < canvas_.height(); ++i) {
        for (size_t j = 0; j < canvas_.width(); ++j) {
            canvas_.at(i, j) = 
                Scene::color(
                    Camera::sight(canvas_.relative(i, j)))
                .value_or(Scene::backgroundColor_);
        }
    }
    return canvas_;
}

} // namespace raytracing::abs
