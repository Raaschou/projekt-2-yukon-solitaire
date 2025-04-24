//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Board.h"




void printBoard(LinkedList columns[7], LinkedList foundations[4], const char *lastCommand, const char *message) {
    int maxHeight = 7;
    for (int i = 0; i < 7; i++) {
        if (columns[i].size > maxHeight)
            maxHeight = columns[i].size;
    }

    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    for (int row = 0; row < maxHeight; row++) {
        for (int col = 0; col < 7; col++) {
            CardNode *current = columns[col].head;
            int index = 0;

            while (current && index < row) {
                current = current->next;
                index++;
            }


            printCard(current, &columns[col]);

           // mangler foundations.
        }
        printf("\t\t");

        if (row < 8 && row % 2 == 0) {
            if (foundations[row].head== NULL) {
                printf("[] F%d",(row+2)/2);
            }else {
                CardNode *currentFound = foundations[row].tail;
                printCard(currentFound, &foundations[row]);
            }
        }
        printf("\n");
    }

    // Print statusfelt
    printf("\nLAST Command: %s\n", lastCommand ? lastCommand : "");
    printf("Message: %s\n", message ? message : "");
    printf("INPUT > ");
}