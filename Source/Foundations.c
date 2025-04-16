//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Foundations.h"
#include "../Include/Deck.h"

// TODO: Initialiser alle 4 foundations som tomme linked lists
void foundations(LinkedList columns[4]) {
    for (int i = 0; i < 4; i++) {
        list(&columns[i]);
    }
}

void moveToFoundation(CardNode *node,LinkedList *from, LinkedList *to) {
    validMove(from, to);
    moveStack(node, from, to);
}

// TODO: Tjek om kort må lægges i denne foundation
bool validMove(CardNode *source, CardNode *target) {

    return true;
}

// TODO: Udskriv alle 4 foundations
void printFoundations(LinkedList foundations[4]) {
    // TODO
}