#include "utils.hpp"

namespace raytracing {

std::istream& operator>>(std::istream& sin, math::vec2& vec) {
    long double x, y;
    sin >> x >> y;
    vec = { x, y };

    return sin;
}

std::istream& operator>>(std::istream& sin, math::vec3& vec) {
    long double x, y, z;
    sin >> x >> y >> z;
    vec = { x, y, z };

    return sin;
}

std::istream& operator>>(std::istream& sin, math::vec4& vec) {
    long double x, y, z, w;
    sin >> x >> y >> z >> w;
    vec = { x, y, z, w };

    return sin;
}

} // namespace raytracing::util
