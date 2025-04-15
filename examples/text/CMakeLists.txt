cmake_minimum_required(VERSION 3.14)

project(text_example)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_executable(${PROJECT_NAME} text_example.cpp)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)
