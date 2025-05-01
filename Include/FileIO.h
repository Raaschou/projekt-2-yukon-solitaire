//
// Created by August Frandsen on 15/04/2025.
//

#ifndef FILEIO_H
#define FILEIO_H


#include "LinkedLists.h"
#include "Deck.h"

int readDeckFromFile(const char *filename, LinkedList *deck, char *message);
void writeDeckToFile(LinkedList *deck, const char *filename);


#endif