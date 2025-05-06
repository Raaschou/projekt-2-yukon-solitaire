//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Columns.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void columns(LinkedList columns[7]) {
    for (int i = 0; i < 7; i++) {
        initList(&columns[i]);
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

/**
 * Fordeler kort fra deck til de 7 kolonner ved spilstart.
 *
 * Giver ét kort ad gangen til hver kolonne fra venstre mod højre,
 * indtil hver kolonne har det ønskede antal kort.
 * De første kort i hver kolonne vendes med billedsiden nedad.
 *
 * @param deck    Deck med 52 kort i korrekt rækkefølge (head → tail).
 * @param columns Array af 7 kolonner (linked lists) som skal fyldes.
 */
void dealToColumns(LinkedList *deck, LinkedList columns[7]) {
    if (!deck) {
        printf("❌ deck er NULL!\n");
        exit(1);
    }
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
                    columns[i].tail->card.faceUp = 0;
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

    if (source->card.suit == target->card.suit) {
        return false;
    }

    return (srcRank + 1 == tgtRank);
}