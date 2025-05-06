//
// Created by August Frandsen on 15/04/2025.
//

#ifndef BOARD_H
#define BOARD_H

#include "LinkedLists.h"
#include "Card.h"
#include <stdio.h>

#define MAX_HISTORY 100

typedef struct {
    LinkedList deck;
    LinkedList columns[7];
    LinkedList foundations[4];
} Board;

typedef struct {
    Board boards[MAX_HISTORY];
    int top;
} BoardStack;

// Board visning
void printBoardPlayPhase(Board *board, const char *lastCommand, const char *message);
void printBoardStartUpPhase(Board *board, const char *lastCommand, const char *message);
// Board håndtering
void freeBoard(Board *board);
void initBoard(Board *board);
void copyBoard(Board *dest, const Board *src);


// Boardstack-funktioner
void initStack(BoardStack *stack);
int isEmpty(BoardStack *stack);
int isFull(BoardStack *stack);
void push(BoardStack *stack, const Board *board);
Board pop(BoardStack *stack);
void clearStack(BoardStack *stack);
void changeBoardStack(BoardStack *undo, BoardStack *redo, Board *board);


// // Linked list håndtering
// void initList(LinkedList *list);
// void freeList(LinkedList *list);
// void copyList(LinkedList *dest, const LinkedList *src);

#endif // BOARD_H


