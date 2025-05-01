//
// Created by August Frandsen on 15/04/2025.
//
#include "../Include/Deck.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

// laver første deck 1-52
void startDeck(LinkedList *deck) {
    char suits[] = {'C', 'D', 'H', 'S'};

    for (int i = 0; i < 4; i++) {
        for (int j = 1; j <= 13; j++) {

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
int validateDeck(const char *line, Card *outCard, int lineNum, char seen[52]) {
    if (strlen(line) < 2) {
        printf("ERROR: Tom eller kort linje på linje %d.\n", lineNum);
        return 0;
    }

    char rank = toupper(line[0]);
    char suit = toupper(line[1]);

    // Ranks
    int r;
    switch (rank) {
        case 'A': r = 0; break;
        case 'T': r = 9; break;
        case 'J': r = 10; break;
        case 'Q': r = 11; break;
        case 'K': r = 12; break;
        case '2'...'9': r = rank - '2' + 1; break;
        default:
            printf("ERROR: Ugyldig rank '%c' på linje %d.\n", rank, lineNum);
        return 0;
    }

    // Suits
    int s;
    switch (suit) {
        case 'C': s = 0; break;
        case 'D': s = 1; break;
        case 'H': s = 2; break;
        case 'S': s = 3; break;
        default:
            printf("ERROR: Ugyldig kulør '%c' på linje %d.\n", suit, lineNum);
        return 0;
    }

    int index = s * 13 + r;
    if (seen[index]) {
        printf("ERROR: Dubletkort '%c%c' på linje %d.\n", rank, suit, lineNum);
        return 0;
    }

    seen[index] = 1;
    outCard->rank = rank;
    outCard->suit = suit;
    outCard->faceUp = 0;

    return 1;
}


