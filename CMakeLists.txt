cmake_minimum_required(VERSION 3.16)
project(bebis-run LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS lib/*.c src/*.c)

add_executable(bebis-run ${SOURCES})

target_compile_options(bebis-run PRIVATE
        -Wall -Wextra -pedantic -Wconversion -Wnull-dereference
        -pipe -fno-common -fstack-protector-strong -O3
)
target_link_libraries(bebis-run PRIVATE
        /usr/local/lib/libraylib.a
        m dl pthread GL
)
target_include_directories(bebis-run PRIVATE inc lib /usr/local/include)
