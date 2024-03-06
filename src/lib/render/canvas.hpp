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
    Canvas(const Canvas& other)
        : width_(other.width_)
        , height_(other.height_)
        , data_(other.data_) {}
    Canvas(Canvas&& other) noexcept
        : width_(other.width_)
        , height_(other.height_)
        , data_(std::move(other.data_)) {}
    Canvas& operator=(const Canvas& other) {
        width_ = other.width_;
        height_ = other.height_;
        data_ = other.data_;
        return *this;
    }
    Canvas& operator=(Canvas&& other) noexcept {
        width_ = other.width_;
        height_ = other.height_;
        data_ = std::move(other.data_);
        return *this;
    }

    render::pixelColor& at(size_t i, size_t j);

    math::vec2 relative(size_t i, size_t j);

    size_t width() const { return width_; }
    size_t height() const { return height_; }

    const std::vector<render::pixelColor>& data() const {
        return data_;
    }

    static void toPPM(std::filesystem::path file, const Canvas& canvas);

private:
    size_t width_ = 0, height_ = 0;
    std::vector<render::pixelColor> data_;
};

} // namespace raytracing::render
