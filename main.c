#include <stdio.h>
#define TOTAL_CARDS 52;


typedef struct card Card;
struct cardObject {
    char suit;
    int rank;
    Card *next;
};

void constructDeck(char suit[], int rank[]);
void printBoard();


int main(void) {
    //This might be better done elsewhere or maybe as a char to better
    // represent suits, jacks, queens and kings?

    // 1=C, 2=D, 3=H, 4=S
    char suit[] = {'C','D','H','S'};

    int rank[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

    constructDeck(suit, rank);

    printBoard();
    return 0;
}


//Early version of a function to print the board
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

void constructDeck(char suit[], int rank[]) {
    int suits = 4;
    int ranks = 13;

    for (int i = 0; i < suits; i++) {
        for (int j = 0; j < ranks; j++) {
            printf("%c%d    ", suit[i], rank[j]);
        }
        printf("\n");
    }
    printf("\n");
}