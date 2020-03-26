cmake_minimum_required(VERSION 3.5)

project(PanoramixQtWidget)
set(CMAKE_CXX_STANDARD 17)
message("CMake build type: " ${CMAKE_BUILD_TYPE})

include(.conan/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(3rd-party)
add_subdirectory(modules/panoramix)
add_subdirectory(modules/image-processors)
add_subdirectory(src)