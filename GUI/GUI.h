//
// Created by Christoffer Forsberg on 04/05/2025.
//

#ifndef GUI_H
#define GUI_H

#include <SDL3/SDL.h>
#include "../Include/Board.h"
#include "../Include/Game.h"
void initGUI();
void shutdownGUI();
void loadCardPictures(SDL_Renderer *renderer);
void drawBoard(SDL_Renderer *renderer, Board *board, GamePhase phase, const char *lastCommand, const char *message);

#endif
