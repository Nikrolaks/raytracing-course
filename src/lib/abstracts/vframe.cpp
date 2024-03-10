#include "vframe.hpp"

#include <math/global_random.hpp>
#include <utils/utils.hpp>

#include <cassert>
#include <fstream>
#include <iterator>
#include <random>
#include <string>
#include <sstream>

namespace raytracing::abs {

namespace {

enum VFrameHeaders {
    DIMENSIONS = (1LL << 0),
    BG_COLOR = (1LL << 1),
    CAMERA_POSITION = (1LL << 2),
    CAMERA_RIGHT = (1LL << 3),
    CAMERA_UP = (1LL << 4),
    CAMERA_FORWARD = (1LL << 5),
    CAMERA_FOV_X = (1LL << 6),
    RAY_DEPTH = (1LL << 7),
    SAMPLES = (1LL << 8),

    MINIMAL_COMPLETE = (1LL << 9) - 1
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
    bool newPrimitive = false;
    render::objectBuilder bObject;
    while (std::getline(stream, line)) {
        if (!line.size()) {
            continue;
        }
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        if (command == "NEW_PRIMITIVE") {
            if (newPrimitive) {
                auto res = bObject.finalize();
                if (res) {
                    result.objects_.push_back(res);
                }
                bObject.clear();
            }
            newPrimitive = true;
            continue;
        }
        if (command == "DIMENSIONS") {
            ss >> result.canvasWidth_ >> result.canvasHeight_;
            completeness |= VFrameHeaders::DIMENSIONS;
            continue;
        }
        if (command == "RAY_DEPTH") {
            ss >> result.maxRecursionDepth_;
            completeness |= VFrameHeaders::RAY_DEPTH;
            continue;
        }
        if (command == "SAMPLES") {
            ss >> result.samplesCount_;
            completeness |= VFrameHeaders::SAMPLES;
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
        if (newPrimitive) {
            bObject.enrich(line);
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
    (void)(completeness); // macOS ti che rugaeshsya na unused???
    assert((completeness & VFrameHeaders::MINIMAL_COMPLETE) == VFrameHeaders::MINIMAL_COMPLETE);
    result.Camera::cs_ = math::CoordSystem(cameraRight, cameraUp, cameraForward);
    float ratio = (float)(result.canvasHeight_) / result.canvasWidth_;
    result.Camera::forwardY_ = 2.f * std::atan(std::tan(result.Camera::forwardX_ * 0.5f) * ratio);
    return result;
}

render::Canvas VFrame::render() {
    std::uniform_real_distribution pixelFormGenerator(-0.5f, 0.5f);
    render::Canvas canvas(canvasWidth_, canvasHeight_);
#pragma omp parallel for shared(pixelFormGenerator, canvas) collapse(3)
    for (int64_t i = 0; i < (int64_t)(canvas.height()); ++i) {
        for (int64_t j = 0; j < (int64_t)(canvas.width()); ++j) {
            for (int64_t sample = 0; sample < (int64_t)(samplesCount_); ++sample) {
                auto
                    dx = pixelFormGenerator(math::GlobalRandomHolder::engine()),
                    dy = pixelFormGenerator(math::GlobalRandomHolder::engine());
                canvas.at(i, j).enrich(
                    Scene::color(
                        Camera::sight(canvas.relative(i, j, dx, dy)))
                    .value_or(Scene::backgroundColor_));
            }
        }
    }
    return canvas;
}

} // namespace raytracing::abs
