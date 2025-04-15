cmake_minimum_required(VERSION 3.14)

project(3d_example)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} 3d_example.c)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    SDL3::SDL3
    OpenGL::GL
)