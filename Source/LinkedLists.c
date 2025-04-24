//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/LinkedLists.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void list(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void addCard(LinkedList *list, Card card) {
    CardNode *newNode = malloc(sizeof(CardNode));
    if (!newNode) return;

    newNode->card = card;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }

    list->size++;
}

bool nodeInList(CardNode *node, LinkedList *list) {
    if (!node || !list || !list->head) return false;

    CardNode *current = list->head;
    while (current != NULL) {
        if (current == node) return true;
        current = current->next;
    }
    return false;
}

void moveStack(CardNode *startNode, LinkedList *oldList, LinkedList *newList) {
    if (!startNode || !oldList || !newList) return;
    if (!nodeInList(startNode, oldList)) return;

    CardNode *newTail = startNode->prev;

    if (newTail) {
        newTail->next = NULL;
    } else {
        oldList->head = NULL;
    }
    oldList->tail = newTail;
    startNode->prev = newList->tail;

    if (newList->tail) {
        newList->tail->next = startNode;
    } else {
        newList->head = startNode;
    }

    CardNode *p = startNode;
    int moved = 1;
    while (p->next != NULL) {
        p = p->next;
        moved++;
    }
    p->next = NULL;
    newList->tail = p;

    oldList->size -= moved;
    newList->size += moved;
}

void printCard(CardNode *node, LinkedList *list) {
    if (!node || !list || !nodeInList(node, list)) {
        printf("  \t");
        return;
    }

    if (node->card.faceUp) {
        switch (node->card.rank) {
            case 1: printf("A%c\t", node->card.suit);
                break;
            case 10: printf("T%c\t", node->card.suit);
                break;
            case 11: printf("J%c\t", node->card.suit);
                break;
            case 12: printf("Q%c\t", node->card.suit);
                break;
            case 13: printf("K%c\t", node->card.suit);
                break;
            default: printf("%d%c\t", node->card.rank, node->card.suit);
        }
    } else {
        printf("[]\t");
    }
}



