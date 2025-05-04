// gui.c - SDL3 Yukon GUI med texture-cache til kortbilleder
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../Include/Game.h"
#include "../Include/FileIO.h"
#include "GUI.h"


#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define CARD_WIDTH 60
#define CARD_HEIGHT 90
#define CARD_SPACING 20
#define FOUNDATION_X 800
#define FOUNDATION_Y 50
#define COLUMN_START_X 50
#define COLUMN_START_Y 50
#define COLUMN_SPACING 80
#define CARD_VERTICAL_OFFSET 25

extern SDL_Renderer *renderer;

// Kort-cache
SDL_Texture *cardTextures[14][4]; // 1–13, suit 0–3
SDL_Texture *cardBackTexture = NULL;

int suitIndex(char suit) {
    switch (suit) {
        case 'H': return 0;
        case 'D': return 1;
        case 'C': return 2;
        case 'S': return 3;
        default: return -1;
    }
}


// Indlæs alle kortbilleder én gang
void loadCardPictures(SDL_Renderer *renderer) {
    char suits[] = { 'H', 'D', 'C', 'S' };

    for (int rank = 1; rank <= 13; rank++) {
        for (int s = 0; s < 4; s++) {
            char filename[64];
            snprintf(filename, sizeof(filename), "Kort/bmp/%s%c.bmp", getRankString(rank), suits[s]);
            SDL_Surface *surface = SDL_LoadBMP(filename);
            if (!surface) {
                printf("Mangler billede: %s\n", filename);
                continue;
            }
            cardTextures[rank][s] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
        }
    }
    // Load back
    SDL_Surface *back = SDL_LoadBMP("assets/back.bmp");
    if (back) {
        cardBackTexture = SDL_CreateTextureFromSurface(renderer, back);
        SDL_DestroySurface(back);
    }
}

void drawCard(SDL_Renderer *renderer, int x, int y, Card card) {
    SDL_FRect dst = { x, y, CARD_WIDTH, CARD_HEIGHT };

    if (!card.faceUp || card.suit == 0) {
        if (cardBackTexture) SDL_RenderTexture(renderer, cardBackTexture, NULL, &dst);
        return;
    }

    int s = suitIndex(card.suit);
    if (s == -1 || !cardTextures[card.rank][s]) return;
    SDL_RenderTexture(renderer, cardTextures[card.rank][s], NULL, &dst);
}

// Tegn board
void drawBoard(SDL_Renderer *renderer, Board *board, GamePhase phase, const char *lastCommand, const char *message) {
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    SDL_RenderClear(renderer);

    for (int col = 0; col < 7; col++) {
        LinkedList *column = &board->columns[col];
        CardNode *node = column->head;
        int x = COLUMN_START_X + col * COLUMN_SPACING;
        int y = COLUMN_START_Y;

        while (node) {
            drawCard(renderer, x, y, node->card);
            y += CARD_VERTICAL_OFFSET;
            node = node->next;
        }
    }

    for (int f = 0; f < 4; f++) {
        LinkedList *foundation = &board->foundations[f];
        CardNode *top = foundation->tail;
        int x = FOUNDATION_X;
        int y = FOUNDATION_Y + f * (CARD_HEIGHT + 10);
        if (top) drawCard(renderer, x, y, top->card);
        else if (cardBackTexture) SDL_RenderTexture(renderer, cardBackTexture, NULL, &(SDL_FRect){x,y,CARD_WIDTH,CARD_HEIGHT});
    }

    // TODO: SDL_ttf til at tegne lastCommand og message
    SDL_RenderPresent(renderer);
}