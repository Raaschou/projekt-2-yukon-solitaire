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

// TODO: Forsøg at lægge kort på foundation. Returnér 1 hvis succes.
int moveToFoundation(LinkedList *foundation, Card card) {
    // TODO: Brug is_valid_foundation_move til at afgøre
    return 0;
}

// TODO: Tjek om kort må lægges i denne foundation
int validMove(const LinkedList *foundation, const Card *card) {
    // TODO: Samme suit og stigende rank
    return 1;
}

// TODO: Udskriv alle 4 foundations
void printFoundations(LinkedList foundations[4]) {
    // TODO
}