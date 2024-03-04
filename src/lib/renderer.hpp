#pragma once

#include <render/base.hpp>
#include <render/canvas.hpp>
#include <abstracts/vframe.hpp>

namespace raytracing {

class Renderer {
public:
    Renderer(size_t width, size_t height) : canvas_(width, height) {}

    render::Canvas& render(const render::VFrame& scene);
private:
    abs::VFrame canvas_;
};

} // namespace raytracing
