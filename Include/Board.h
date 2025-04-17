#ifndef BOARD_H
#define BOARD_H

#include "LinkedLists.h"
#include "Card.h"
#include <stdio.h>


typedef struct {
    LinkedList deck;
    LinkedList columns[7];
    LinkedList foundations[4];
    char message[128];
    char lastCommand[128];
    char inputLine[128];
} Board;

void init_board(Board *board);
void print_board(const Board *board);

#endif