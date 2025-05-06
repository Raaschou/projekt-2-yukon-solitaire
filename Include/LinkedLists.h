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


void initList(LinkedList *list);
void freeList(LinkedList *list);
void clearList(LinkedList *list);
void copyList(LinkedList *dest, const LinkedList *src);
void addCard(LinkedList *list, Card card);
bool nodeInList(CardNode *node, LinkedList *list);
void moveStack(CardNode *startNode, LinkedList *from, LinkedList *to);
void printCard(CardNode *node, LinkedList *list);
void moveSingleCard(LinkedList *from, LinkedList *to);
CardNode* findCardNode(LinkedList *list, int rank, char suit);
void flipLastCardIfAny(LinkedList *list);



#endif