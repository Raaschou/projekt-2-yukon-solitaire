//
// Created by August Frandsen on 15/04/2025.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Card.h"

typedef struct CardNode {
    Card card;
    struct CardNode *next;
} CardNode;

typedef struct {
    CardNode *head;
    CardNode *tail;
    int size;
} LinkedList;

void init_list(LinkedList *list);
void append_card(LinkedList *list, Card card);
CardNode* pop_card(LinkedList *list);
void free_list(LinkedList *list);

#endif