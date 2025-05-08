//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Game.h"
#include "../Include/Board.h"
#include "../Include/GUI.h"
#include "../Include/Deck.h"
#include "../Include/FileIO.h"
#include "../Include/Foundations.h"
#include "../Include/Columns.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>

/**
 * Starter game loop i terminal-tilstand.
 * Viser spilbrættet og håndterer brugerens input i begge faser.
 *
 * @param board Pointer til det aktuelle spilbræt.
 */
void gameLoopTerminal(Board *board) {
    GamePhase phase = STARTUP;
    char lastCommand[100] = "";
    char message[100] = "";
    char input[100];
    BoardStack undoStack, redoStack;
    initStack(&undoStack);
    initStack(&redoStack);

    while (1) {
        if (phase == STARTUP) {
            printBoardStartUpPhase(board, lastCommand, message);
        } else if (phase == PLAY) {
            if (isWinState(board)) {
                strcpy(message, "Du har vundet, flot, du har spildt noget af dit liv – tak for i dag!");
                printBoardStartUpPhase(board, lastCommand, message);
                exit(0);
            }
            printBoardPlayPhase(board, lastCommand, message);
        }

        // LÆS INPUT HVER GANG
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        // Kør kommando afhængig af fase
        if (phase == STARTUP) {
            phase = startupPhase(board, phase, input, lastCommand, message, &undoStack, &redoStack);
        } else if (phase == PLAY) {
            phase = playPhase(board, phase, input, lastCommand, message, &undoStack, &redoStack);
        }
    }
}

/**
 * Behandler brugerens kommandoer i startup-fasen.
 * Understøtter load deck,save deck, diverse bland, visning af kort og start af spillet (Skift af fase).
 *
 * @param board Pointer til spilbrættet.
 * @param currentPhase Den aktuelle spilfase.
 * @param input Brugerens indtastning som tekst.
 * @param lastCommand Sidste kommando.
 * @param message Status eller fejlbesked.
 * @return Den eventuelt opdaterede spilfase.
 */
GamePhase startupPhase(Board *board, GamePhase currentPhase, const char *input, char *lastCommand, char *message,
                    BoardStack *undoStack, BoardStack *redoStack) {
    char cmd[100] = "";
    char arg[100] = "";
    sscanf(input, "%s%99[^\n]", cmd, arg);

    if (strcasecmp(cmd, "LD") == 0) {
        clearList(&board->deck);
        // Gem sidste kommando
        strcpy(lastCommand, "LD");
        char *filename = arg;
        while (*filename == ' ') filename++;


        if (strlen(filename) > 0) {
            readDeckFromFile(filename, &board->deck, message);
        } else {
            // Lav standarddeck
            startDeck(&board->deck);
            strcpy(message, "OK");

        }
            return STARTUP;
    }
    // LOADGAME
    if (strcasecmp(input, "L") == 0 || strncasecmp(input, "L ", 2) == 0) {
        strcpy(lastCommand, "L");

        const char *filename = input + 1;
        while (*filename == ' ') filename++;

        if (strlen(filename) == 0) {
            strcpy(message, "FEJL: Angiv filnavn efter L.");
            return currentPhase;
        }

        loadGame(filename, board, undoStack, redoStack,message);
        strcpy(message, "Dit spil er indlæst og du kan nu spille");
        return PLAY;
    }
    // SW,Show deck
    if (strcasecmp(cmd, "SW") == 0) {
        strcpy(lastCommand, "SW");
        if (board->deck.size != 52) {
            strcpy(message, "Der er ikke nogen kort at vise");
            return STARTUP;
        }
        CardNode *current = board->deck.head;
        while (current) {
            current->card.faceUp = 1;
            current = current->next;
        }
        strcpy(message, "Kort er nu vist");
        return STARTUP;
        //SI, Split
    } if (strcasecmp(cmd, "SI") == 0) {
        char *endptr;
        long cutSize;
        strcpy(lastCommand, "SI");

        if (board->deck.size != 52) {
            strcpy(message, "Kan ikke shuffle, kortbunken er ugyldig.");
            return STARTUP;
        }

        if (arg == NULL || strlen(arg) == 0) {
            cutSize = rand() % (board->deck.size - 1) + 1; // mellem 1 og size-1
            splitShuffle(&board->deck, (int) cutSize);
            sprintf(message, "Deck splittet og blandet ved punkt %ld.", cutSize);
        } else {
            // Argument angivet
            cutSize = strtol(arg, &endptr, 10);
            if (endptr == arg || *endptr != '\0') {
                strcpy(message, "Ugyldigt input – skriv et tal.");
            } else if (cutSize <= 0 || cutSize >= board->deck.size) {
                strcpy(message, "Ugyldigt splitpunkt.");
            } else {
                splitShuffle(&board->deck, (int) cutSize);
                strcpy(message, "Deck splittet og blandet.");
            }
        }
        return STARTUP;
    }
    // SR, = randomShuffle
    if (strcasecmp(input, "SR") == 0) {
        strcpy(lastCommand, "SR");
        if (board->deck.size != 52) {
            strcpy(message, "Kan ikke shuffle, kortbunken er ugyldig.");
            return STARTUP;
        }
        randomShuffle(&board->deck);
        strcpy(message, "Shuffle random");
        return STARTUP;
    }
    //SD, Save deck
    if (strcasecmp(cmd, "SD") == 0) {
        strcpy(lastCommand, "SD");
        if (board->deck.size != 52) {
            strcpy(message, "Kan ikke shuffle, kortbunken er ugyldig.");
            return STARTUP;
        }
        strcpy(message, "Kort er nu gemt");
        char *filename = arg;
        while (*filename == ' ') filename++;
        if (strlen(filename) == 0) filename = "cards.txt";

        writeDeckToFile(&board->deck, filename, message);
        return STARTUP;
    } if (strcasecmp(input, "QQ") == 0) {
        strcpy(lastCommand, "QQ");
        strcpy(message, "Forlader spil - Tak for i dag!.\n");
        // Bare så det ser pænt ud.
        printBoardStartUpPhase(board, lastCommand, message);

        exit(0);

    } if (strcasecmp(input, "P") == 0) {
        strcpy(lastCommand, "P");
        if (board->deck.size == 0) {
            strcpy(message, "Der er ikke loadet et deck!");
            strcpy(lastCommand, input);
            return currentPhase;
        }
        if (board->deck.head == NULL) {
            initBoard(board);
        }

        dealToColumns(&board->deck, board->columns);
        clearList(&board->deck);
        strcpy(message, "Vi spiller!");
        return PLAY;
    }
        strcpy(lastCommand, "Invalid");
        strcpy(message, "Ugyldig kommando i startup-phase.\n");
    return currentPhase;
}

/**
 * Behandler brugerens kommandoer i play-fasen.
 * Understøtter træk, undo/redo, afslutning, samt gem og indlæs.
 *
 * @param board Pointer til spilbrættet.
 * @param currentPhase Den aktuelle spilfase.
 * @param input Brugerens indtastning som tekst.
 * @param lastCommand Sidste gyldige kommando.
 * @param message Status eller fejlbesked.
 * @param undoStack Pointer til undo-stak.
 * @param redoStack Pointer til redo-stak.
 * @return Eventuelt opdaterede spilfase.
 */
GamePhase playPhase(Board *board, GamePhase currentPhase, const char *input, char *lastCommand, char *message,BoardStack *undoStack, BoardStack *redoStack) {
    char localInput[100];
    strncpy(localInput, input, sizeof(localInput) - 1);
    localInput[sizeof(localInput) - 1] = '\0';

    for (int i = 0; localInput[i]; i++) {
        localInput[i] = toupper((unsigned char) localInput[i]);
    }

    if (strcmp(localInput, "U") == 0) {
        strcpy(lastCommand, "U");
        if (!isEmpty(undoStack)) {
            push(redoStack, board);    // Først push nuværende board til redoStack
            freeBoard(board);          // Ryd det gamle board
            *board = pop(undoStack);   // Hent det forrige board fra undoStack
            strcpy(message, "Træk fortrudt.");
        } else {
            strcpy(message, "Der er ikke noget at fortryde.");
        }
        return currentPhase;
    }

    if (strcmp(localInput, "R") == 0) {
        strcpy(lastCommand, "R");
        if (!isEmpty(redoStack)) {
            push(undoStack, board); // Først push nuværende board til undoStack
            freeBoard(board); // Ryd det gamle board
            *board = pop(redoStack); // Hent det næste board fra redoStack
            strcpy(message, "Træk gentaget.");
        } else {
            strcpy(message, "Der er ikke noget at gentage.");
        }
        return currentPhase;
    }

    if (strcmp(localInput, "Q") == 0) {
        strcpy(lastCommand, "Q");
        strcpy(message, "Afslutter spilfase...");
        freeBoard(board);
        clearStack(undoStack);
        clearStack(redoStack);
        return STARTUP;
    }

    if (strcasecmp(localInput, "S") == 0 || strncasecmp(localInput, "S ", 2) == 0) {
        strcpy(lastCommand, "S");

        char *filename = localInput + 1;
        while (*filename == ' ') filename++;

        if (strlen(filename) == 0) {
            // Brug default
            saveGame(NULL, board, undoStack, redoStack,message);
            strcpy(message, "Spil gemt til defaultSave.bin");
        } else {
            saveGame(filename, board, undoStack, redoStack,message);
            strcpy(message, "Spil gemt.");
        }

        return currentPhase;
    }



    if (!strstr(localInput, "->")) {
        strcpy(lastCommand, localInput);
        strcpy(message, "Ugyldig kommando i spilfase.");
        return currentPhase;
    }

    char from[100], to[100];
    if (sscanf(localInput, "%[^-]->%s", from, to) != 2) {
        strcpy(message, "Forkert format. Brug fx C3->F1 eller C2:KH->C4");
        return currentPhase;
    }

    strcpy(lastCommand, localInput);

    if (from[0] == 'C' && to[0] == 'F') {
        int colIndex = from[1] - '1';
        int fIndex = to[1] - '1';

        if (colIndex < 0 || colIndex >= 7 || fIndex < 0 || fIndex >= 4) {
            strcpy(message, "Ugyldig kolonne eller foundation.");
            return currentPhase;
        }

        LinkedList *source = &board->columns[colIndex];
        LinkedList *target = &board->foundations[fIndex];
        CardNode *card = source->tail;

        if (!card)
            strcpy(message, "Kolonnen er tom!");
        else if (validMoveF(card, target->tail)) {
            changeBoardStack(undoStack, redoStack, board);
            moveToFoundation(card, source, target);
            flipLastCardIfAny(source);
            strcpy(message, "OK");
        } else
            strcpy(message, "Ugyldigt træk!");

        return currentPhase;
    }

    if (from[0] == 'C' && strchr(from, ':')) {
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
            strcpy(message, "Kan ikke flytte skjulte kort.");
        } else if (validMoveC(node, dest->tail)) {
            changeBoardStack(undoStack, redoStack, board);
            moveBetweenColumns(node, source, dest);
            flipLastCardIfAny(source);
            strcpy(message, "OK");
        } else {
            strcpy(message, "Ugyldigt træk!");
        }

        return currentPhase;
    }

    if (from[0] == 'C' && to[0] == 'C') {
        int fromCol = from[1] - '1';
        int toCol = to[1] - '1';

        if (fromCol < 0 || fromCol >= 7 || toCol < 0 || toCol >= 7) {
            strcpy(message, "Ugyldig kolonne!");
            return currentPhase;
        }

        LinkedList *source = &board->columns[fromCol];
        LinkedList *dest = &board->columns[toCol];
        CardNode *node = source->tail;

        if (!node)
            strcpy(message, "Kolonnen er tom!");
        else if (!node->card.faceUp)
            strcpy(message, "Kan ikke flytte skjulte kort.");
        else if (validMoveC(node, dest->tail)) {
            changeBoardStack(undoStack, redoStack, board);
            moveBetweenColumns(node, source, dest);
            flipLastCardIfAny(source);
            strcpy(message, "OK");
        } else
            strcpy(message, "Ugyldigt træk!");

        return currentPhase;
    }

    if (from[0] == 'F' && to[0] == 'C') {
        int fromIndex = from[1] - '1';
        int toIndex = to[1] - '1';

        if (fromIndex < 0 || fromIndex >= 4 || toIndex < 0 || toIndex >= 7) {
            strcpy(message, "Ugyldig foundation eller kolonne!");
            return currentPhase;
        }

        LinkedList *source = &board->foundations[fromIndex];
        LinkedList *dest = &board->columns[toIndex];
        CardNode *node = source->tail;

        if (!node)
            strcpy(message, "Foundation er tom!");
        else if (validMoveC(node, dest->tail)) {
            changeBoardStack(undoStack, redoStack, board);
            moveBetweenColumns(node, source, dest);
            strcpy(message, "OK");
        } else
            strcpy(message, "Ugyldigt træk!");

        return currentPhase;
    }

    strcpy(message, "Ugyldig kommando i spilfase.");
    return currentPhase;
}
