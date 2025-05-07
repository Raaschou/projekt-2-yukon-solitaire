//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/LinkedLists.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Initialiserer en tom linked list ved at nulstille head, tail og size.
 *
 * @param list Pointer til listen der skal initialiseres.
 */
void initList(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/**
 * Frigiver hukommelse brugt af en linkedlist.
 *
 * @param list Pointer til den liste der skal nulstilles.
 */
void freeList(LinkedList *list) {
    CardNode *current = list->head;
    while (current != NULL) {
        CardNode *next = current->next;
        free(current);
        current = next;
    }
    initList(list);
}
/**
 * Rydder hele linked list og frigiver hukommelsen.
 *
 * @param deck Pointer til listen der skal tømmes.
 */
void clearList(LinkedList *deck) {
    CardNode *current = deck->head;
    while (current) {
        CardNode *next = current->next;
        free(current);
        current = next;
    }
    deck->head = NULL;
    deck->tail = NULL;
    deck->size = 0;
}
/**
 * Kopierer alle kort fra en linked list til en ny liste.
 * Der oprettes nye noder, men kortene kopieres direkte (shallow copy).
 *
 * @param dest Pointer til destinationen.
 * @param src Pointer til den liste der kopieres fra.
 */
void copyList(LinkedList *dest, const LinkedList *src) {
    initList(dest);

    CardNode *current = src->head;
    while (current != NULL) {
        Card copy = current->card; // shallow copy af Card er OK
        addCard(dest, copy); // vi allokerer nyt node
        current = current->next;
    }
}

/**
 * Tilføjer et kort til slutningen af en linked list.
 *
 * @param list Listen kortet tilføjes til.
 * @param card Kortet der skal tilføjes.
 */
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

/**
 * Tjekker om et givet CardNode findes i en linked list.
 *
 * @param node Kortet der søges efter.
 * @param list Listen der gennemsøges.
 * @return true hvis node findes i listen, ellers false.
 */
bool nodeInList(CardNode *node, LinkedList *list) {
    if (!node || !list || !list->head) return false;

    CardNode *current = list->head;
    while (current != NULL) {
        if (current == node) return true;
        current = current->next;
    }
    return false;
}

/**
 * Flytter en stak kort fra startNode til slutningen af newList.
 * Opdaterer links og størrelser i begge lister.
 *
 * @param startNode Første kort i stakken, der skal flyttes.
 * @param oldList Listen stakken fjernes fra.
 * @param newList Listen stakken flyttes til.
 */
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


/**
 * Printer et kort, hvis det findes og er i listen.
 * Viser "[ ]" hvis face down, ellers fx "9H".
 * Viser tomt felt hvis ugyldigt node.
 *
 * @param node Pointer til det kort, der skal udskrives.
 * @param list Listen kortet forventes at tilhøre.
 */
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

/**
 * Flytter ét kort fra starten af 'from'-listen til slutningen af 'to'-listen.
 * Antager at begge lister er dobbeltkædede og korrekt initialiseret.
 *
 * @param from Pointer til den liste, kortet fjernes fra.
 * @param to Pointer til den liste, kortet tilføjes til.
 */
void moveSingleCard(LinkedList *from, LinkedList *to) {
    if (!from || !from->head || !to) {
        return;
    }

    CardNode *card = from->head;

    // Fjern fra 'from'-listen
    if (card->next) {
        card->next->prev = NULL;
    } else {
        from->tail = NULL;
    }

    from->head = card->next;
    from->size--;

    // Tilføj til 'to'-listen i slutningen
    card->prev = to->tail;
    card->next = NULL;

    if (to->tail) {
        to->tail->next = card;
    } else {
        to->head = card;
    }

    to->tail = card;
    to->size++;
}

/**
 * Finder et kort i listen baseret på rank og suit.
 *
 * @param list Listen der gennemsøges.
 * @param rank Kortets rang (1–13).
 * @param suit Kortets kulør ('H', 'D', 'C', 'S').
 * @return Pointer til det fundne CardNode, ellers NULL.
 */
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

/**
 * Vender det sidste kort i listen, hvis det er face down.
 *
 * @param list Listen der kontrolleres.
 */
void flipLastCardIfAny(LinkedList *list) {
    if (list->tail && !list->tail->card.faceUp) {
        list->tail->card.faceUp = 1;
    }
}



