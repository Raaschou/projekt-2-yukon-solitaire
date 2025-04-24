//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Foundations.h"


// TODO: Initialiser alle 4 foundations som tomme linked lists
void foundations(LinkedList columns[4]) {
    for (int i = 0; i < 4; i++) {
        list(&columns[i]);
    }
}

void moveToFoundation(CardNode *node,LinkedList *from, LinkedList *to) {
    if (validMove(from, to)) {
        moveStack(node, from, to);
    } else {
        //message ="Cant Move this card to foundation";
    }
}


// TODO: Tjek om kort må lægges i denne foundation
bool validMove(CardNode *source, CardNode *target) {
    if (!source) return false;

    if (!target) {
        return source->card.rank == 1; // Start med Ace
    }

    return (source->card.suit == target->card.suit &&
            source->card.rank == target->card.rank + 1);
}