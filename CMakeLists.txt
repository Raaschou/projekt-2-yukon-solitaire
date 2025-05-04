cmake_minimum_required(VERSION 3.30)
project(projekt_2_yukon_solitaire C)

set(CMAKE_C_STANDARD 23)

# Find SDL3 via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)

# Tilføj SDL3 include/lib fra pkg-config
if(APPLE)
    include_directories(${SDL3_INCLUDE_DIRS})
    link_directories(${SDL3_LIBRARY_DIRS})
    add_definitions(${SDL3_CFLAGS_OTHER})
endif()

# Tilføj SDL3_ttf manuelt (uden pkg-config)
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

# Projektets kildefiler
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
)

# Inkluder SDL3 og SDL3_ttf
target_include_directories(projekt_2_yukon_solitaire PRIVATE ${SDL3_INCLUDE_DIRS} /opt/homebrew/include)

# Link SDL3 og SDL3_ttf
target_link_libraries(projekt_2_yukon_solitaire ${SDL3_LIBRARIES} SDL3_ttf)