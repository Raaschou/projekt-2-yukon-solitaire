//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Columns.h"
#include <stdbool.h>

void columns(LinkedList columns[7]) {
    for (int i = 0; i < 7; i++) {
        list(&columns[i]);
    }
}



void moveBetweenColumns(CardNode *node, LinkedList *from, LinkedList *to) {
    if (validMoveC(node, to->tail)) {
        moveStack(node, from, to);
    } else {
       // message =("Ugyldigt træk!\n");
    }
}

void dealToColumns(LinkedList *deck, LinkedList columns[7]) {
    int layout[7] = {1, 6, 7, 8, 9, 10, 11}; // Antal kort per kolonne
    int faceDownCount = 21;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < layout[i]; j++) {
            CardNode *node = deck->tail;
            moveStack(node, deck, &columns[i]);
            if (faceDownCount > 0) {
                node->card.faceUp = 0;
                faceDownCount--;
            } else {
                node->card.faceUp = 1;
            }
        }
    }
}

//Tjekker om det kort vi prøver at rykke matcher kriterierne i forhold til at rykke til ny
bool validMoveC(CardNode *source, CardNode *target) {
    if (!source) return false;
    if (!target) {
        return source->card.rank == 13;
    }
    int srcRank = source->card.rank;
    int tgtRank = target->card.rank;
    char srcSuit = source->card.suit;
    char tgtSuit = target->card.suit;
    bool isSrcRed = (srcSuit == 'H' || srcSuit == 'D');
    bool isTgtRed = (tgtSuit == 'H' || tgtSuit == 'D');

    return (srcRank + 1 == tgtRank) && (isSrcRed != isTgtRed);
}