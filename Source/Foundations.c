//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Foundations.h"
#include <stdbool.h>

/**
 * Initialiserer 4 foundation-lister.
 *
 * @param columns Array med 4 LinkedList-strukturer til foundations.
 */
void foundations(LinkedList columns[4]) {
    for (int i = 0; i < 4; i++) {
        initList(&columns[i]);
    }
}


/**
 * Flytter et kort fra en kolonne til en foundation, hvis det er et gyldigt træk.
 *
 * @param node Kortet der forsøges flyttet.
 * @param from Listen kortet kommer fra.
 * @param to Foundation-listen kortet skal til.
 */
void moveToFoundation(CardNode *node,LinkedList *from, LinkedList *to) {
    if (validMoveF(node, to->tail)) {
        moveStack(node, from, to);
    }
}


/**
 * Tjekker om et kort må flyttes til en foundation.
 * Et es må ligge først, derefter samme kulør og stigende rank.
 *
 * @param source Kortet der skal flyttes.
 * @param target Sidste kort i foundation.
 * @return true hvis trækket er gyldigt, ellers false.
 */
bool validMoveF(CardNode *source, CardNode *target) {
    if (!source) return false;

    if (!target) {
        return source->card.rank == 1; // Start me Ace
    }

    return (source->card.suit == target->card.suit &&
            source->card.rank == target->card.rank + 1);
}