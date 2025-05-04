#include <SDL2/SDL.h>
#include <time.h>
#include <stdio.h>
#include "../Include/Board.h"
#include "../Include/Card.h"
#include "../Include/Game.h"

typedef struct {
    SDL_Rect rect;
    const char *label;
} Button;

#define NUM_STARTUP_BUTTONS 7

Button startupButtons[NUM_STARTUP_BUTTONS] = {
    {{50, 550, 100, 40}, "Load"},
    {{160, 550, 100, 40}, "Shuffle R"},
    {{270, 550, 100, 40}, "Shuffle I"},
    {{380, 550, 100, 40}, "Split"},
    {{490, 550, 100, 40}, "Sort"},
    {{600, 550, 100, 40}, "Show"},
    {{710, 550, 100, 40}, "Play"}
};

void drawButton(SDL_Renderer *renderer, Button *button) {
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
    SDL_RenderFillRect(renderer, &button->rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &button->rect);
}

void drawCard(SDL_Renderer *renderer, int x, int y, Card *card) {
    if (card->suit == 'S' || card->suit == 'C') {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    }

    SDL_Rect rect = {x, y, 80, 120};
    SDL_RenderFillRect(renderer, &rect);
}

void drawEmptyBoard(SDL_Renderer *renderer) {

    for (int col = 0; col < 7; col++) {
        SDL_Rect slot = { 50 + col * 100, 100, 80, 120 };
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        SDL_RenderFillRect(renderer, &slot);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &slot);
    }

    for (int i = 0; i < 4; i++) {
        SDL_Rect fslot = { 600 + i * 90, 20, 80, 120 };
        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        SDL_RenderFillRect(renderer, &fslot);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &fslot);
    }
}


void runGUI() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Yukon Solitaire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Board board = {0};
    GamePhase phase = STARTUP;

    SDL_Event e;
    int running = 1;

    srand((unsigned int)time(NULL));

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && phase == STARTUP) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                for (int i = 0; i < NUM_STARTUP_BUTTONS; i++) {
                    SDL_Rect r = startupButtons[i].rect;
                    if (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h) {
                        printf("Clicked: %s\n", startupButtons[i].label);
                        // Her kalder du fx:
                        // if (i == 0) loadDeck(&board.deck, "file.txt");
                        // if (i == 6) phase = PLAY;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        if (phase == STARTUP) {
            for (int i = 0; i < NUM_STARTUP_BUTTONS; i++) {
                drawButton(renderer, &startupButtons[i]);
                drawEmptyBoard(renderer);
            }
        }

        if (phase == PLAY) {
            int spacingX = 100;
            int spacingY = 30;

            for (int col = 0; col < 7; col++) {
                CardNode *node = board.columns[col].head;
                int i = 0;
                while (node != NULL) {
                    int x = col * spacingX + 50;
                    int y = i * spacingY + 50;
                    drawCard(renderer, x, y, &node->card);
                    node = node->next;
                    i++;
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
