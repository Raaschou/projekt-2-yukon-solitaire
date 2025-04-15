//
// Created by August Frandsen on 15/04/2025.
//

#ifndef COLUMNS_H
#define COLUMNS_H

#include "LinkedLists.h"

// Initialiser 7 kolonner
void init_columns(LinkedList columns[7]);

// Flyt kort fra én kolonne til en anden
int move_between_columns(LinkedList *from, LinkedList *to, int count);

// Print alle kolonner
void print_columns(LinkedList columns[7]);

// Valider om flyt er gyldigt (f.eks. skiftende farver, faldende rank)
int is_valid_column_move(CardNode *source, CardNode *target);

#endif