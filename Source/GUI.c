#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "../Include/Board.h"
#include "../Include/Card.h"
#include "../Include/Game.h"

#define NUM_STARTUP_BUTTONS 7
#define NUM_PLAY_BUTTONS 5

typedef struct {
    SDL_Rect rect;
    const char *label;
} Button;

Button startupButtons[NUM_STARTUP_BUTTONS] = {
    {{50, 550, 100, 40}, "Load"},
    {{160, 550, 100, 40}, "Shuffle R"},
    {{270, 550, 100, 40}, "Shuffle I"},
    {{380, 550, 100, 40}, "Split"},
    {{490, 550, 100, 40}, "Sort"},
    {{600, 550, 100, 40}, "Show"},
    {{710, 550, 100, 40}, "Play"}
};

Button playButtons[NUM_PLAY_BUTTONS] = {
    {{50, 550, 150, 40}, "PlayPhase"},
    {{210, 550, 150, 40}, "Shuffle R"},
    {{370, 550, 150, 40}, "Shuffle S"},
    {{530, 550, 150, 40}, "ShowCards"},
    {{690, 550, 150, 40}, "LoadDeck"}
};

void drawButton(SDL_Renderer *renderer, Button *button, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
    SDL_RenderFillRect(renderer, &button->rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &button->rect);

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, button->label, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int texW = 0, texH = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &texW, &texH);

    SDL_Rect textRect = {
        button->rect.x + (button->rect.w - texW) / 2,
        button->rect.y + (button->rect.h - texH) / 2,
        texW,
        texH
    };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void drawMessage(SDL_Renderer *renderer, const char *message, TTF_Font *font) {
    if (!message || strlen(message) == 0) return;
    SDL_Color color = {0, 0, 0};
    SDL_Surface *msgSurf = TTF_RenderText_Solid(font, message, color);
    SDL_Texture *msgTex = SDL_CreateTextureFromSurface(renderer, msgSurf);
    SDL_Rect msgRect = {10, 570, msgSurf->w, msgSurf->h};
    SDL_RenderCopy(renderer, msgTex, NULL, &msgRect);
    SDL_FreeSurface(msgSurf);
    SDL_DestroyTexture(msgTex);
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
        SDL_Rect slot = {50 + col * 100, 100, 80, 120};
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        SDL_RenderFillRect(renderer, &slot);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &slot);
    }

    for (int i = 0; i < 4; i++) {
        SDL_Rect fslot = {600 + i * 90, 20, 80, 120};
        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        SDL_RenderFillRect(renderer, &fslot);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &fslot);
    }
}
void drawBoard(SDL_Renderer *renderer, Board *board, GamePhase phase, const char *message, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    drawEmptyBoard(renderer);

    if (phase == STARTUP) {
        for (int i = 0; i < NUM_STARTUP_BUTTONS; i++) {
            drawButton(renderer, &startupButtons[i], font);
        }
    } else if (phase == PLAY) {
        int spacingX = 100;
        int spacingY = 30;
        for (int col = 0; col < 7; col++) {
            CardNode *node = board->columns[col].head;
            int i = 0;
            while (node != NULL) {
                int x = col * spacingX + 50;
                int y = i * spacingY + 50;
                drawCard(renderer, x, y, &node->card);
                node = node->next;
                i++;
            }
        }
        for (int i = 0; i < NUM_PLAY_BUTTONS; i++) {
            drawButton(renderer, &playButtons[i], font);
        }
    }

    drawMessage(renderer, message, font);
    SDL_RenderPresent(renderer);
}



void gameLoopGUI(Board *board) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Yukon Solitaire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //TTF_Font *font = TTF_OpenFont("Kort/Font/DejaVuSans.ttf", 16);
    TTF_Font *font = TTF_OpenFont("Kort/Font/ttf/DejaVuSans.ttf", 16);
    if (!font) {
        fprintf(stderr, "❌ Kunne ikke åbne fonten: %s\n", TTF_GetError());
        exit(1);
    } else {
        printf("✅ Font indlæst korrekt!\n");
    }

    GamePhase phase = STARTUP;
    char message[256] = "";
    char lastCommand[100] = "";
    SDL_Event e;
    int running = 1;

    const char *startupCmds[NUM_STARTUP_BUTTONS] = {"LD", "SR", "SI", "SH", "SO", "SW", "P"};
    const char *playCmds[NUM_PLAY_BUTTONS] = {"", "SR", "SI", "SW", "LD"};

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (phase == STARTUP) {
                    for (int i = 0; i < NUM_STARTUP_BUTTONS; i++) {
                        SDL_Rect r = startupButtons[i].rect;
                        if (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h) {
                            phase = startupPhase(board, phase, startupCmds[i], lastCommand, message);
                        }
                    }
                } else if (phase == PLAY) {
                    for (int i = 1; i < NUM_PLAY_BUTTONS; i++) { // skip i==0 (label)
                        SDL_Rect r = playButtons[i].rect;
                        if (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h) {
                            phase = playPhase(board, phase, playCmds[i], lastCommand, message);
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawEmptyBoard(renderer);

        if (phase == STARTUP) {
            for (int i = 0; i < NUM_STARTUP_BUTTONS; i++) {
                drawButton(renderer, &startupButtons[i], font);
            }
        } else if (phase == PLAY) {
            int spacingX = 100;
            int spacingY = 30;
            for (int col = 0; col < 7; col++) {
                CardNode *node = board->columns[col].head;
                int i = 0;
                while (node != NULL) {
                    int x = col * spacingX + 50;
                    int y = i * spacingY + 50;
                    drawCard(renderer, x, y, &node->card);
                    node = node->next;
                    i++;
                }
            }
            for (int i = 0; i < NUM_PLAY_BUTTONS; i++) {
                drawButton(renderer, &playButtons[i], font);
            }
        }

        drawMessage(renderer, message, font);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
