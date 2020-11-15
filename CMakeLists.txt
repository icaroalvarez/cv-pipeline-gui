cmake_minimum_required(VERSION 3.5)

project(CvPipelineGui)
set(CMAKE_CXX_STANDARD 17)
message("CMake build type: " ${CMAKE_BUILD_TYPE})

include(.conan/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(modules/cv-pipeline)
add_subdirectory(src)
add_subdirectory(test)
