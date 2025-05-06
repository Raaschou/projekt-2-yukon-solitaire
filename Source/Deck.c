//
// Created by August Frandsen on 15/04/2025.
//
#include "../Include/Deck.h"
#include "../Include/Board.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Opretter et standard deck med 52 kort (C, D, H, S i suit og rank 1-13)
 * og tilføjer dem til den angivne linked list.
 *
 * @param deck Pointer til den liste hvor kortene gemmes.
 */
void startDeck(LinkedList *deck) {
    initList(deck);
    char suits[] = {'C', 'D', 'H', 'S'};

    for (int i = 0; i < 4; i++) {
        for (int j = 1; j <= 13; j++) {

            Card newCard;
            newCard.suit = suits[i];
            newCard.rank = j;
            newCard.faceUp = 0;

            addCard(deck, newCard);
        }
    }
}

/**
 * Kopierer alle kort fra en linked list over i et array.
 * Bruges senere af shuffle-funktioner.
 *
 * @param deck Pointer til linked list med kort.
 * @param card Array hvor kortene kopieres hen.
 */
void copyCardsFromList(LinkedList *deck, Card card[]) {
    CardNode *current = deck->head;
    int i = 0;
    while (current && i < deck->size) {
        card[i++] = current->card;
        current = current->next;
    }
}

/**
 * Rydder hele linked list og frigiver hukommelsen.
 *
 * @param deck Pointer til listen der skal tømmes.
 */
void clearList(LinkedList *deck) {
    CardNode *current = deck->head;
    while (current) {
        CardNode *next = current->next;
        free(current);
        current = next;
    }
    deck->head = NULL;
    deck->tail = NULL;
    deck->size = 0;
}

/**
 * Blander kortene i et deck tilfældigt med Fisher-Yates algoritmen,
 * som vi har valgt da den understøtter formålet.
 * Virker kun hvis decket indeholder præcis 52 kort.
 *
 * @param deck Pointer til det deck der skal blandes.
 */
void randomShuffle(LinkedList *deck) {
    srand(time(NULL));
    if (deck->size != 52) return;

    Card cards[52];
    copyCardsFromList(deck, cards);

    for (int i = 51; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }

    clearList(deck);

    for (int i = 0; i < 52; i++) {
        addCard(deck, cards[i]);
    }
}

/**
 * Blander kortene ved at splitte decket og blande fra venstre og højre.
 * Hvis cutSize er ugyldig, sker der ingenting.
 *
 * @param deck Pointer til det deck der skal blandes.
 * @param cutSize Antal kort fra venstre side af decket.
 * Skal være 0<cutSize<52. Hvis cutSize ikke er angivet, oprettes en tilfældigt (Kan ses i Game.c).
 */
void splitShuffle(LinkedList *deck, int cutSize) {
    if (cutSize <= 0 || cutSize >= deck->size) return;

    Card cards[52];
    copyCardsFromList(deck, cards);
    clearList(deck);

    int leftIndex = cutSize - 1;
    int rightIndex = 51;

    while (leftIndex >= 0 && rightIndex >= cutSize) {
        addCard(deck, cards[leftIndex--]);
        addCard(deck, cards[rightIndex--]);
    }

    while (leftIndex > 0) {
        addCard(deck, cards[leftIndex--]);
    }
    while (rightIndex >= cutSize ) {
        addCard(deck, cards[rightIndex--]);
    }
}

/**
 * Validerer en tekstlinje som et kort.
 * Tjekker for ugyldige ranks, kulører og eventuelt for ens kort.
 *
 * @param line Linjen der indeholder kortet (f.eks. "AS" for spar es).
 * @param outCard Pointer til hvor det validerede kort gemmes.
 * @param lineNum Linjenummer til fejlbeskeder.
 * @param seen Array til at holde styr på kort set indtil videre.
 * @param message Evt fejlbesked
 * @return 1 hvis gyldigt kort, 0 hvis ikke.
 */
int validateDeck(const char *line, Card *outCard, int lineNum, char seen[52], char *message) {
    if (strlen(line) < 2 ) {
       sprintf(message,"ERROR: Tom eller kort linje på linje %d.\n", lineNum);
        return 0;
    }

    char rank = toupper(line[0]);
    char suit = toupper(line[1]);

    // Ranks
    int r;
    switch (rank) {
        case 'A': r = 1; break;
        case 'T': r = 10; break;
        case 'J': r = 11; break;
        case 'Q': r = 12; break;
        case 'K': r = 13; break;
        case '2'...'9': r = rank - '0'; break;
        default:
            sprintf(message,"ERROR: Ugyldig rank '%c' på linje %d.\n", rank, lineNum);
        return 0;
    }

    // Suits
    int s;
    switch (suit) {
        case 'C': s = 0; break;
        case 'D': s = 1; break;
        case 'H': s = 2; break;
        case 'S': s = 3; break;
        default:
            sprintf(message,"ERROR: Ugyldig kulør '%c' på linje %d.\n", suit, lineNum);
        return 0;
    }

    int index = s * 13 + (r-1);
    if (seen[index]) {
        sprintf(message,"ERROR: Dubletkort '%c%c' på linje %d.\n", rank, suit, lineNum);
        return 0;
    }

    seen[index] = 1;
    outCard->rank = r;
    outCard->suit = suit;
    outCard->faceUp = 0;

    return 1;
}



