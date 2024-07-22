cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/Users/jessi/OneDrive/Escritorio/New folder/sdl/SDL2-devel-2.30.5-mingw/SDL2-2.30.5/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(untitled main.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
