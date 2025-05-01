//
// Created by August Frandsen on 15/04/2025.
//

#ifndef FILEIO_H
#define FILEIO_H


#include "LinkedLists.h"

void readDeckFromFile(LinkedList *deck, const char *filename);
void writeDeckToFile(LinkedList *deck, const char *filename);


#endif