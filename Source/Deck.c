//
// Created by August Frandsen on 15/04/2025.
//
#include "stdlib.h"
#include "time.h"
#include "../Include/Deck.h"
#include "../Include/Card.h"
#include "../Include/LinkedLists.h"

void startDeck(LinkedList *deck) {
    char suits[] = {'C', 'D', 'H', 'S'};

    for (int i = 0; i < 4; i++) {
        for (int j = 1; j <= 13; j++) {
            // gav lidt problemer;(
            // Card *newCard = createCard(suits[i], j);
            // addCard(deck, *newCard);

            Card newCard;
            newCard.suit = suits[i];
            newCard.rank = j;
            newCard.faceUp = 1;

            addCard(deck, newCard);
        }
    }
}

void copyCardsFromList(LinkedList *deck, Card card[]) {
    CardNode *current = deck->head;
    int i = 0;
    while (current && i < deck->size) {
        card[i++] = current->card;
        current = current->next;
    }

}

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

void randomShuffle(LinkedList *deck) {
    if (deck->size != 52) return;

    Card cards[52];
    copyCardsFromList(deck, cards);


    for (int i = 51; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }

    clearList(deck);

    for (int i = 0; i < 52; i++) {
        addCard(deck, cards[i]);
    }
}

void splitShuffle(LinkedList *deck, int cutSize) {
    if (cutSize <= 0 || cutSize >= deck->size) return;

    Card cards[52];
    copyCardsFromList(deck, cards);
    clearList(deck);

    int leftIndex = -1;
    int rightIndex = cutSize;

    while (leftIndex < cutSize && rightIndex < 52) {
        addCard(deck, cards[++leftIndex]);
        addCard(deck, cards[rightIndex++]);
    }

    while (leftIndex < cutSize) {
        addCard(deck, cards[++leftIndex]);
    }
    while (rightIndex < 52) {
        addCard(deck, cards[rightIndex++]);
    }
}
