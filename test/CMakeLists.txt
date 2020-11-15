cmake_minimum_required(VERSION 3.5)

set(test_source_files
        main.test.cpp
        ParametersWidget.test.cpp
        )

add_executable(test-gui
        ${test_source_files}
        )

target_link_libraries(test-gui PRIVATE
        cv-pipeline
        CONAN_PKG::trompeloeil
        CONAN_PKG::Catch2
        )

target_include_directories(test-gui PRIVATE
        ../src)

target_compile_definitions(test-gui
        PRIVATE fixtures_path="${CMAKE_CURRENT_SOURCE_DIR}/fixtures/"
        )

option(COVERAGE_ENABLED "Calculate test coverage" OFF)

if(COVERAGE_ENABLED MATCHES ON)
    message("Testing coverage enabled")
    include(CMakeModules/CodeCoverage.cmake)
    append_coverage_compiler_flags()
    target_compile_options(gui PRIVATE -g -fprofile-arcs -ftest-coverage)
    setup_target_for_coverage_lcov(
            NAME test-gui-coverage
            EXECUTABLE test-gui
            DEPENDENCIES cv-pipeline
            BASE_DIRECTORY "${PROJECT_SOURCE_DIR}/src"
            EXCLUDE "${PROJECT_SOURCE_DIR}/test/*" "/usr/*" "$ENV{HOME}/.conan/*")
endif()