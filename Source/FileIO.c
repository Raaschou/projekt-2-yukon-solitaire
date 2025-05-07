//
// Created by August Frandsen on 15/04/2025.
//

#include "../Include/FileIO.h"
#include "../Include/Deck.h"
#include "../Include/LinkedLists.h"
#include "../Include/Board.h"

#include <stdio.h>
#include <string.h>



/**
 * Læser et kortdæk fra en fil og tilføjer kortene til en linked list.
 * Validerer indhold og sikrer præcis 52 unikke kort.
 *
 * @param filename Filnavn der skal læses fra.
 * @param deck Linked list hvor kortene gemmes.
 * @param message Outputbesked om succes eller fejl.
 * @return 1 hvis dæk blev læst korrekt, ellers 0.
 */
int readDeckFromFile(const char *filename, LinkedList *deck, char *message) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        sprintf(message, "ERROR: Kunne ikke åbne '%s'.", filename);
        return 0;
    }

    char seen[52] = {0};
    char line[8];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        Card card;
        if (!validateDeck(line, &card, lineNumber, seen, message)) {
            clearList(deck);
            fclose(file);
            return 0;
        }
        addCard(deck, card);
        deck->tail->card.faceUp = 0;
    }

    fclose(file);

    if (deck->size != 52) {
        sprintf(message, "ERROR: Forventede 52 kort, fandt %d.", deck->size);
        clearList(deck);
        return 0;
    }

    strcpy(message, "OK");
    return 1;
}

/**
 * Returnerer ranks som tekst. Fx 1 → "A", 11 → "J", 3 → "3".
 *
 * @param rank Kortets rang (1–13).
 * @return En streng der repræsenterer rang.
 */
const char* getRankString(int rank) {
    switch (rank) {
        case 1: return "A";
        case 10: return "T";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: {
            static char buf[3];  // nok til "2" til "9"
            //hvad er buf?? ik kig markus.
            snprintf(buf, sizeof(buf), "%d", rank);
            return buf;
        }
    }
}

/**
 * Skriver alle kort fra en linked list til en fil i tekstformat.
 *
 * @param deck Linked list med kort.
 * @param filename Navn på outputfil.
 * @param message Outputbesked om succes eller fejl.
 */
void writeDeckToFile(LinkedList *deck, const char *filename, char *message) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        sprintf(message, "Kunne ikke åbne filen: %s", filename);
        return;
    }

    CardNode *node = deck->head;
    while (node) {
        Card card = node->card;
        const char *rankStr = getRankString(card.rank);
        fprintf(file, "%s%c\n", rankStr, card.suit);
        node = node->next;
    }

    fclose(file);
    sprintf(message, "Deck gemt til %s", filename);
}

/**
 * Skriver en linked list til en binær fil.
 *
 * @param file Åbent filpointer i binær skrive-tilstand.
 * @param list Pointer til listen der skal skrives.
 */
void writeListBinary(FILE *file, LinkedList *list) {
    fwrite(&list->size, sizeof(int), 1, file);
    CardNode *curr = list->head;
    while (curr) {
        fwrite(&curr->card, sizeof(Card), 1, file);
        curr = curr->next;
    }
}
/**
 * Læser en linked list fra en binær fil.
 *
 * @param file Åbent filpointer i binær læse-tilstand.
 * @param list Pointer til listen der skal genskabes.
 */
void readListBinary(FILE *file, LinkedList *list) {
    int size;
    fread(&size, sizeof(int), 1, file);
    initList(list);
    for (int i = 0; i < size; i++) {
        Card c;
        fread(&c, sizeof(Card), 1, file);
        addCard(list, c);
    }
}

/**
 * Gemmer hele Board-strukturen til en binær fil.
 *
 * @param file Åbent filpointer.
 * @param board Pointer til Board der skal gemmes.
 */
void writeBoardBinary(FILE *file, Board *board) {
    writeListBinary(file, &board->deck);
    for (int i = 0; i < 7; i++) writeListBinary(file, &board->columns[i]);
    for (int i = 0; i < 4; i++) writeListBinary(file, &board->foundations[i]);
}

/**
 * Indlæser hele Board-strukturen fra en binær fil.
 *
 * @param file Åben filpointer.
 * @param board Pointer til Board der skal rekonstrueres.
 */
void readBoardBinary(FILE *file, Board *board) {
    readListBinary(file, &board->deck);
    for (int i = 0; i < 7; i++) readListBinary(file, &board->columns[i]);
    for (int i = 0; i < 4; i++) readListBinary(file, &board->foundations[i]);
}

/**
 * Skriver en hel BoardStack til fil (fx undo/redo).
 *
 * @param file Åben filpointer.
 * @param stack Pointer til stakken der skal skrives.
 */
void writeStackBinary(FILE *file, BoardStack *stack) {
    fwrite(&stack->top, sizeof(int), 1, file);
    for (int i = 0; i < stack->top; i++) {
        writeBoardBinary(file, &stack->boards[i]);
    }
}

/**
 * Læser en hel BoardStack fra fil (fx undo/redo).
 *
 * @param file Åbent filpointer.
 * @param stack Pointer til stakken der skal rekonstrueres.
 */
void readStackBinary(FILE *file, BoardStack *stack) {
    fread(&stack->top, sizeof(int), 1, file);
    for (int i = 0; i < stack->top; i++) {
        initBoard(&stack->boards[i]);
        readBoardBinary(file, &stack->boards[i]);
    }
}

/**
 * Gemmer hele spiltilstanden (Board, undo og redo) til én binær fil.
 *
 * @param filename Filnavnet der skal skrives til.
 * @param board Pointer til det aktuelle Board.
 * @param undo Pointer til undo-stakken.
 * @param redo Pointer til redo-stakken.
 * @param message Status eller fejlbesked.
 */
void saveGame(const char *filename, Board *board, BoardStack *undo, BoardStack *redo, char *message) {
    if (!filename) filename = "defaultSave.bin";

    FILE *file = fopen(filename, "wb"); // write binary
    if (!file) {
        sprintf(message,"Kunne ikke åbne %s til skrivning.", filename);
        return;
    }

    writeBoardBinary(file, board);
    writeStackBinary(file, undo);
    writeStackBinary(file, redo);

    fclose(file);
    sprintf(message, "Spil historik gemt til %s", filename);
}

/**
 * Indlæser hele spiltilstanden (Board, undo og redo) fra én binær fil.
 *
 * @param filename Filnavn der skal læses fra.
 * @param board Pointer til det Board der skal gendannes.
 * @param undo Pointer til undo-stakken.
 * @param redo Pointer til redo-stakken.
 * @param message Status eller fejlbesked.
 */
void loadGame(const char *filename, Board *board, BoardStack *undo, BoardStack *redo, char *message) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        sprintf(message, "Kunne ikke åbne %s til læsning.", filename);
        return;
    }
    readBoardBinary(file, board);
    readStackBinary(file, undo);
    readStackBinary(file, redo);
    fclose(file);
    sprintf(message,"Spil + historik indlæst fra %s\n", filename);
}