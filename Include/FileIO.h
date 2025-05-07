//
// Created by August Frandsen on 15/04/2025.
//

#ifndef FILEIO_H
#define FILEIO_H


#include "LinkedLists.h"
#include "Deck.h"
#include "Board.h"

int readDeckFromFile(const char *filename, LinkedList *deck, char *message);
void writeDeckToFile(LinkedList *deck, const char *filename,char *message);
void saveGame(const char *filename, Board *board, BoardStack *undo, BoardStack *redo, char *message);
void loadGame(const char *filename, Board *board, BoardStack *undo, BoardStack *redo, char *message);
void writeListBinary(FILE *file, LinkedList *list);
void readListBinary(FILE *file, LinkedList *list);
void writeBoardBinary(FILE *file, Board *board);
void readBoardBinary(FILE *file, Board *board);
void writeStackBinary(FILE *file, BoardStack *stack);
void readStackBinary(FILE *file, BoardStack *stack);


#endif