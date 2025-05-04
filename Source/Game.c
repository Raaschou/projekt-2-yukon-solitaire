//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Game.h"

#include <ctype.h>

#include "../Include/Board.h"
#include "../Include/Deck.h"
#include "../Include/FileIO.h"
#include "../Include/Foundations.h"
#include "../Include/Columns.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "../GUI/GUI.h"

// vi skal

SDL_Renderer *renderer;
void gameLoop(Board *board, int useGUI) {
    GamePhase phase = STARTUP;
    int running = 1;

    char lastCommand[100] = "";
    char message[100] = "";

    SDL_Event event;
    char guiInput[100] = ""; // Kommando via GUI

    while (running) {
        if (useGUI) {
            drawBoard(renderer, board, phase, lastCommand, message);
        }


        if (useGUI) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = 0;
                }

                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    // TODO: lav klik → kommando
                    // Midlertidigt test:
                    strcpy(guiInput, "LD");  // fx load standarddeck
                }

            }

            if (strlen(guiInput) > 0) {
                if (phase == STARTUP) {
                    phase = startupPhase(board, phase, guiInput, lastCommand, message);
                } else if (phase == PLAY) {
                    phase = playPhase(board, phase, guiInput, lastCommand, message);
                }
                guiInput[0] = 0; // nulstil
            }

            SDL_Delay(16); // ca. 60 FPS
        }

        // Terminal-mode
        else {
            if (phase == STARTUP) {
                phase = playPhaseTerminal(board, phase, lastCommand, message);
                printBoardStartUpPhase(board, lastCommand, message);
            } else if (phase == PLAY) {
                phase = playPhaseTerminal(board, phase, lastCommand, message);
                printBoardPlayPhase(board, lastCommand, message);
            }
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
    if (strcasecmp(input, "LD") == 0) {
        if (board->deck.size != 0) {
            strcpy(lastCommand,"LD men der er allerede et deck");
            strcpy(message,"vil du overskride 1-ja, 2-nej");
            printBoardStartUpPhase(board, lastCommand, message);

        }else {
            goto nytDeck;
        }

        nytDeck:
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



        // SW,Show deck
    } else if (strcasecmp(input, "SW") == 0) {
        CardNode *current = board->deck.head;
        while (current) {
            current->card.faceUp = 1;
            current = current->next;
        }
        strcpy(lastCommand,"SW");
        strcpy(message,"Kort er nu vist");
return STARTUP;
        //SI, Split
    } else if (strcasecmp(input, "SI") == 0) {
        char *endptr;
        long cutPoint = strtol(arg, &endptr, 10);
        strcpy(lastCommand, "SI");

        if (endptr == arg || *endptr != '\0') {
            strcpy(message, "Ugyldigt input – skriv et tal.");
        } else if (cutPoint <= 0 || cutPoint >= board->deck.size) {
            strcpy(message, "Ugyldigt splitpunkt.");
        } else {
            splitShuffle(&board->deck, (int)cutPoint);
            strcpy(message, "Deck splittet og blandet.");
        }

        return STARTUP;
        // SR, = randomShuffle
    } else if (strcasecmp(input, "SR") == 0) {
        randomShuffle(&board->deck);
        strcpy(lastCommand,"SR");
        strcpy(message,"Shuffle random (ikke implementeret endnu)\n");

        //SD, Save deck
    } else if (strcasecmp(input, "SD") == 0) {
        strcpy(lastCommand, "SD");
        char *filename = arg;
        while (*filename == ' ') filename++;
        if (strlen(filename) == 0) filename = "cards.txt";

        writeDeckToFile(&board->deck, filename, message);
        return STARTUP;

    } else if (strcasecmp(input, "QQ") == 0) {
        strcpy(lastCommand,"QQ");
        strcpy(message,"Forlader spil - Tak for i dag!.\n");
        // Bare så det ser pænt ud.
        printBoardStartUpPhase(board, lastCommand, message);

        exit(0);

        //P, Start play phase
    } else if (strcasecmp(input, "P") == 0) {
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
         strcpy(lastCommand,"Invalid");
         strcpy(message,"Ugyldig kommando i startup-phase.\n");
    }
    return currentPhase;
}

GamePhase playPhase(Board *board, GamePhase currentPhase,const char *input, char *lastCommand, char *message) {

    //Q, Quit play phase
    if (strcasecmp(input, "Q") == 0) {
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

            //TODO har lavet en getranking den virker måske her.
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
    } else if (strcasecmp(input, "U") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Fortryder sidste træk...\n");
        //R, Redo
    } else if (strcasecmp(input, "R") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Gør træk om...\n");
        //S, Save game
    } else if (strcasecmp(input, "S") == 0) {
        strcpy(lastCommand,"SI");
        strcpy(message,"Gemmer spil...\n");
        //
        //L, Load game
        //
    } else if (strcasecmp(input, "L") == 0) {
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