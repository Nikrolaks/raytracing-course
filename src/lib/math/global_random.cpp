#include "global_random.hpp"

namespace raytracing::math {

GlobalRandomHolder GlobalRandomHolder::instance_;
thread_local std::minstd_rand GlobalRandomHolder::engine_;

} // namespace raytracing::math
