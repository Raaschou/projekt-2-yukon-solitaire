//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Board.h"

#include <stdlib.h>

void freeList(LinkedList *list) {
    CardNode *current = list->head;
    while (current != NULL) {
        CardNode *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void freeBoard(Board *board) {
    freeList(&board->deck);
    for (int i = 0; i < 7; i++) {
        freeList(&board->columns[i]);
    }
    for (int i = 0; i < 4; i++) {
        freeList(&board->foundations[i]);
    }
}
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
            int f = row / 2;
            if (board->foundations[f].head== NULL) {
                printf("[] F%d",f+1);
            }else {
                CardNode *currentFound = board->foundations[f].tail;
                printCard(currentFound, &board->foundations[f]);
                printf("F%d",f +1);
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

void initBoard(Board *board) {
    for (int i = 0; i < 7; i++) {
        list(&board->columns[i]);
    }
    for (int i = 0; i < 4; i++) {
        list(&board->foundations[i]);
    }
    list(&board->deck);
}

void printBoardStartUpPhase(Board *board, const char *lastCommand, const char *message) {
    printf("\nC1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    CardNode *current = board->deck.head;
    int i = 0;
    int row = 0;
if (board->deck.size!=0) {
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
}else {
    while (row < 8) {
        printf("\t\t\t\t\t\t");
        printf("\t\t");
        if (row < 8 && row % 2 == 0) {
            int f = row / 2;
            if (board->foundations[f].head == NULL) {
                printf("[] F%d", f + 1);
            } else {
                CardNode *fCard = board->foundations[f].tail;
                printCard(fCard, &board->foundations[f]);
                printf("F%d",f +1);
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


void initStack(BoardStack* stack) {
    stack->top = 0;
}

int isEmpty(BoardStack* stack) {
    return stack->top == 0;
}

int isFull(BoardStack* stack) {
    return stack->top >= MAX_HISTORY;
}

void push(BoardStack* stack, const Board* board) {
    if (isFull(stack)) {
        printf("Stack overflow! Kan ikke gemme flere træk.\n");
        return;
    }

    initBoard(&stack->boards[stack->top]);
    freeBoard(&stack->boards[stack->top]);
    copyBoard(&stack->boards[stack->top], board);
    stack->top++;
}

Board pop(BoardStack* stack) {
    if (isEmpty(stack)) {
        printf("Stack underflow! Ingen flere træk at fortryde.\n");
        Board empty = {0};
        return empty;
    }

    stack->top--;
    Board result = {0};
    copyBoard(&result, &stack->boards[stack->top]);

    // Frigør board i stack'en, da det nu er ude af stack'en
    freeBoard(&stack->boards[stack->top]);

    return result;
}

void initList(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void copyList(LinkedList *dest, const LinkedList *src) {
    initList(dest);

    CardNode *current = src->head;
    while (current != NULL) {
        Card copy = current->card; // shallow copy af Card er OK
        addCard(dest, copy);       // vi allokerer nyt node
        current = current->next;
    }
}

void copyBoard(Board *dest, const Board *src) {
    copyList(&dest->deck, &src->deck);
    for (int i = 0; i < 7; i++) {
        copyList(&dest->columns[i], &src->columns[i]);
    }
    for (int i = 0; i < 4; i++) {
        copyList(&dest->foundations[i], &src->foundations[i]);
    }
}
void clearStack(BoardStack* stack) {
    for (int i = 0; i < stack->top; i++) {
        freeBoard(&stack->boards[i]);
    }
    stack->top = 0;
}
void changeBoardStack(BoardStack *undo, BoardStack *redo, Board *board) {
    push(undo, board);
    clearStack(redo);
}
// void initBoard(Board *board) {
//     initList(&board->deck);
//     for (int i = 0; i < 7; i++) {
//         initList(&board->columns[i]);
//     }
//     for (int i = 0; i < 4; i++) {
//         initList(&board->foundations[i]);
//     }
// }
