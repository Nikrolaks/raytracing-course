#include "abstracts/vframe.hpp"

#include <iostream>

int main(int, char* argv[]) {
    auto vframe = raytracing::abs::VFrame::fromGLTF(argv[1]);
    raytracing::render::Canvas::toPPM(argv[2], vframe.render());

    return 0;
}
