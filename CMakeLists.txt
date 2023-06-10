cmake_minimum_required(VERSION 3.25)
project(learning_c C)

set(CMAKE_C_STANDARD 11)

add_executable(learning_c main.c)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(learning_c PkgConfig::GTK4 OpenGL::GL GLEW::GLEW)
