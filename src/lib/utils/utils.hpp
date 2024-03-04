#pragma once

#include <math/base.hpp>

#include <istream>

namespace raytracing {

std::istream& operator>>(std::istream& sin, math::vec2& vec);
std::istream& operator>>(std::istream& sin, math::vec3& vec);
std::istream& operator>>(std::istream& sin, math::vec4& vec);

} // namespace raytracing::util
