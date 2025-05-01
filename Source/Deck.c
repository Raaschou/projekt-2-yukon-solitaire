//
// Created by August Frandsen on 15/04/2025.
//
#include "../Include/Deck.h"

// laver første deck 1-52
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
            newCard.faceUp = 0;

            addCard(deck, newCard);
        }
    }
}

//hjælpe funktion der kopiere alle kort fra linkedlist til et array, bruges i de to shuffels.
void copyCardsFromList(LinkedList *deck, Card card[]) {
    CardNode *current = deck->head;
    int i = 0;
    while (current && i < deck->size) {
        card[i++] = current->card;
        current = current->next;
    }

}

// rydder vores hjælpe array, så vi ikke bruger unødig plads
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

// den shufler random ved hjælp af yates shuffel
void randomShuffle(LinkedList *deck) {
    srand(time(NULL));
    if (deck->size != 52) return;

    Card cards[52];
    copyCardsFromList(deck, cards);

// yates shuffle
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

// shuffler baseret på hvor man cutter decket.
// hvis der ikke bliver givet en cutsize, skal den tage et random tal mellem 1-51 // opgave kriterie starter fra venstre
void splitShuffle(LinkedList *deck, int cutSize) {
    if (cutSize <= 0 || cutSize >= deck->size) return;

    Card cards[52];
    copyCardsFromList(deck, cards);
    clearList(deck);

    int leftIndex = cutSize - 1;
    int rightIndex = 51;

    while (leftIndex >= 0 && rightIndex >= cutSize) {
        addCard(deck, cards[leftIndex--]);
        addCard(deck, cards[rightIndex--]);
    }

    while (leftIndex > 0) {
        addCard(deck, cards[leftIndex--]);
    }
    while (rightIndex >= cutSize ) {
        addCard(deck, cards[rightIndex--]);
    }
}


