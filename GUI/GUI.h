//
// Created by Christoffer Forsberg on 04/05/2025.
//

// In GUI.h
// =========================================
#ifndef GUI_H
#define GUI_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../Include/Game.h"

// === Globale konstanter ===
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define CARD_WIDTH 60
#define CARD_HEIGHT 90
#define COLUMN_START_X 50
#define COLUMN_START_Y 50
#define COLUMN_SPACING 80
#define CARD_VERTICAL_OFFSET 25
#define FOUNDATION_X 800
#define FOUNDATION_Y 50

// Declare externals for other files to access
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *font;
extern SDL_Texture *cardTextures[14][4];
extern SDL_Texture *cardBackTexture;

// Function declarations
void initGUI(void);
void shutdownGUI(void);
void loadCardPictures(SDL_Renderer *renderer);
void drawBoard(SDL_Renderer *renderer, Board *board, GamePhase phase, const char *lastCommand, const char *message);
// Add other function declarations...

#endif // GUI_H
