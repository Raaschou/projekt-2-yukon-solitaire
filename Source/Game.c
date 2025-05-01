//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




void gameLoop(Board *board) {
    GamePhase phase = STARTUP;
    int running = 1;

    char lastCommand[100] = "";
    char message[100] = "";
    printBoardStartUpPhase(board, lastCommand, message);

    while (running) {


        if (phase == STARTUP) {

            phase = startupPhase(board, phase, lastCommand, message);
            printBoardStartUpPhase(board, lastCommand, message);
        } else if (phase == PLAY) {
            phase = playPhase(board, phase, lastCommand, message);
            printBoardPlayPhase(board, lastCommand, message);
        }
    }
}
GamePhase startupPhase(Board *board, GamePhase currentPhase, char *lastCommand, char *message) {
    char input[100];
    char arg[100] = "";
    char line[200];
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%s%99[^\n]", input, arg);

    if (strcmp(input, "LD") == 0) {
        // Gem sidste kommando
        strcpy(lastCommand, "LD");
        // Fjern evt. førende mellemrum fra arg
        char *filename = arg;
        while (*filename == ' ') filename++;

        int success;
        if (strlen(filename) > 0) {
            success = readDeckFromFile(filename, &board->deck, message);
        } else {
            // Lav standarddeck
            startDeck(&board->deck);
            strcpy(message, "OK");
            success = 1;
        }


        if (success) {
           return STARTUP;
        }


    } else if (strcmp(input, "SW") == 0) {
        CardNode *current = board->deck.head;
        while (current) {
            current->card.faceUp = 1;
            current = current->next;
        }
        strcpy(lastCommand,"SW");
        strcpy(message,"Kort er nu vist");

    } else if (strcmp(input, "SI") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Splitter deck (ikke implementeret endnu)\n");

    } else if (strcmp(input, "SR") == 0) {
        strcpy(lastCommand,"SR");
        strcpy(message,"Shuffle random (ikke implementeret endnu)\n");

    } else if (strcmp(input, "SD") == 0) {
        strcpy(lastCommand,"SD");
        strcpy(message,"Gemmer deck (ikke implementeret endnu)\n");

    } else if (strcmp(input, "QQ") == 0) {
        printf("Forlader spil - Tak for i dag!.\n");
        exit(0);
    } else if (strcmp(input, "P") == 0) {
    //     clearList(&board->deck);
    //     for (int i = 0; i < 7; i++) clearList(&board->columns[i]);
    //     for (int i = 0; i < 4; i++) clearList(&board->foundations[i]);
    //     startDeck(&board->deck);
    //     strcpy(message, "Indlæser et deck");
    //     strcpy(lastCommand, input);
    //     dealToColumns(&board->deck, board->columns);
    //     printf("%s %s\n", message, lastCommand);


        return PLAY;
    } else {
         strcpy(lastCommand,"SI");
         strcpy(message,"Ugyldig kommando i startup-phase.\n");
    }
    return currentPhase;
}

GamePhase playPhase(Board *board, GamePhase currentPhase, char *lastCommand, char *message) {
    char input[100];
    scanf("%s", input);

    if (strcmp(input, "Q") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Afslutter spilfase...\n");
        return STARTUP;
    } else if (strcmp(input, "MOVES") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Viser mulige træk...\n");

    } else if (strcmp(input, "U") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Fortryder sidste træk...\n");

    } else if (strcmp(input, "R") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Gør træk om...\n");

    } else if (strcmp(input, "S") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Gemmer spil...\n");

    } else if (strcmp(input, "L") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Indlæser spil...\n");

    } else {
        strcpy(lastCommand,"Was not valid");
        strcpy(message,"Ugyldig kommando i spilfase.\n");
    }

    return currentPhase;
}

//
// #include <stdio.h>
// #include <string.h>
// #include "../Include/game.h"
// #include "../Include/Commands.h"
//

/*
    LD,  // Load deck
    SW,  // Show deck
    SI,  // Split
    SR,  // Shuffle random
    SD,  // Save deck
    QQ,  // Quit program
    P    // Start play phase


// Kommandoer i spilfasen – 'Q' går tilbage til startfasen

    Q,      // Quit play phase
    MOVES,  // Show legal moves
    U,      // Undo
    R,      // Redo
    S,      // Save game
    L       // Load game
*/