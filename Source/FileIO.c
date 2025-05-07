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
            clearList(deck);
            fclose(file);
            return 0;
        }
        addCard(deck, card);
        deck->tail->card.faceUp = 0;
    }

    fclose(file);

    if (deck->size != 52) {
        sprintf(message, "ERROR: Forventede 52 kort, fandt %d.", deck->size);
        clearList(deck);
        return 0;
    }

    strcpy(message, "OK");
    return 1;
}
const char* getRankString(int rank) {
    switch (rank) {
        case 1: return "A";
        case 10: return "T";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: {
            static char buf[3];  // nok til "2" til "9"
            //hvad er buf?? ik kig markus.
            snprintf(buf, sizeof(buf), "%d", rank);
            return buf;
        }
    }
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
        const char *rankStr = getRankString(card.rank);
        fprintf(file, "%s%c\n", rankStr, card.suit);
        node = node->next;
    }

    fclose(file);
    sprintf(message, "Deck gemt til %s", filename);
}
