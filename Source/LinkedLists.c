//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/LinkedLists.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// linked list// vi har en size som kan hjælpe os. bruges fx. i Board.C
void list(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// tilføjer et kort til en Linkedlist
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

//tjekke om det kort vi giver faktisk er i den liste.
bool nodeInList(CardNode *node, LinkedList *list) {
    if (!node || !list || !list->head) return false;

    CardNode *current = list->head;
    while (current != NULL) {
        if (current == node) return true;
        current = current->next;
    }
    return false;
}

// rykker stacken 1 til mange kort.
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

// printer et kort fra en liste
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

void moveSingleCard(LinkedList *from, LinkedList *to) {
    if (!from || !from->head) return;

    CardNode *card = from->head;

    // Fjern fra 'from'
    if (card->next)
        card->next->prev = NULL;
    else
        from->tail = NULL;

    from->head = card->next;
    from->size--;

    // Tilføj til 'to' i slutningen
    card->prev = to->tail;
    card->next = NULL;

    if (to->tail)
        to->tail->next = card;
    else
        to->head = card;

    to->tail = card;
    to->size++;
}

CardNode* findCardNode(LinkedList *list, int rank, char suit) {
    CardNode *current = list->head;
    while (current != NULL) {
        if (current->card.rank == rank && current->card.suit == suit) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void flipLastCardIfAny(LinkedList *list) {
    if (list->tail && !list->tail->card.faceUp) {
        list->tail->card.faceUp = 1;
    }
}