cmake_minimum_required(VERSION 3.16)
project(bebis-run LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(external/raylib)

set(SOURCES
    src/bebis.c
    src/player.c
    src/main.c
)

add_executable(bebis-run ${SOURCES})

target_include_directories(bebis-run
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/external/raylib/src
)

target_link_libraries(bebis-run
    PRIVATE
        raylib
)
