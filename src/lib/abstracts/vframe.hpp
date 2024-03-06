#pragma once

#include "camera.hpp"
#include "scene.hpp"

#include <render/canvas.hpp>

#include <filesystem>

namespace raytracing::abs {

class VFrame : private Scene, Camera {
public:
    static VFrame fromGLTF(const std::filesystem::path& filepath);

    const render::Canvas& render();
private:
    VFrame() = default;

    render::Canvas canvas_;
};

} // namespace raytracing::abs
