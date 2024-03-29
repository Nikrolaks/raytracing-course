#include "canvas.hpp"

#include <cassert>
#include <fstream>

namespace raytracing::render {

render::integrableColor& Canvas::at(size_t i, size_t j) {
    assert(i < height_ && j < width_);
    return data_[i * width_ + j];
}

math::vec2 Canvas::relative(size_t i, size_t j, float xCorrectionToNonAtom, float yCorrectionToNonAtom) {
    assert(i < height_ && j < width_);
    assert(-0.5f <= xCorrectionToNonAtom && xCorrectionToNonAtom <= 0.5f
        && -0.5f <= yCorrectionToNonAtom && yCorrectionToNonAtom <= 0.5f);
    return math::vec2{
        (2.f * (float(j) + 0.5f + xCorrectionToNonAtom) / width_) - 1.f,
        -(2.f * (float(i) + 0.5f + yCorrectionToNonAtom) / height_) + 1.f
    };
}

void Canvas::toPPM(std::filesystem::path file, const Canvas& canvas) { 
    std::ofstream stream(file, std::ios::binary);
    stream << "P6\n";
    stream << canvas.width_ << " " << canvas.height_ << "\n";
    stream << "255\n";
    std::copy_n(
        reinterpret_cast<const uint8_t *>(canvas.data().data()),
        canvas.data_.size() * sizeof(render::pixelColor),
        std::ostreambuf_iterator(stream));
}

} // namespace raytracing::render
