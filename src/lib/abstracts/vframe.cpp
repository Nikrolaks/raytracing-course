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
    MINIMAL_COMPLETE = (1 << 7) - 1
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
        if (command == "DIMENSIONS") {
            ss >> frameWidth >> frameHeight;
            result.canvas_ = render::Canvas(frameWidth, frameHeight);
            completeness |= VFrameHeaders::DIMENSIONS;
            continue;
        }
        if (command == "BG_COLOR") {
            math::vec3 vec;
            ss >> vec;
            result.backgroundColor_ = vec;
            completeness |= VFrameHeaders::BG_COLOR;
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
    }
    while (stream) {
        auto res = render::object::fromStream(stream);
        if (res) {
            result.Scene::objects_.push_back(res);
        }
    }
    assert((completeness & VFrameHeaders::MINIMAL_COMPLETE) == VFrameHeaders::MINIMAL_COMPLETE);
    result.Camera::cs_ = math::CoordSystem(cameraRight, cameraUp, cameraForward);
    float ratio = (float)(frameHeight) / frameWidth;
    result.Camera::forwardY_ = 2.f * std::atan(std::tan(result.Camera::forwardX_ * 0.5f) * ratio);
    return result;
}

const render::Canvas& VFrame::render() {
    for (size_t i = 0; i < canvas_.height(); ++i) {
        for (size_t j = 0; j < canvas_.width(); ++j) {
            canvas_.at(i, j) = Scene::color(
                Camera::sight(canvas_.relative(i, j)), backgroundColor_);
        }
    }
    return canvas_;
}

} // namespace raytracing::abs
