cmake_minimum_required(VERSION 3.20)
project(Labyrinth)

find_package(raylib CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(main src/main.cpp src/maze.cpp src/minotaur.cpp)

add_executable(custom_camera examples/custom_camera.cpp)
add_executable(sounds_ex examples/sound_ex.cpp src/sound.cpp)

target_link_libraries(main PRIVATE raylib)
target_link_libraries(custom_camera PRIVATE raylib)
target_link_libraries(sounds_ex PRIVATE raylib)

include_directories(includes)

