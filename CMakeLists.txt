cmake_minimum_required(VERSION 3.20)
project(chip8emu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
add_executable(chip8 src/main.cpp src/display.cpp src/memory.cpp src/cpu.cpp)
  
  # Link against the canonical SDL3 target
target_link_libraries(chip8 PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)
