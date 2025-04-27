//
// Created by August Frandsen on 15/04/2025.
//

#ifndef GAME_H
#define GAME_H

#include "Board.h"

typedef enum {
    STARTUP,
    PLAY
} GamePhase;

void gameLoop(Board *board);
GamePhase startupPhase(Board *board, GamePhase currentPhase);
GamePhase playPhase(Board *board, GamePhase currentPhase);

#endif