cmake_minimum_required(VERSION 3.30)
project(projekt_2_yukon_solitaire C)

set(CMAKE_C_STANDARD 23)

# Brug pkg-config til både SDL3 og SDL3_ttf
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)
pkg_check_modules(SDL3_TTF REQUIRED sdl3-ttf)

# Inkluder directories og compile-flags
include_directories(${SDL3_INCLUDE_DIRS} ${SDL3_TTF_INCLUDE_DIRS})
link_directories(${SDL3_LIBRARY_DIRS} ${SDL3_TTF_LIBRARY_DIRS})
add_definitions(${SDL3_CFLAGS_OTHER} ${SDL3_TTF_CFLAGS_OTHER})

add_executable(projekt_2_yukon_solitaire
        Source/Main.c
        Include/Main.h
        Source/Deck.c
        Include/Deck.h
        Source/Game.c
        Include/Game.h
        Source/Card.c
        Include/Card.h
        Source/Foundations.c
        Include/Foundations.h
        Source/FileIO.c
        Include/FileIO.h
        Source/Utils.c
        Include/Utils.h
        Source/Board.c
        Source/LinkedLists.c
        Include/LinkedLists.h
        Source/Columns.c
        Include/Columns.h
        Test/Test.c
        Test/Test.h
        GUI/GUI.c
        GUI/GUI.h
        Server/Server.c
        Server/Server.h
)

# Korrekt link til SDL3 og SDL3_ttf via pkg-config
target_link_libraries(projekt_2_yukon_solitaire ${SDL3_LIBRARIES} ${SDL3_TTF_LIBRARIES})

