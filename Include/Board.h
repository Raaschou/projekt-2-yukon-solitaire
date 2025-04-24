//
// Created by August Frandsen on 15/04/2025.
//

#ifndef BOARD_H
#define BOARD_H
#include "LinkedLists.h"
#include "Card.h"
#include <stdio.h>

typedef struct {
    LinkedList columns[7];
    LinkedList foundations[4];
} Board;

void printBoard(Board *board, const char *lastCommand, const char *message);

#endif //BOARD_H
