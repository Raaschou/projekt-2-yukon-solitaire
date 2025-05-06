cmake_minimum_required(VERSION 3.16)
project(projekt_2_yukon_solitaire C)

set(CMAKE_C_STANDARD 23)

# ---- Platform detection ----
if(APPLE)
    message(STATUS "Building on macOS")

    # Brug SDL2 installeret via Homebrew
    include_directories(/opt/homebrew/include/SDL2)
    link_directories(/opt/homebrew/lib)

    set(SDL2_LIBS SDL2 SDL2_ttf)

elseif(WIN32)
    message(STATUS "Building on Windows")

    find_package(SDL2 REQUIRED)

    include_directories(${SDL2_INCLUDE_DIRS} )
    link_directories(${SDL2_LIBRARY_DIRS})

    set(SDL2_LIBS ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARY})

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
        Source/Utils.c
        Include/Utils.h
        Source/Board.c
        Source/LinkedLists.c
        Include/LinkedLists.h
        Source/Columns.c
        Include/Columns.h
        Source/GUI.c
        Include/GUI.h
        Test/Test.c
        Test/Test.h
)

# ---- Link SDL2 og TTF ----
target_link_libraries(projekt_2_yukon_solitaire ${SDL2_LIBS})