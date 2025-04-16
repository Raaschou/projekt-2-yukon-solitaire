//
// Created by August Frandsen on 15/04/2025.
//

#ifndef COLUMNS_H
#define COLUMNS_H

#include "LinkedLists.h"

void columns(LinkedList columns[7]);
void moveBetweenColumns(LinkedList *from, LinkedList *to, int count);
void printColumns(LinkedList columns[7]);
bool validMove(CardNode *source, CardNode *target);

#endif