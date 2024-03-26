cmake_minimum_required(VERSION 3.16)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else()
    message(FATAL_ERROR "OpenMP not found! You need OpenMP for speedup on multicore CPUs!")
endif()

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
    src/lib/math/distributions.hpp
    src/lib/math/distributions.cpp
    src/lib/math/global_random.hpp
    src/lib/math/global_random.cpp
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
  # target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX) # OpenMP and VS17 >:{
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)


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