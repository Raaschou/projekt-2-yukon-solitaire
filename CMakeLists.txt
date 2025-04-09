cmake_minimum_required(VERSION 3.30)
project(projekt_2_yukon_solitaire C)

set(CMAKE_C_STANDARD 23)

# Find SDL3 via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)

# Inkluder og link SDL3
include_directories(${SDL3_INCLUDE_DIRS})
link_directories(${SDL3_LIBRARY_DIRS})
add_definitions(${SDL3_CFLAGS_OTHER})

# Dit projekt + sourcefiler
add_executable(projekt_2_yukon_solitaire
        main.c
        LOL/Kort.c
        LOL/Regler.c
        main.h
)

# Link SDL3 med dit projekt
target_link_libraries(projekt_2_yukon_solitaire ${SDL3_LIBRARIES})