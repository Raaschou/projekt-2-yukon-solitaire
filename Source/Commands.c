#include "../Include/Commands.h"
#include "../Include/Deck.h"
#include "../Include/LinkedLists.h"
#include "../Include/Board.h"
// #include "../Include/Board.h"
// #include "../Include/Board.h"
#include <stdio.h>
#include <string.h>



// tjek h fil for mere info.
void executeCommand(GamePhase phase, void *command, char *message/*, LinkedList *deck*/) {
    if (phase == STARTUP) {
        StartupCommand cmd = *(StartupCommand*)command;
        switch (cmd) {
            case LD:
                LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
                startDeck(list);
                strcpy(message, "Indlæser deck...");
            break;
            case SW:
            //     if (deck == NULL || deck->size == 0) {
            //         strcpy(message, "Der er intet Deck indlæst ven :'(");
            //         return;
            //     }
            //
            // CardNode *current = deck->head;
            // while (current != NULL) {
            //     Card *card = &current->card;
            //     if (card->faceUp) {
            //         printf("%d%c ", card->rank, card->suit);  // Format: 5D for 5 of Diamonds
            //     } else {
            //         printf("X%c ", card->suit);  // X for face down card
            //     }
            //     current = current->next;
            // }
            // printf("\n");
            // strcpy(message, "OK");

            break;
            case SI:
                // skal kalde split, på deck
                strcpy(message, "Blander deck )...");
            break;
            case SR:
                // skal kalde randomShuffel på deck
                strcpy(message, "Blander deck (random)...");
            break;
            case SD:
                // save deck to file mangler implementering
                strcpy(message, "Gemmer deck...");
            break;
            case QQ:
                // forlader programmet // mangler implementering
                strcpy(message, "Afslutter programmet...");
            break;
            case P:
                // skift fase
                strcpy(message, "Skifter til spilfase...");
            break;
            case INVALIDS:
                strcpy(message, "Ugyldig kommando i startfasen!");
            break;
            default:
                strcpy(message, "Ugyldig kommando i startfasen!");
            break;
        }
    }
    else if (phase == PLAY) {
        PlayCommand cmd = *(PlayCommand*)command;
        switch (cmd) {
            case Q:
                // skifter til Startphase
                strcpy(message, "Afslutter spilfase...");
            break;
            case MOVES:
                // mangler meget men er håndetering af ryk
                strcpy(message, "Viser mulige træk...");
            break;
            case U:
                // undo//mangler implementering
                strcpy(message, "Fortryder sidste træk...");
            break;
            case R:
                // redo mangler implementering
                strcpy(message, "Genopretter træk...");
            break;
            case S:
                // save state to file not just a deck, but correct columns
                strcpy(message, "Gemmer spil...");
            break;
            case L:
                // load saved state from file, not just a deck, but correct columns
                strcpy(message, "Indlæser spil...");
            break;
            case INVALIDP:
                strcpy(message, "Ugyldig kommando i startfasen!");
            break;
            default:
                strcpy(message, "Ugyldig kommando i spilfasen!");
            break;
        }
    }
    else {
        strcpy(message, "Ugyldig fase!");
    }
}

StartupCommand parseStartupCommand(const char *input) {
    if (strcmp(input, "LD") == 0) return LD;
    if (strcmp(input, "SW") == 0) return SW;
    if (strcmp(input, "SI") == 0) return SI;
    if (strcmp(input, "SR") == 0) return SR;
    if (strcmp(input, "SD") == 0) return SD;
    if (strcmp(input, "QQ") == 0) return QQ;
    if (strcmp(input, "P") == 0) return P;
    return INVALIDS;
}

PlayCommand parsePlayCommand(const char *input) {
    if (strcmp(input, "Q") == 0) return Q;
    if (strcmp(input, "MOVES") == 0) return MOVES;
    if (strcmp(input, "U") == 0) return U;
    if (strcmp(input, "R") == 0) return R;
    if (strcmp(input, "S") == 0) return S;
    if (strcmp(input, "L") == 0) return L;
    return INVALIDP;  // Or define a CMD_INVALID for play if needed
}