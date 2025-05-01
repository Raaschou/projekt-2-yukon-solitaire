//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Game.h"
#include "../Include/Board.h"
#include "../Include/Deck.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../Include/Columns.h"


void gameLoop(Board *board) {
    GamePhase phase = STARTUP;
    int running = 1;

    char lastCommand[100] = "";
    char message[100] = "";

    while (running) {
        printBoard(board, lastCommand, message);

        if (phase == STARTUP) {
            phase = startupPhase(board, phase, lastCommand, message);
        } else if (phase == PLAY) {
            phase = playPhase(board, phase, lastCommand, message);
        }
    }
}
GamePhase startupPhase(Board *board, GamePhase currentPhase, char *lastCommand, char *message) {

    char input[100];
    scanf("%s", input);

    if (strcmp(input, "LD") == 0) {
        clearList(&board->deck);
        for (int i = 0; i < 7; i++) clearList(&board->columns[i]);
        for (int i = 0; i < 4; i++) clearList(&board->foundations[i]);
        startDeck(&board->deck);
        strcpy(message, "Indlæser et deck");
        strcpy(lastCommand, input);
        dealToColumns(&board->deck, board->columns);
        printf("%s %s\n", message, lastCommand);
    } else if (strcmp(input, "SW") == 0) {
        if (board->deck.head == NULL) {
            strcpy(message, "Fejl: Der er ikke indlæst et deck endnu.");
        } else {
            CardNode *current = board->deck.head;
            while (current != NULL) {
                Card card = current->card;
                printf("%d%c ", card.rank, card.suit);
                current = current->next;
            }
            printf("\n");
            strcpy(message, "OK");
        }
        strcpy(lastCommand, input);

    } else if (strcmp(input, "SI") == 0) {
        printf("Splitter deck (ikke implementeret endnu)\n");

    } else if (strcmp(input, "SR") == 0) {
        printf("Shuffle random (ikke implementeret endnu)\n");

    } else if (strcmp(input, "SD") == 0) {
        printf("Gemmer deck (ikke implementeret endnu)\n");

    } else if (strcmp(input, "QQ") == 0) {
        printf("Forlader spil - Tak for i dag!.\n");
        exit(0);
    } else if (strcmp(input, "P") == 0) {
        // dealToColumns(&board->deck, board->columns);
        // // vend øverste kort i hver kolonne
        // for (int i = 0; i < 7; i++) {
        //     if (board->columns[i].tail != NULL) {
        //         board->columns[i].tail->card.faceUp = 1;
        //     }
        // }
        // printf("Starter spil...\n");
        return PLAY;
    } else {
        printf("Ugyldig kommando i startup-phase.\n");
    }
    return currentPhase;
}

GamePhase playPhase(Board *board, GamePhase currentPhase, char *lastCommand, char *message) {
    char input[100];
    scanf("%s", input);

    if (strcmp(input, "Q") == 0) {
        printf("Afslutter spilfase...\n");
        return STARTUP;
    } else if (strcmp(input, "MOVES") == 0) {
        printf("Viser mulige træk...\n");

    } else if (strcmp(input, "U") == 0) {
        printf("Fortryder sidste træk...\n");

    } else if (strcmp(input, "R") == 0) {
        printf("Gør træk om...\n");

    } else if (strcmp(input, "S") == 0) {
        printf("Gemmer spil...\n");

    } else if (strcmp(input, "L") == 0) {
        printf("Indlæser spil...\n");

    } else {
        printf("Ugyldig kommando i spilfase.\n");
    }

    return currentPhase;
}

//
// #include <stdio.h>
// #include <string.h>
// #include "../Include/game.h"
// #include "../Include/Commands.h"
//