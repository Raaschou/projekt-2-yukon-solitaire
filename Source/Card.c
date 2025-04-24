//
// Created by August Frandsen on 15/04/2025.
//



#include "../Include/Card.h"
#include <stdio.h>
#include <stdlib.h>


// Suits og ranks til opbygning af deck
char suits[] = {'C', 'D', 'H', 'S'};
int ranks[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

// laver et kort
Card *createCard(char suit, int rank) {
    Card *newCard = malloc(sizeof(Card));
    if (!newCard) return NULL;

    newCard->suit = suit;
    newCard->rank = rank;
    // 1 = vis 0 = skjul
    newCard->faceUp = 1;

    return newCard;
}


