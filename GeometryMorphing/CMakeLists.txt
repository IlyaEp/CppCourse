cmake_minimum_required(VERSION 3.21)
project(morph)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

add_executable(morph geometry_cube.cpp
                     geometry_cube.hpp
                     morph_window.cpp
                     morph_window.hpp
        shaders/shaders.qrc
                     main_widget.cpp
                     main_widget.hpp
                     main.cpp)

target_link_libraries(morph PRIVATE Qt5::Core
                                    Qt5::Gui
                                    Qt5::Widgets
                                    Qt5::OpenGL)
