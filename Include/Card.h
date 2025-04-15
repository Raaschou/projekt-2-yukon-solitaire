//
// Created by August Frandsen on 15/04/2025.
//
#ifndef CARD_H
#define CARD_H

typedef enum { HEARTS, CLUBS, DIAMONDS, SPADES } Suit;
typedef struct {
    char rank; // '1'-'9', 't', 'j', 'q', 'k'
    Suit suit;
    int faceUp; // 1 = oppe, 0 = nedad
} Card;

void print_card(const Card *card);

#endif