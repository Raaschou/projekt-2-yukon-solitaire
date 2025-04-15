cmake_minimum_required(VERSION 3.30)
project(projekt_2_yukon_solitaire C)

set(CMAKE_C_STANDARD 23)

# Find SDL3 via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)

# Inkluder og link SDL3
if(APPLE)
    include_directories(${SDL3_INCLUDE_DIRS})
    link_directories(${SDL3_LIBRARY_DIRS})
    add_definitions(${SDL3_CFLAGS_OTHER})
endif()
# Dit projekt + sourcefiler
add_executable(projekt_2_yukon_solitaire
        main.c
        Board.c
        Include/main.h
        Include/Board.h
        Source/Deck.c
        Include/Deck.h
        Source/Game.c
        Include/Game.h
        Source/Card.c
        Include/Card.h
        Source/Foundations.c
        Include/Foundations.h
        Source/Commands.c
        Include/Commands.h
        Source/FileIO.c
        Include/FileIO.h
        Source/Shuffle.c
        Include/Shuffle.h
        Source/Utils.c
        Include/Utils.h
        Source/NewMain.c
        Include/NewMain.h
        Source/NewBoard.c
        Include/NewBoard.h
        Source/LinkedLists.c
        Include/LinkedLists.h
        Source/Columns.c
        Include/Columns.h
)

# Link SDL3 med dit projekt
target_link_libraries(projekt_2_yukon_solitaire ${SDL3_LIBRARIES})