//
// Created by August Frandsen on 15/04/2025.
//
#ifndef CARD_H
#define CARD_H



typedef struct {
    int rank; // '1'-'9', 't', 'j', 'q', 'k'
    char suit;
    int faceUp; // 1 = oppe, 0 = nedad
} Card;
Card *createCard(char suit, int rank);


#endif