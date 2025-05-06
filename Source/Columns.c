//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Columns.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Initialiserer de 7 kolonner i spillet som tomme linked lists.
 *
 * @param columns Array af 7 LinkedList-strukturer der skal initialiseres.
 */
void columns(LinkedList columns[7]) {
    for (int i = 0; i < 7; i++) {
        initList(&columns[i]);
    }
}

/**
 * Flytter en kortstak mellem to kolonner, hvis trækket er gyldigt.
 * TODO tjek om if statement kan fjernes
 * @param node Kortet der skal flyttes.
 * @param from Kolonnen der flyttes fra.
 * @param to Kolonnen der flyttes til.
 */
void moveBetweenColumns(CardNode *node, LinkedList *from, LinkedList *to) {
    if (validMoveC(node, to->tail)) {
        moveStack(node, from, to);
    }
}

/**
 * Deler kort fra et deck ud til de 7 kolonner efter Yukon layoutet.

 *
 * @param deck Pointer til den linked list, som repræsenterer kortbunken.
 * @param columns Array med 7 linked lists (kolonner), som kortene fordeles til.
 */
void dealToColumns(LinkedList *deck, LinkedList columns[7]) {
    if (!deck) {
        printf("Deck er NULL!\n");
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

/**
 * Validerer om et kort, og eller stak, kan flyttes fra én kolonne til en anden.
 *
 * @param source Pointer til det kort der ønskes flyttet.
 * @param target Pointer til det kort der ligger i stakken der skal rykkes til.
 * @return true hvis trækket er gyldigt, ellers false.
 */
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