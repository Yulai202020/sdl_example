cmake_minimum_required(VERSION 3.14)

project(snake)

find_package(SDL3 REQUIRED)

add_executable(${PROJECT_NAME} snake.cpp)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    SDL3::SDL3
)