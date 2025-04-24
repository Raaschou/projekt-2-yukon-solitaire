//
// Created by August Frandsen on 15/04/2025.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Card.h"
#include <stdbool.h>

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


void list(LinkedList *list);
void addCard(LinkedList *list, Card card);
bool nodeInList(CardNode *node, LinkedList *list);
void moveStack(CardNode *startNode, LinkedList *from, LinkedList *to);
void printCard(CardNode *node, LinkedList *list);



#endif