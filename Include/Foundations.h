//
// Created by August Frandsen on 15/04/2025.
//

#ifndef FOUNDATIONS_H
#define FOUNDATIONS_H

#include "LinkedLists.h"

void foundations(LinkedList foundations[4]);

void moveToFoundation(CardNode *node,LinkedList *from, LinkedList *to);

bool validMove(CardNode *source, CardNode *target);

void printFoundations(LinkedList foundations[4]);

#endif