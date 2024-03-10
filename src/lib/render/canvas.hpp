#pragma once

#include <math/base.hpp>
#include <render/base.hpp>

#include <vector>
#include <filesystem>

namespace raytracing::render {

class Canvas {
public:
    Canvas() = default;
    Canvas(size_t width, size_t height)
        : width_(width)
        , height_(height)
        , data_(width * height) {}
    Canvas(const Canvas& other) = default;
    Canvas(Canvas&& other) noexcept = default;
    Canvas& operator=(const Canvas& other) = default;
    Canvas& operator=(Canvas&& other) noexcept = default;

    render::integrableColor& at(size_t i, size_t j);

    math::vec2 relative(size_t i, size_t j, float xCorrectionToNonAtom = 0.0, float yCorrectionToNonAtom = 0.0);

    size_t width() const { return width_; }
    size_t height() const { return height_; }

    // maybe will add ability to continue enriching picture
    std::vector<render::pixelColor> data() const {
        return { data_.begin(), data_.end() };
    }

    static void toPPM(std::filesystem::path file, const Canvas& canvas);

private:
    size_t width_ = 0, height_ = 0;
    std::vector<render::integrableColor> data_;
};

} // namespace raytracing::render
