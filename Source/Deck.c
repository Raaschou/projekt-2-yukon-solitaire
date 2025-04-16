//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Deck.h"
#include "../Include/Card.h"
#include "../Include/LinkedLists.h"

void startDeck(LinkedList *deck) {
    if (!deck) return;

    char suits[] = {'C', 'D', 'H', 'S'};

    for (int s = 0; s < 4; s++) {
        for (int r = 1; r <= 13; r++) {
            // gav lidt problemer;(
            // Card *newCard = createCard(suits[s], r);
            // addCard(deck, *newCard);

            Card newCard;
            newCard.suit = suits[s];
            newCard.rank = r;
            newCard.faceUp = 1;

            addCard(deck, newCard);
        }
    }
}