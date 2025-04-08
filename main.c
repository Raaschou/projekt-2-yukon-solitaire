#include <stdio.h>
#include <stdlib.h>
#define TOTAL_CARDS 52;


typedef struct cardObject Card;
//Linked list structure for cards
struct cardObject {
    char suit;
    int rank;
    Card *next;
};


void printBoard(Card *deck);
Card createCards(char suit, int rank);
Card *createDeck(char suit[], int rank[], Card *deck[]);
void createLinkedLinkedList(Card *deck);

int main(void) {
    //This might be better done elsewhere or maybe as a char to better
    // represent suits, jacks, queens and kings?

    // 1=C, 2=D, 3=H, 4=S
    char suit[] = {'C','D','H','S'};
    int rank[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

    Card *deck[52];
    //Not working, need to understand pointers for linked lists
    *deck = createDeck(suit, rank, deck);

    printBoard(*deck);
    for (int i = 0; i < 52; i++) {
        free(deck[i]);
    }
    return 0;
}


//Early version of a function to print the board
// When variable height is implemented, the standard should be 10,
// and increasing to the max height of a column
void printBoard(Card *deck) {
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
                printf("%c%d     ", deck->suit, deck->rank);
            } else {
                printf("%c%d", deck->suit, deck->rank);
            }

        }
        printf("\n");
    }
}
//Doesn't do anything other than printing all cards
//Maybe better to create deck directly in createCards?
//Probably just need to get pointers and addresses right
Card *createDeck(char suit[], int rank[], Card *deck[]) {
    int suits = 4;
    int ranks = 13;
    int k = 0;


    for (int i = 0; i < suits; i++) {
        for (int j = 0; j < ranks; j++) {
           *deck[k] = createCards(suit[i], rank[j]);
            k++;
        }
    }
    printf("Returning deck\n");
    return deck;
}

Card createCards(char suit, int rank) {
    Card *newCard = (Card *)malloc(sizeof(Card));
    newCard->suit = suit;
    newCard->rank = rank;
    return *newCard;
}

//Should create linked list with size of the input part of the deck
void createLinkedList(Card *deck) {
    Card *head = &deck[0];

    //Alternative to sizeof?
    for (int i = 1; i < sizeof(*deck); i++) {
        deck->next = &deck[i];
    }
}