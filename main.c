#include <stdio.h>

void printBoard();

int main(void) {


    printBoard();
    return 0;
}

//Early version of a method to print the board
// When variable height is implemented, the standard should be 10,
// and increasing to the max height of a column
void printBoard() {
    for (int columnName = 1; columnName < 8; columnName++) {
        printf("C%d", columnName);
        if (columnName < 7) {
            // 5 spaces
            printf("     ");
        }
    }
    printf("\n");

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 7; col++) {
            if (col < 6) {
                printf("[]     ");
            } else {
                printf("[]");
            }

        }
        printf("\n");
    }
}