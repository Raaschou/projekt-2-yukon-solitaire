//
// Created by August Frandsen on 15/04/2025.
//

#ifndef COLUMNS_H
#define COLUMNS_H

#include "LinkedLists.h"

void columns(LinkedList columns[7]);
int move_between_columns(LinkedList *from, LinkedList *to, int count);
void print_columns(LinkedList columns[7]);
int is_validmove(CardNode *source, CardNode *target);

#endif