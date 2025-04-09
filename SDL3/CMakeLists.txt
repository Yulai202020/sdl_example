cmake_minimum_required(VERSION 3.14)

project(sdl_example)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(3d_example 3d_example.c)
add_executable(text_example text_example.cpp)

target_link_libraries(3d_example
    PUBLIC
    SDL3::SDL3
    OpenGL::GL
)

target_link_libraries(text_example
    PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)


set(SOURCE_FILES
    src/main.cpp
    src/game.cpp
    src/player.cpp
    src/tools.cpp
    src/animation.cpp
)

# Define the executable and link SDL2
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
)