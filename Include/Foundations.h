//
// Created by August Frandsen on 15/04/2025.
//

#ifndef FOUNDATIONS_H
#define FOUNDATIONS_H

#include "../Include/LinkedLists.h"
#include "../Include/Deck.h"
#include <stdbool.h>

void foundations(LinkedList foundations[4]);

void moveToFoundation(CardNode *node,LinkedList *from, LinkedList *to);

bool validMoveF(CardNode *source, CardNode *target);

#endif