//
// Created by August Frandsen on 15/04/2025.
//



#include "../Include/Card.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Array med mulige suits og ranks
 */
char suits[] = {'C', 'D', 'H', 'S'};
int ranks[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

/**
 * Initialiserer et nyt kort med kulør og rank.
 * Kortet sættes som face-up til start.
 *
 * @param suit Kulør på kortet ('C', 'D', 'H', 'S').
 * @param rank Rank af kortet (1 til 13).
 * @return Det nye kort
 */
Card *createCard(char suit, int rank) {
    Card *newCard = malloc(sizeof(Card));
    if (!newCard) return NULL;

    newCard->suit = suit;
    newCard->rank = rank;
    newCard->faceUp = 1;

    return newCard;
}


