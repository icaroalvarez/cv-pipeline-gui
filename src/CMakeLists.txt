cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set Policy CMP0071 to new for letting AUTOMOC and AUTOUIC process GENERATE files.
if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

message(${CMAKE_MODULE_PATH})
find_package(Qt5Widgets)

if (Qt5Widgets_FOUND)
    message(STATUS "Qt Version Found:" ${Qt5Widgets_VERSION})
endif()

set(source_files
        main.cpp
        mainwindow
        qlabelmat
        qwidgethandler
        ProcessorWidget
        FrameSourceNavigationWidget
        Controller
        )

set(FORMS
        mainwindow.ui
        )

qt5_wrap_ui(FORM_H ${FORMS})

add_executable(gui
        ${source_files}
        ${FORM_H}
        )

target_link_libraries(gui PRIVATE
        panoramix
        image_processors
        Qt5::Widgets
        CONAN_PKG::opencv
        CONAN_PKG::easyloggingpp
        )

target_include_directories(gui PRIVATE
        ../src
        )