cmake_minimum_required(VERSION 3.14)

project(sdl_example CXX)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
)

# add_executable(${PROJECT_NAME}_static main.c)
# target_link_libraries(${PROJECT_NAME}_static PUBLIC SDL2::SDL2)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

