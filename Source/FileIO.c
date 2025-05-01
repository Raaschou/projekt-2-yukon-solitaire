//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/FileIO.h"

#include <stdio.h>
#include <string.h>




void readDeckFromFile(LinkedList *deck, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return;
    }

    char line[4];
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) < 2) continue;

        Card card;
        card.rank = line[0];
        card.suit = line[1];
        card.faceUp = 1;

        addCard(deck, card);
    }

    fclose(file);
}

// TODO: Gem board til tekstfil
void writeDeckToFile(LinkedList *deck, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Could not open file");
        return;
    }

    CardNode *current = deck->head;
    while (current) {
        fprintf(file, "%c%c\n", current->card.rank, current->card.suit);
        current = current->next;
    }

    fclose(file);
}