//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Columns.h"

// TODO: Initialiser alle 7 kolonner som tomme linked lists
void init_columns(LinkedList columns[7]) {
    for (int i = 0; i < 7; i++) {
        init_list(&columns[i]);
    }
}

// TODO: Flyt `count` kort fra én kolonne til en anden, hvis gyldigt
int move_between_columns(LinkedList *from, LinkedList *to, int count) {
    // TODO: Tjek validitet, flyt kortene, returnér 1 ved succes, 0 ellers
    return 0;
}

// TODO: Udskriv kolonner i pænt format
void print_columns(LinkedList columns[7]) {
    // TODO
}

// TODO: Tjek regler for om flyt er gyldigt (alternating color, descending)
int is_valid_column_move(CardNode *source, CardNode *target) {
    // TODO
    return 1;
}