cmake_minimum_required(VERSION 3.20)
project(Labyrinth)

find_package(raylib CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(main src/main.cpp)

add_executable(custom_camera examples/custom_camera.cpp)

target_link_libraries(main PRIVATE raylib)
target_link_libraries(custom_camera PRIVATE raylib)

include_directories(includes)

