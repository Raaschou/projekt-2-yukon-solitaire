//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Board.h"




#include "../Include/Board.h"

//Bør printe boardet
// håber den kan bruges til alt
void printBoardPlayPhase(Board *board, const char *lastCommand, const char *message) {
    int maxHeight = 7;
    // den her funktion tjekker højden på alle vores columns
    // pga foundations skal den minimum være 7
    for (int i = 0; i < 7; i++) {
        if (board->columns[i].size > maxHeight)
            maxHeight = board->columns[i].size;
    }

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    for (int row = 0; row < maxHeight; row++) {
        for (int col = 0; col < 7; col++) {
            CardNode *current = board->columns[col].head;
            int index = 0;
            while (current && index < row) {
                current = current->next;
                index++;
            }
            printCard(current, &board->columns[col]);
        }

        printf("\t\t");
        // printer de 4 foundations
        if (row < 8 && row % 2 == 0) {
            if (board->foundations[row/2].head== NULL) {
                printf("[] F%d",(row+2)/2);
            }else {
                CardNode *currentFound = board->foundations[row].tail;
                printCard(currentFound, &board->foundations[row]);
            }
        }
        printf("\n");
    }

    // Print statusfelt
    // tjekker om der er parameter ellers tom
    printf("\nLast Command: %s\n", lastCommand ? lastCommand : "");
    printf("Message: %s\n", message ? message : "");
    printf("INPUT >  ");
}
void printBoardStartUpPhase(Board *board, const char *lastCommand, const char *message) {
    printf("Deck:\n\n");
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");
    CardNode *current = board->deck.head;
    int i = 0;
    while (current) {
        printf("[ ]\t"); // alle kort er skjulte i startup
        current = current->next;
        i++;
        if (i % 7 == 0) printf("\n");
    }

    printf("\n\nLast Command: %s\n", lastCommand ? lastCommand : "");
    printf("Message: %s\n", message ? message : "");
    printf("INPUT >  ");
}