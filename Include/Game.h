//
// Created by August Frandsen on 15/04/2025.
//

#ifndef GAME_H
#define GAME_H
#include "../Include/Board.h"
#include "../Include/Deck.h"
#include "../Include/FileIO.h"
#include "../Include/Columns.h"


//tilføjet ENUM på faser herinde i stedet for commands
typedef enum {
    STARTUP,
    PLAY
} GamePhase;

void gameLoop(Board *board);
GamePhase startupPhase(Board *board, GamePhase currentPhase, char *lastCommand, char *message);
GamePhase playPhase(Board *board, GamePhase currentPhase, char *lastCommand, char *message);

#endif