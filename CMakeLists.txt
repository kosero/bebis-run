cmake_minimum_required(VERSION 3.16)
project(bebis-run LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c)

add_executable(bebis-run ${SOURCES})

target_compile_options(bebis-run PRIVATE
    -Wall -Wextra -pedantic -Wconversion -Wnull-dereference
    -pipe -fno-common -fstack-protector-strong -O3
)

target_include_directories(bebis-run PRIVATE
    inc
)

if (WIN32)
    target_link_libraries(bebis-run PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/libraylib.a
        winmm
        gdi32
        opengl32
    )

    target_compile_definitions(bebis-run PRIVATE
        PLATFORM_DESKTOP
        _WIN32_WINNT=0x0600
    )

    target_link_options(bebis-run PRIVATE
        -static-libgcc
    )
else()
    target_link_libraries(bebis-run PRIVATE
        raylib
        m dl pthread GL
    )
endif()

