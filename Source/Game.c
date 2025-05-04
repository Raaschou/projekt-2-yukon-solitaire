//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Game.h"
#include "../Include/Board.h"
#include "../Include/Deck.h"
#include "../Include/FileIO.h"
#include "../Include/Foundations.h"
#include "../Include/Columns.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




void gameLoop(Board *board) {
    GamePhase phase = STARTUP;
    int running = 1;

    char lastCommand[100] = "";
    char message[100] = "";
    // printBoardStartUpPhase(board, lastCommand, message);

    while (running) {

        if (phase == STARTUP) {
            printBoardStartUpPhase(board, lastCommand, message);
            phase = startupPhase(board, phase, lastCommand, message);

        } else if (phase == PLAY) {
            printBoardPlayPhase(board, lastCommand, message);
            phase = playPhase(board, phase, input, lastCommand, message);

        }
    }
}
GamePhase playPhaseTerminal(Board *board, GamePhase currentPhase, char *lastCommand, char *message) {
    char input[100];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Fjern newline

    if (currentPhase == PLAY) {
        return playPhase(board, currentPhase, input, lastCommand, message);
    } else if (currentPhase == STARTUP) {
        return startupPhase(board, currentPhase, input, lastCommand, message);
    }

    strcpy(lastCommand, input);
    strcpy(message, "Ukendt fase");
    return currentPhase;
}

GamePhase startupPhase(Board *board, GamePhase currentPhase,const char *input, char *lastCommand, char *message) {
    char cmd[100] = "";
    char arg[100] = "";
    sscanf(input, "%s%99[^\n]", cmd, arg);

    //TODO måske skulle man lave en tjek hvis der eksistere et deck allerede hvor man bekræfter at man vil overskride

    // LD, Load deck
    if (strcmp(input, "LD") == 0) {
        clearList(&board->deck);
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

        // LD, Load deck
        // SW,Show deck

    } else if (strcmp(input, "SW") == 0) {
        CardNode *current = board->deck.head;
        while (current) {
            current->card.faceUp = 1;
            current = current->next;
        }
        strcpy(lastCommand,"SW");
        strcpy(message,"Kort er nu vist");

        //SI, Split
    } else if (strcmp(input, "SI") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Splitter deck (ikke implementeret endnu)\n");

        //SR, Shuffle random
    } else if (strcmp(input, "SR") == 0) {
        strcpy(lastCommand,"SR");
        strcpy(message,"Shuffle random (ikke implementeret endnu)\n");

        //SD, Save deck
    } else if (strcmp(input, "SD") == 0) {
        strcpy(lastCommand,"SD");
        strcpy(message,"Gemmer deck (ikke implementeret endnu)\n");

        //QQ, Quit program
    } else if (strcmp(input, "QQ") == 0) {
        printf("Forlader spil - Tak for i dag!.\n");
        exit(0);

        //P, Start play phase
    } else if (strcmp(input, "P") == 0) {
        if (board->deck.size == 0) {
            strcpy(message, "Der er ikke loadet et deck!");
            strcpy(lastCommand, input);
            return currentPhase;
        }
        dealToColumns(&board->deck, board->columns);
        clearList(&board->deck);
        strcpy(lastCommand,"P");
        strcpy(message,"Vi spiller!");

        return PLAY;
    } else {
         strcpy(lastCommand,"SI");
         strcpy(message,"Ugyldig kommando i startup-phase.\n");
    }
    return currentPhase;
}

GamePhase playPhase(Board *board, GamePhase currentPhase,const char *input, char *lastCommand, char *message) {
    char input[100];
    scanf("%s", input);
    //Q, Quit play phase
    if (strcmp(input, "Q") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Afslutter spilfase...\n");
        return STARTUP;
        //MOVES, Show legal moves
    } else if (strstr(input, "->") != NULL) {
        char from[100], to[100];
        sscanf(input, "%[^-]->%s", from, to);

        if (from[0] == 'C' && to[0] == 'F') {
            int colIndex = from[1] - '1';     // fx 'C2' -> index 1
            int fIndex = to[1] - '1';         // fx 'F1' -> index 0

            if (colIndex < 0 || colIndex >= 7 || fIndex < 0 || fIndex >= 4) {
                strcpy(message, "Ugyldigt kolonne!");
                return currentPhase;
            }

            LinkedList *source = &board->columns[colIndex];
            LinkedList *target = &board->foundations[fIndex];
            CardNode *card = source->tail;

            if (!card) {
                strcpy(message, "Kolonnen er tom!");
            } else if (validMoveF(card, target->tail)) {
                moveToFoundation(card, source, target);
                flipLastCardIfAny(source);
                strcpy(message, "OK");
            } else {
                strcpy(message, "Ugyldigt træk!");
            }
            return currentPhase;
        } else if (from[0] == 'C' && strchr(from, ':')) {
            int fromCol = from[1] - '1';
            int toCol = to[1] - '1';

            if (fromCol < 0 || fromCol >= 7 || toCol < 0 || toCol >= 7) {
                strcpy(message, "Ugyldig kolonne!");
                return currentPhase;
            }

            char *colon = strchr(from, ':');
            char *cardStr = colon + 1;

            int rank = 0;
            char suit;

            // Forsøg først numerisk parsing
            if (sscanf(cardStr, "%d%c", &rank, &suit) != 2) {
                if (cardStr[0] == 'A') rank = 1;
                else if (cardStr[0] == 'T') rank = 10;
                else if (cardStr[0] == 'J') rank = 11;
                else if (cardStr[0] == 'Q') rank = 12;
                else if (cardStr[0] == 'K') rank = 13;
                else {
                    strcpy(message, "Ugyldigt kort!");
                    return currentPhase;
                }
                suit = cardStr[1];
            }

            LinkedList *source = &board->columns[fromCol];
            LinkedList *dest = &board->columns[toCol];

            CardNode *node = findCardNode(source, rank, suit);

            if (!node || !nodeInList(node, source)) {
                strcpy(message, "Kortet er ikke i kolonnen!");
            } else if (!node->card.faceUp) {
                strcpy(message, "Kan ikke flytte [] kortene");
            } else if (validMoveC(node, dest->tail)) {
                moveBetweenColumns(node, source, dest);
                flipLastCardIfAny(source);
                strcpy(message, "OK");
            } else {
                strcpy(message, "Ugyldigt træk!");
            }

            return currentPhase;
        } else if (from[0] == 'C' && to[0] == 'C') {
            int fromCol = from[1] - '1';
            int toCol = to[1] - '1';

            if (fromCol < 0 || fromCol >= 7 || toCol < 0 || toCol >= 7) {
                strcpy(message, "Ugyldig kolonne!");
                return currentPhase;
            }

            LinkedList *source = &board->columns[fromCol];
            LinkedList *dest = &board->columns[toCol];

            CardNode *node = source->tail;

            if (!node) {
                strcpy(message, "Kolonnen er tom!");
            } else if (!node->card.faceUp) {
                strcpy(message, "Kan ikke flytte [] kortene");
            } else if (validMoveC(node, dest->tail)) {
                moveBetweenColumns(node, source, dest);
                flipLastCardIfAny(source);
                strcpy(message, "OK");
            } else {
                strcpy(message, "Ugyldigt træk!");
            }

            return currentPhase;
        } else if (from[0] == 'F' && to[0] == 'C') {
            int fromIndex = from[1] - '1';
            int toIndex = to[1] - '1';

            if (fromIndex < 0 || fromIndex >= 4 || toIndex < 0 || toIndex >= 7) {
                strcpy(message, "Ugyldig foundation!");
                return currentPhase;
            }

            LinkedList *source = &board->foundations[fromIndex];
            LinkedList *dest = &board->columns[toIndex];

            CardNode *node = source->tail;

            if (!node) {
                strcpy(message, "Foundation er tom!");
            } else if (validMoveC(node, dest->tail)) {
                moveBetweenColumns(node, source, dest);
                strcpy(message, "OK");
            } else {
                strcpy(message, "Ugyldigt træk!");
            }

            return currentPhase;
        }
        strcpy(lastCommand, input);
        printf("Du prøvede at flytte fra '%s' til '%s'\n", from, to);
        strcpy(message, "Trækmodtagelse testet.");
        //U, Undo
    } else if (strcmp(input, "U") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Fortryder sidste træk...\n");
        //R, Redo
    } else if (strcmp(input, "R") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Gør træk om...\n");
        //S, Save game
    } else if (strcmp(input, "S") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Gemmer spil...\n");
        //
        //L, Load game
        //
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



// Kommandoer i spilfasen – 'Q' går tilbage til startfasen


*/