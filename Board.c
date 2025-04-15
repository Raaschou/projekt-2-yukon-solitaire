#include <stdio.h>
#include "Board.h"
#include <stdbool.h>

bool isFaceUp = false;

// tjek om et deck er loaded
//bool loadedDeck = false;

void revealCard(/*card*/) {
    if (isFaceUp) {
        printf("[]\t");
    } else {
        printf(""/*card*/);
    }

}
void printInitialBoard() {
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n"
           "                              [] F1\n"
           "                              [] F2\n"
           "                              [] F3\n"
           "                              [] F4\n"
           "                                   \n"
           "LAST Command:                      \n"
           "Message:                           \n"
           "INPUT >                            \n"
        );
}

void printBoardLD(/*deck*/) {
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");
    for (int i = 0; i < 52; ++i) {
        for (int j = 0; j < 7; ++j) {
            revealCard(/*card*/);
        }
        printf("\n");
    }

}

