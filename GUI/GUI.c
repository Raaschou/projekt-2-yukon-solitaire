#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "../Include/Game.h"
#include "../Include/FileIO.h"
#include "GUI.h"

// === Konstanter ===
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define CARD_WIDTH 60
#define CARD_HEIGHT 90
#define FOUNDATION_X 800
#define FOUNDATION_Y 50
#define COLUMN_START_X 50
#define COLUMN_START_Y 50
#define COLUMN_SPACING 80
#define CARD_VERTICAL_OFFSET 25

// === Globale variabler ===
extern SDL_Renderer *renderer;
static TTF_Font *font = NULL;

SDL_Texture *cardTextures[14][4];  // rank 1–13, suit 0–3
SDL_Texture *cardBackTexture = NULL;

// === Hjælpefunktioner ===
int suitIndex(char suit) {
    switch (suit) {
        case 'H': return 0;
        case 'D': return 1;
        case 'C': return 2;
        case 'S': return 3;
        default: return -1;
    }
}

bool fileExists(const char *path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

// === Init og Shutdown ===
void initGUI() {
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF init fejlede: %s\n", SDL_GetError());
        exit(1);
    }

    const char *fontPath = "Kort/Font/ttf/DejaVuSans.ttf";
    if (!fileExists(fontPath)) {
        fprintf(stderr, "Fontfil ikke fundet: %s\n", fontPath);
        exit(1);
    }

    font = TTF_OpenFont(fontPath, 16);
    if (!font) {
        fprintf(stderr, "Fejl ved fontload: %s\n", SDL_GetError());
        exit(1);
    }
}

void shutdownGUI() {
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    TTF_Quit();
}

// === Grafik ===
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

    SDL_Surface *back = SDL_LoadBMP("Kort/bmp/back.bmp");
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

void drawButton(SDL_Renderer *renderer, float x, float y, float w, float h) {
    SDL_FRect rect = { x, y, w, h };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &rect);
}

void drawText(SDL_Renderer *renderer, const char *text, float x, float y) {
    if (!font  == 0) {
        printf("drawText: intet at tegne\n");
        return;
    }

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, strlen(text), color);
    if (!surface) {
        printf("TTF_RenderText_Solid fejlede: %s\n", SDL_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface fejlede: %s\n", SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect dst = { x, y, surface->w, surface->h };
    SDL_RenderTexture(renderer, texture, NULL, &dst);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

// === Tegn det hele ===
void drawBoard(SDL_Renderer *renderer, Board *board, GamePhase phase, const char *lastCommand, const char *message) {
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); // grøn baggrund
    SDL_RenderClear(renderer);

    // Kolonner (C1–C7)
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

    // Foundations (F1–F4)
    for (int f = 0; f < 4; f++) {
        LinkedList *foundation = &board->foundations[f];
        CardNode *top = foundation->tail;
        int x = FOUNDATION_X;
        int y = FOUNDATION_Y + f * (CARD_HEIGHT + 10);
        if (top)
            drawCard(renderer, x, y, top->card);
        else if (cardBackTexture)
            SDL_RenderTexture(renderer, cardBackTexture, NULL, &(SDL_FRect){x, y, CARD_WIDTH, CARD_HEIGHT});
    }
    if (!message || strlen(message) == 0) message = "(ingen besked)";
    if (!lastCommand || strlen(lastCommand) == 0) lastCommand = "(ingen kommando)";
    // === GUI knapper nederst ===
    drawButton(renderer, 20, 660, 150, 30);
    drawText(renderer, "LAST CMD MESSAGE:", 30, 665);

    drawButton(renderer, 20, 700, 150, 40);
    drawText(renderer, "Switch Phase", 30, 710);

    drawButton(renderer, 190, 700, 800, 40);


    SDL_RenderPresent(renderer);
}


