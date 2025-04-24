#include "../Include/Commands.h"
#include <stdio.h>
#include <string.h>

#include "../Include/Board.h"

void executeCommand(GamePhase phase, void *command, char *message) {
    if (phase == STARTUP) {
        StartupCommand cmd = *(StartupCommand*)command;
        switch (cmd) {
            case LD:
                // load deck from file mangler impelementering
                strcpy(message, "Indlæser deck...");
            break;
            case SW:
                // Set all cards face to 1.
                strcpy(message, "Viser deck...");
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
            default:
                strcpy(message, "Ugyldig kommando i spilfasen!");
            break;
        }
    }
    else {
        strcpy(message, "Ugyldig fase!");
    }
}
