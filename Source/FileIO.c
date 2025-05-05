//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/FileIO.h"

#include <stdio.h>
#include <string.h>




int readDeckFromFile(const char *filename, LinkedList *deck, char *message) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        sprintf(message, "ERROR: Kunne ikke åbne '%s'.", filename);
        return 0;
    }

    char seen[52] = {0};
    char line[8];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        Card card;
        if (!validateDeck(line, &card, lineNumber, seen, message)) {
            fclose(file);
            return 0;
        }
        addCard(deck, card);
        deck->tail->card.faceUp = 0;
    }

    fclose(file);

    if (deck->size != 52) {
        sprintf(message, "ERROR: Forventede 52 kort, fandt %d.", deck->size);
        return 0;
    }

    strcpy(message, "OK");
    return 1;
}


void writeDeckToFile(LinkedList *deck, const char *filename, char *message) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        sprintf(message, "Kunne ikke åbne filen: %s", filename);
        return;
    }

    CardNode *node = deck->head;
    while (node) {
        Card card = node->card;
        char rankChar = getRankChar(card.rank);  // fx 'A', '7', 'K'
        fprintf(file, "%c%c\n", rankChar, card.suit);  // fx AH, 7D, QC
        node = node->next;
    }

    fclose(file);
    sprintf(message, "Deck gemt til %s", filename);
}