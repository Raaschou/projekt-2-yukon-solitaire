//
// Created by August Frandsen on 15/04/2025.
//

#ifndef FOUNDATIONS_H
#define FOUNDATIONS_H

#include "LinkedLists.h"

void init_foundations(LinkedList foundations[4]);

// Læg kort på foundation hvis gyldigt
int move_to_foundation(LinkedList *foundation, Card card);

// Tjek om flyt til foundation er lovligt
int is_valid_foundation_move(const LinkedList *foundation, const Card *card);

// Udskriv foundations
void print_foundations(LinkedList foundations[4]);

#endif