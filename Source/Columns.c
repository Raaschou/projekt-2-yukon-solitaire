//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Columns.h"
#include <stdbool.h>
#include <stddef.h>

void columns(LinkedList columns[7]) {
    for (int i = 0; i < 7; i++) {
        list(&columns[i]);
    }
}


// rykker fra en list til en anden bruge Bool validmove til at tjekke om det er gyldigt
void moveBetweenColumns(CardNode *node, LinkedList *from, LinkedList *to) {
    if (validMoveC(node, to->tail)) {
        moveStack(node, from, to);
    } else {
       // message =("Ugyldigt træk!\n");
    }
}

// deler kortne ud, så det matcher hvordan spillet skal spilles.
//TODO sikre os de to faser, lige nu er den sat til Playphase
// der er i øvrigt en lille fejl lige nu
void dealToColumns(LinkedList *deck, LinkedList columns[7]) {
    int layout[7]        = {1, 6, 7, 8, 9, 10, 11};  // max antal kort pr kolonne
    int faceDownCount[7] = {0, 1, 2, 3, 4, 5, 6};    // antal skjulte kort


    int done = 0;
    while (!done) {
        done = 1;

        for (int i = 0; i < 7; i++) {
            if (0 < layout[i]) {
                moveSingleCard(deck, &columns[i]);
                layout[i]--;
                done = 0;

                if (faceDownCount[i] > 0) {
                    columns[i].tail->card.faceUp = 1;
                    faceDownCount[i]--;
                } else {
                    columns[i].tail->card.faceUp = 1;
                }
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