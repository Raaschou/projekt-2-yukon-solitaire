#include <stdio.h>
#include <stdlib.h>
#define TOTAL_CARDS 52



typedef struct cardObject Card; // header fil?
//Linked list structure for cards
struct cardObject {
    char suit;
    int rank;
    Card *next;
};

Card *head = NULL;
Card *current = NULL;

void printBoard(Card *deck[]);
Card *createCards(char suit, int rank);
Card *createDeck(char suit[], int rank[], Card *deck[]); // hvorfor * Card *deck[]
void createLinkedList(Card *deck[]);
void createLinkedlist(Card *lumns[],Card *deck);

int main(void) {
    //This might be better done elsewhere or maybe as a char to better
    // represent suits, jacks, queens and kings?

    // 1=C, 2=D, 3=H, 4=S
    char suit[] = {'C','D','H','S'};
    int rank[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

    Card *deck[52];
    //Not working, need to understand pointers for linked lists
    createDeck(suit, rank, deck);

    printBoard(deck);
    // STARTUP
    //Create columns of linked list
    int intialLength[] = {1,6,7,8,9,10,11};
    Card *lumns[7]= {NULL};
    void createLinkedlist(Card *lumns[],Card *deck)


    // *head = deck[0]
    // for (int i = 0; i < IL[k] ; i++){
    // deck[i]->next = deck[i+1]

    //
    //}


    // Linked list = [H6 -> D5 -> D8 -> H7 -> C9]
    createLinkedList(deck);
    printf("%c%d ",deck[30]->next->suit, deck[30]->next->rank);

    //Frees memory allocated in createCards, not sure why the malloc call says memory leaks
    //Manual testing shows it being freed
    for (int i = 0; i < 52; i++) {
        free(deck[i]);
    }
    return 0;
}


//Early version of a function to print the board
// When variable height is implemented, the standard should be 10,
// and increasing to the max height of a column
void printBoard(Card *deck[]) {
    int k = 0;
    for (int columnName = 1; columnName < 8; columnName++) {
        printf("x%d", columnName);

        if (columnName < 7) {
            //\t is the equivalent of tab, makes alignment easier compared to pure spaces.
            printf("\t");
        }
    }
    printf("\n");
    //Print all
    for (int row = 0; row < 9; row++) {
        // "&& k < 52" makes sure program exits as expected
        for (int col = 0; col < 7 && k < 52; col++) {
            if (col < 6) {
                printf("%c%d\t", deck[k]->suit, deck[k]->rank);
            } else {
                printf("%c%d", deck[k]->suit, deck[k]->rank);
            }
        k++;
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
           deck[k] = createCards(suit[i], rank[j]);
            k++;
        }
    }
    printf("Returning deck\n");
    return *deck;
}

Card *createCards(char suit, int rank) {
    Card *newCard = malloc(sizeof(Card));
    newCard->suit = suit;
    newCard->rank = rank;
    return newCard;
}

//Should create linked list with size of the input part of the deck
void createLinkedList(Card *deck[]) {
    head = deck[0];

    //Alternative to sizeof?
    for (int i = 1; i < 51; i++) {
        deck[i]->next = deck[i+1];
    }
}