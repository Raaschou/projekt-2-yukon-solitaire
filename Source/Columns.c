//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Columns.h"
#include "../Include/Deck.h"

void columns(LinkedList columns[7]) {
    for (int i = 0; i < 7; i++) {
        list(&columns[i]);
    }
}


void moveBetweenColumns(CardNode *node,LinkedList *from, LinkedList *to) {
    validMove(from, to);
    moveStack(node, from, to);
}



// TODO: Udskriv kolonner i pænt format
void printColumns(LinkedList columns[7]) {

      for (int i = 0; i < 7; i++) {

      }
}

// TODO: Tjek regler for om flyt er gyldigt (alternating color, descending)
bool validMove(CardNode *source, CardNode *target) {
    // TODO
    return true;
}