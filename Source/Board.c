//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/Board.h"
#include "../Include/LinkedLists.h"
#include <stdlib.h>

/**
 * Udskriver board til PLAY fasen med kolonner, foundations og statusfelt.
 *
 * @param board Pointer til spilbrættet.
 * @param lastCommand Sidste indtastede kommando.
 * @param message Statusbesked.
 */
void printBoardPlayPhase(Board *board, const char *lastCommand, const char *message) {
    //Vi sætter en minimumshøjde på boardet på 7 pga. foundations
    int maxHeight = 7;
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
            if (board->foundations[f].head == NULL) {
                printf("[] F%d", f + 1);
            } else {
                CardNode *currentFound = board->foundations[f].tail;
                printCard(currentFound, &board->foundations[f]);
                printf("F%d", f + 1);
            }
        }
        printf("\n");
    }

    printf("\nLast Command: %s\n", lastCommand ? lastCommand : "");
    printf("Message: %s\n", message ? message : "");
    printf("INPUT >  ");
}

/**
 * Udskriver boardet i STARTUP fasen med kort fra deck og foundations.
 *
* @param board Pointer til spilbrættet.
 * @param lastCommand Sidste indtastede kommando.
 * @param message Statusbesked.
 */
void printBoardStartUpPhase(Board *board, const char *lastCommand, const char *message) {
    printf("\nC1\tC2\tC3\tC4\tC5\tC6\tC7\n\n");

    CardNode *current = board->deck.head;
    int i = 0;
    int row = 0;
    if (board->deck.size != 0) {
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
    } else {
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
                    printf("F%d", f + 1);
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

/**
 * Frigiver hukommelse brugt af et board:
 * deck, kolonner og foundations.
 *
 * @param board Pointer til det board der skal tømmes.
 */
void freeBoard(Board *board) {
    freeList(&board->deck);
    for (int i = 0; i < 7; i++) {
        freeList(&board->columns[i]);
    }
    for (int i = 0; i < 4; i++) {
        freeList(&board->foundations[i]);
    }
}

/**
 * Initialiserer alle lister i boardet: kolonner, foundations og deck.
 *
 * @param board Pointer til det board der skal initialiseres.
 */
void initBoard(Board *board) {
    for (int i = 0; i < 7; i++) {
        initList(&board->columns[i]);
    }
    for (int i = 0; i < 4; i++) {
        initList(&board->foundations[i]);
    }
    initList(&board->deck);
}

/**
 * Kopierer hele spilbrættet inkl. deck, kolonner og foundations.
 *
 * @param dest Pointer til det board der skal kopieres til.
 * @param src Pointer til det board der kopieres fra.
 */
void copyBoard(Board *dest, const Board *src) {
    copyList(&dest->deck, &src->deck);
    for (int i = 0; i < 7; i++) {
        copyList(&dest->columns[i], &src->columns[i]);
    }
    for (int i = 0; i < 4; i++) {
        copyList(&dest->foundations[i], &src->foundations[i]);
    }
}

/**
 * Initialiserer en tom board stak ved at nulstille toppen.
 *
 * @param stack Pointer til stakken der skal initialiseres.
 */
void initStack(BoardStack *stack) {
    stack->top = 0;
}

/**
 * Tjekker om en board stak er tom.
 *
 * @param stack Pointer til stakken der skal tjekkes.
 * @return 1 hvis stakken er tom, ellers 0.
 */
int isEmpty(BoardStack *stack) {
    return stack->top == 0;
}

/**
 * Tjekker om en board-stak er fuld.
 *
 * @param stack Pointer til stakken der skal tjekkes.
 * @return 1 hvis stakken er fuld, ellers 0.
 */
int isFull(BoardStack *stack) {
    return stack->top >= MAX_HISTORY;
}

/**
 * Tilføjer en kopi af boardet til toppen af stakken.
 * Ignorerer handlingen hvis stakken er fuld.
 *
 * @param stack Pointer til stakken.
 * @param board Pointer til det board der skal kopieres og gemmes.
 */
void push(BoardStack *stack, const Board *board) {
    if (isFull(stack)) {
        printf("Stack overflow! Kan ikke gemme flere træk.\n");
        return;
    }

    initBoard(&stack->boards[stack->top]);
    freeBoard(&stack->boards[stack->top]);
    copyBoard(&stack->boards[stack->top], board);
    stack->top++;
}

/**
 * Fjerner og returnerer det øverste board fra stakken.
 * Hvis stakken er tom, returneres et tomt board.
 *
 * @param stack Pointer til stakken.
 * @return En kopi af det board der blev taget fra toppen.
 */
Board pop(BoardStack *stack) {
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

/**
 * Rydder hele stakken og frigiver hukommelse for hvert board.
 *
 * @param stack Pointer til stakken der skal tømmes.
 */
void clearStack(BoardStack *stack) {
    for (int i = 0; i < stack->top; i++) {
        freeBoard(&stack->boards[i]);
    }
    stack->top = 0;
}

/**
 * Gemmer nuværende board i undo-stakken og rydder redo-stakken.
 *
 * @param undo Pointer til undo-stakken.
 * @param redo Pointer til redo-stakken.
 * @param board Pointer til det aktuelle board.
 */
void changeBoardStack(BoardStack *undo, BoardStack *redo, Board *board) {
    push(undo, board);
    clearStack(redo);
}

