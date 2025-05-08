cmake_minimum_required(VERSION 3.16)
project(projekt_2_yukon_solitaire C)

set(CMAKE_C_STANDARD 23)

# ---- Platform detection ----
if(APPLE)
    message(STATUS "Building on macOS")

    include_directories(/opt/homebrew/include/SDL2)
    link_directories(/opt/homebrew/lib)
    set(SDL2_LIBS SDL2 SDL2_ttf)

elseif(WIN32)
    message(STATUS "Building on Windows")
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
    set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
    set(SDL2_TTF_PATH "SDL2/x86_64-w64-mingw32")

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TFF_INCLUDE_DIRS})

    set(SDL2_LIBS ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARIES})


else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# ---- Kildesystem ----
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
        Source/Board.c
        Source/LinkedLists.c
        Include/LinkedLists.h
        Source/Columns.c
        Include/Columns.h
        Source/GUI.c
        Include/GUI.h
)
target_link_libraries(projekt_2_yukon_solitaire ${SDL2_LIBS})