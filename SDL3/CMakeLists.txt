cmake_minimum_required(VERSION 3.14)

project(SDL3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(game)
add_subdirectory(snake_game)
add_subdirectory(examples)