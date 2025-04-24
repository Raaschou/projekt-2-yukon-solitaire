//
// Created by August Frandsen on 15/04/2025.
//

#ifndef BOARD_H
#define BOARD_H
#include "LinkedLists.h"
#include "Card.h"
#include <stdio.h>

void printBoard(LinkedList columns[7], LinkedList foundations[4], const char *lastCommand, const char *message);

#endif //BOARD_H
