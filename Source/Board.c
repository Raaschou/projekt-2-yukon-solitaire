//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Board.h"




#include "../Include/Board.h"

//Bør printe boardet
// håber den kan bruges til alt
void printBoardStartUpPhase(Board *board, const char *lastCommand, const char *message) {
    printf("\nC1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    CardNode *current = board->deck.head;
    int i = 0;
    int row = 0;

    while (current) {
        printCard(current, &board->deck);
        current = current->next;
        i++;

        if (i % 7 == 0) {
            // Foundation visning hver anden linje (ligesom i playPhase)
            printf("\t\t");
            if (row < 8 && row % 2 == 0) {
                int f = row / 2;
                if (board->foundations[f].head == NULL) {
                    printf("[] F%d", f + 1);
                } else {
                    CardNode *fCard = board->foundations[f].tail;
                    printCard(fCard, &board->foundations[f]);
                }
            }
            printf("\n");
            row++;
        }
    }

    printf("\nLast Command: %s\n", lastCommand ? lastCommand : "");
    printf("Message: %s\n", message ? message : "");
    printf("INPUT >  ");
}