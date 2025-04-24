//
// Created by August Frandsen on 15/04/2025.
//

#ifndef COLUMNS_H
#define COLUMNS_H

#include "LinkedLists.h"
#include <stdbool.h>

void columns(LinkedList columns[7]);
void moveBetweenColumns(CardNode *node,LinkedList *from, LinkedList *to);
void dealToColumns(LinkedList *deck, LinkedList columns[7]);
bool validMoveC(CardNode *source, CardNode *target);

#endif