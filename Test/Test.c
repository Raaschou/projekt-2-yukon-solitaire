//
// Created by August Frandsen on 16/04/2025.
//
#include "Test.h"



void testCreateCard() {
    char suits[] = {'C', 'D', 'H', 'S'};

    int cardNumber = rand() % 13;
    char suit =suits[rand() % 4];

    Card *card = createCard(suit, cardNumber);


    // printf(card, cardNumber, suit);

    if (card == NULL) {
        printf("Card not initialised\n");
    } else if (card->rank != cardNumber) {
        printf("Rank not initialised properly\n");
    } else if (card->suit != suit) {
        printf("Suit not initialised properly\n");
    } else {
        printf("Card created right you beautiful beast\n");
    }
}