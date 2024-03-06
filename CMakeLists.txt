cmake_minimum_required(VERSION 3.16)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/lib/abstracts/camera.hpp
    src/lib/abstracts/camera.cpp
    src/lib/abstracts/scene.hpp
    src/lib/abstracts/scene.cpp
    src/lib/abstracts/vframe.hpp
    src/lib/abstracts/vframe.cpp

    src/lib/math/base.hpp
    src/lib/math/base.cpp
    src/lib/math/coord_system.hpp
    src/lib/math/ray.hpp
    src/lib/math/ray.cpp

    src/lib/render/objects/factory.hpp
    src/lib/render/objects/factory.cpp
    src/lib/render/objects/plane.hpp
    src/lib/render/objects/plane.cpp
    src/lib/render/objects/ellipsoid.hpp
    src/lib/render/objects/ellipsoid.cpp
    src/lib/render/objects/box.hpp
    src/lib/render/objects/box.cpp
    
    src/lib/render/base.hpp
    src/lib/render/base.cpp
    src/lib/render/canvas.cpp
    src/lib/render/canvas.hpp
    src/lib/render/elementary.cpp
    src/lib/render/elementary.hpp
    src/lib/render/intersection.hpp

    src/lib/utils/readers.cpp
    src/lib/utils/utils.hpp
)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/lib)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


add_executable(raytracer src/bin/main.cpp)
target_link_libraries(raytracer ${PROJECT_NAME})
if(MSVC)
  target_compile_options(raytracer PRIVATE /W4 /WX)
else()
  target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

foreach(source IN LISTS SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()