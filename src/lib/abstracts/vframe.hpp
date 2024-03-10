#pragma once

#include "camera.hpp"
#include "scene.hpp"

#include <render/canvas.hpp>

#include <filesystem>

namespace raytracing::abs {

class VFrame : private Scene, Camera {
public:
    static VFrame fromGLTF(const std::filesystem::path& filepath);

    render::Canvas render();
private:
    VFrame() = default;

    size_t samplesCount_;
    size_t canvasWidth_, canvasHeight_;
};

} // namespace raytracing::abs
