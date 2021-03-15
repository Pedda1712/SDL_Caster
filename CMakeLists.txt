cmake_minimum_required(VERSION 3.16)
project(SDL_CASTER)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(SOURCES 
    ${CMAKE_SOURCE_DIR} map.cpp
    ${CMAKE_SOURCE_DIR} main.cpp
    ${CMAKE_SOURCE_DIR} player.cpp
    ${CMAKE_SOURCE_DIR} caster_math.cpp
    ${CMAKE_SOURCE_DIR} raycaster.cpp
)

add_executable(caster ${SOURCES})
target_compile_options(caster PUBLIC -O3 -Wall -Wextra)
target_link_libraries(caster ${SDL2_LIBRARIES} -lSDL2_ttf -lm)
add_definitions(-DSDL_MAIN_HANDLED)
