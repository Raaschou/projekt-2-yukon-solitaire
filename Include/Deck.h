//
// Created by August Frandsen on 15/04/2025.
//

#ifndef DECK_H
#define DECK_H
#include "../Include/Card.h"
#include "../Include/LinkedLists.h"
#include "../Include/Deck.h"
#include "stdlib.h"
#include "time.h"

void startDeck(LinkedList *deck);
void copyCardsFromList(LinkedList *deck, Card card[]);
void clearList(LinkedList *deck);
void randomShuffle(LinkedList *deck);
void splitShuffle(LinkedList *deck, int cutSize);
#endif //DECK_H
