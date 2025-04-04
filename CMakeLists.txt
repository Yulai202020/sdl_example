cmake_minimum_required(VERSION 3.14)

project(sdl_example C)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2)