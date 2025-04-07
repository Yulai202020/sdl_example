cmake_minimum_required(VERSION 3.14)

project(sdl_example C)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
)
