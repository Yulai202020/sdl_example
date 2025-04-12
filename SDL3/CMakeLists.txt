cmake_minimum_required(VERSION 3.14)

project(SDL3)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(examples)

file(GLOB SOURCE_FILES
    src/*.cpp
)

# Define the executable and link SDL2
add_executable(game ${SOURCE_FILES})

target_link_libraries(game
    PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)