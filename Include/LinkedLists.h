//
// Created by August Frandsen on 15/04/2025.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdlib.h>
#include <stdbool.h>
#include "Card.h"

typedef struct CardNode {
    Card card;
    struct CardNode *next;
    struct CardNode *prev;
} CardNode;

typedef struct {
    CardNode *head;
    CardNode *tail;
    int size;
} LinkedList;
bool nodeInList(CardNode *node, LinkedList *list);
void list(LinkedList *list);
void addCard(LinkedList *list, Card card);
void moveStack(CardNode *startNode, LinkedList *from, LinkedList *to);
void printCard(CardNode *node, LinkedList *list);



#endif