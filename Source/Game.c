//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Game.h"
#include "../Include/Board.h"
#include "../Include/Commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



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
    scanf("%s", input);

    StartupCommand cmd = parseStartupCommand(input);

    switch (cmd) {
        case LD:
            // fx: loadDeck(board);
                printf("Loading deck...\n");
        break;
        case SW:
            printf("Show deck\n");
        break;
        case SI:
            printf("Split deck\n");
        break;
        case SR:
            printf("Shuffle deck randomly\n");
        break;
        case SD:
            printf("Save deck\n");
        break;
        case QQ:
            printf("Quitting game.\n");
        exit(0);
        case P:
            return PLAY;
        default:
            printf("Invalid command!\n");
        break;
    }
    return currentPhase;
}
GamePhase playPhase(Board *board, GamePhase currentPhase) {
    char input[100];
    scanf("%s", input);

    PlayCommand cmd = parsePlayCommand(input);

    switch (cmd) {
        case Q:
                printf("Loading deck...\n");
        break;
        case MOVES:
            printf("Show deck\n");
        break;
        case U:
            printf("Split deck\n");
        break;
        case R:
            printf("Shuffle deck randomly\n");
        break;
        case S:
            printf("Save deck\n");
        break;
        case L:
            printf("Quitting game.\n");
        case INVALIDP:
            return PLAY;
    }
    return currentPhase;
}


//
// #include <stdio.h>
// #include <string.h>
// #include "../Include/game.h"
// #include "../Include/Commands.h"
//