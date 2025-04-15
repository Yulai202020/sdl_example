cmake_minimum_required(VERSION 3.14)

project(music_example)

find_package(SDL3 REQUIRED)
find_package(SDL3_mixer REQUIRED)

add_executable(${PROJECT_NAME} music_example.cpp)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    SDL3::SDL3
    SDL3_mixer::SDL3_mixer
)