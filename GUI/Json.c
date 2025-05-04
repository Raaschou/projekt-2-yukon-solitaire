

#include "../Include/Board.h"
#include <stdio.h>
#include <string.h>
#include "../Include/FileIO.h"
#include "../GUI/Json.h"

void generateBoardJSON(Board *board, const char *message, char *out, size_t maxLen) {
    strcpy(out, "{\n  \"columns\": [\n");

    for (int col = 0; col < 7; col++) {
        strcat(out, "    [");

        CardNode *node = board->columns[col].head;
        while (node) {
            char cardStr[8];
            if (node->card.faceUp) {
                snprintf(cardStr, sizeof(cardStr), "\"%s%c\"",
                         getRankString(node->card.rank),
                         node->card.suit);
            } else {
                snprintf(cardStr, sizeof(cardStr), "\"[]\"");
            }

            strcat(out, cardStr);
            if (node->next) strcat(out, ", ");
            node = node->next;
        }

        strcat(out, "]");
        if (col < 6) strcat(out, ",\n");
        else strcat(out, "\n");
    }

    char msgPart[512];
    snprintf(msgPart, sizeof(msgPart), "  ],\n  \"message\": \"%s\"\n}\n", message);
    strcat(out, msgPart);
}