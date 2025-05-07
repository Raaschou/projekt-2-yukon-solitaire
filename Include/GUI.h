#ifndef GUI_H
#define GUI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "Board.h"
#include "Card.h"
#include "Game.h"

#define NUM_STARTUP_BUTTONS 7
#define NUM_PLAY_BUTTONS 4

typedef struct {
    SDL_Rect rect;
    const char *label;
} Button;

typedef struct {
    SDL_Rect rect;
    CardNode *node;
} CardVisual;

extern char commandArg[64];
extern int typing;


void gameLoopGUI(Board *board);


void drawButton(SDL_Renderer *renderer, Button *button, TTF_Font *font);
void drawMessage(SDL_Renderer *renderer, const char *message, TTF_Font *font, int windowHeight, int buttonHeight);
void drawCard(SDL_Renderer *renderer, int x, int y, Card *card);
void drawBoardPlayPhase(SDL_Renderer *renderer, SDL_Window *window, Board *board, TTF_Font *font, const char *message);
void drawBoardStartUpPhase(SDL_Renderer *renderer, SDL_Window *window, Board *board, TTF_Font *font, const char *message);

#endif
