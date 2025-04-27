//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Game.h"
#include "../Include/Board.h"
#include "../Include/Commands.h"
#include <stdio.h>
#include <string.h>


void gameLoop(Board *board) {
    GamePhase phase = STARTUP;
    int running = 1;

    while (running) {
        printBoard(board, "last command", "status message");

        if (phase == STARTUP) {
            phase = startupPhase(board, phase);
        } else if (phase == PLAY) {
            phase = playPhase(board, phase);
        }
    }
}

// forskellige faser
GamePhase startupPhase(Board *board, GamePhase currentPhase) {
    char input[100];
    printf("Enter startup command: ");
    scanf("%s", input);

    if (strcmp(input, "LD") == 0) {
    } else if (strcmp(input, "P") == 0) {
        return PLAY;
    } else {
        printf("Invalid command!\n");
    }
    return currentPhase;
}

GamePhase playPhase(Board *board, GamePhase currentPhase) {
    char input[100];
    printf("Enter play command: ");
    scanf("%s", input);

    if (strcmp(input, "MOVES") == 0) {

    } else if (strcmp(input, "Q") == 0) {
        return STARTUP;
    } else {
        printf("Invalid command!\n");
    }
    return currentPhase;
}
