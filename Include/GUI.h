#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "Board.h"
#include "Card.h"
#include "Game.h"

#define NUM_STARTUP_BUTTONS 7
#define NUM_PLAY_BUTTONS 5

typedef struct {
    SDL_Rect rect;
    const char *label;
} Button;

// GUI main loop
void gameLoopGUI(Board *board);

// Tegnefunktioner
void drawButton(SDL_Renderer *renderer, Button *button, TTF_Font *font);
void drawMessage(SDL_Renderer *renderer, const char *message, TTF_Font *font);
void drawCard(SDL_Renderer *renderer, int x, int y, Card *card);
void drawBoard(SDL_Renderer *renderer, Board *board, GamePhase phase, const char *message, TTF_Font *font);

void drawEmptyBoard(SDL_Renderer *renderer);

#endif
