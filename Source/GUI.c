#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "../Include/Board.h"
#include "../Include/Card.h"
#include "../Include/Game.h"
#include "../Include/Foundations.h"


#define NUM_STARTUP_BUTTONS 5 // ændre til 6 når load current game virker
#define NUM_PLAY_BUTTONS 3 //- ændre til 4 når save current phase virker

typedef struct {
    SDL_Rect rect;
    const char *label;
} Button;

typedef struct {
    SDL_Rect rect;
    CardNode *node;
} CardVisual;

Button startupButtons[NUM_STARTUP_BUTTONS] = {
    {{50, 550, 100, 40}, "Load"},
    {{160, 550, 100, 40}, "Shuffle R"},
    {{270, 550, 100, 40}, "Shuffle I"},
    {{380, 550, 100, 40}, "Show"},// man skal kunne give parameter for at den virker
    {{490, 550, 100, 40}, "Play"}
};

Button playButtons[NUM_PLAY_BUTTONS] = {
    {{50, 550, 100, 40}, "Quit"},
    {{160, 550, 100, 40}, "Undo"},
    {{270, 550, 100, 40}, "Redo"},

};

SDL_Texture *cardTextures[13][4];
SDL_Texture *backTexture = NULL;

CardNode *selectedCard = NULL;
int selectedCol = -1;

SDL_Texture *loadCardTexture(SDL_Renderer *renderer, const char *filename) {
    SDL_Surface *surf = SDL_LoadBMP(filename);
    if (!surf) {
        printf("Kunne ikke loade %s: %s\n", filename, SDL_GetError());
        return NULL;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return tex;
}

void loadAllCardTextures(SDL_Renderer *renderer) {
    const char *suits = "HDCS";
    const char *ranks = "A23456789TJQK";

    for (int r = 0; r < 13; r++) {
        for (int s = 0; s < 4; s++) {
            char filename[64];
            snprintf(filename, sizeof(filename), "Kort/bmp/%c%c.bmp", ranks[r], suits[s]);
            cardTextures[r][s] = loadCardTexture(renderer, filename);
        }
    }
    backTexture = loadCardTexture(renderer, "Kort/bmp/back.bmp");
}

void freeCardTextures() {
    for (int r = 0; r < 13; r++) {
        for (int s = 0; s < 4; s++) {
            if (cardTextures[r][s]) SDL_DestroyTexture(cardTextures[r][s]);
        }
    }
    if (backTexture) SDL_DestroyTexture(backTexture);
}

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
    if (!msgSurf) return;

    SDL_Texture *msgTex = SDL_CreateTextureFromSurface(renderer, msgSurf);
    if (!msgTex) {
        SDL_FreeSurface(msgSurf);
        return;
    }

    SDL_Rect msgRect = {10, 570, msgSurf->w, msgSurf->h};
    SDL_RenderCopy(renderer, msgTex, NULL, &msgRect);
    SDL_FreeSurface(msgSurf);
    SDL_DestroyTexture(msgTex);
}

void drawCard(SDL_Renderer *renderer, int x, int y, Card *card) {
    SDL_Rect dst = {x, y, 80, 120};

    if (!card->faceUp) {
        SDL_RenderCopy(renderer, backTexture, NULL, &dst);
        return;
    }

    int rankIndex = card->rank - 1;
    int suitIndex = (card->suit == 'H') ? 0 :
                    (card->suit == 'D') ? 1 :
                    (card->suit == 'C') ? 2 :
                    (card->suit == 'S') ? 3 : -1;

    SDL_Texture *tex = cardTextures[rankIndex][suitIndex];
    if (tex) {
        SDL_RenderCopy(renderer, tex, NULL, &dst);
    } else {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &dst);
    }
}

void drawBoardStartUpPhase(SDL_Renderer *renderer, Board *board, TTF_Font *font, const char *message) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Tegn foundation slots
    for (int i = 0; i < 4; i++) {
        SDL_Rect fslot = {800 , 20 + i * 130, 80, 120};
        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        SDL_RenderFillRect(renderer, &fslot);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &fslot);

        CardNode *fCard = board->foundations[i].tail;
        if (fCard) {
            drawCard(renderer, fslot.x, fslot.y, &fCard->card);
        }
    }

    // Tegn deck i kolonne-visning (op til 7 kort per række)
    CardNode *current = board->deck.head;
    int i = 0, row = 0;
    while (current) {
        int x = (i % 7) * 100 + 50;
        int y = row * 30 + 150;
        drawCard(renderer, x, y, &current->card);
        i++;
        current = current->next;
        if (i % 7 == 0) row++;
    }

    // Tegn knapper
    for (int i = 0; i < NUM_STARTUP_BUTTONS; i++) {
        drawButton(renderer, &startupButtons[i], font);
    }

    drawMessage(renderer, message, font);
}

void drawBoardPlayPhase(SDL_Renderer *renderer, Board *board, TTF_Font *font, const char *message) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Foundation slots + kort
    for (int i = 0; i < 4; i++) {
        SDL_Rect fslot = {800 , 20 + 130 * i, 80, 120};
        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        SDL_RenderFillRect(renderer, &fslot);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &fslot);

        CardNode *fCard = board->foundations[i].tail;
        if (fCard) {
            drawCard(renderer, fslot.x, fslot.y, &fCard->card);
        }
    }

    // Kolonner med kort
    int spacingX = 100;
    int spacingY = 30;
    for (int col = 0; col < 7; col++) {
        SDL_Rect slot = {20 + col * spacingX, 20, 80, 120};
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        SDL_RenderFillRect(renderer, &slot);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &slot);

        CardNode *node = board->columns[col].head;
        int i = 0;
        while (node != NULL) {
            int x = 20 + col * spacingX;
            int y = 100 + i * spacingY;
            drawCard(renderer, x, y, &node->card);
            if (selectedCard == node) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // grøn ramme
                SDL_Rect border = {x, y, 80, 120};
                SDL_RenderDrawRect(renderer, &border);
            }
            node = node->next;
            i++;
        }
    }

    // Knapper
    for (int i = 0; i < NUM_PLAY_BUTTONS; i++) {
        drawButton(renderer, &playButtons[i], font);
    }

    drawMessage(renderer, message, font);
}


void gameLoopGUI(Board *board) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    initBoard(board);

    SDL_Window *window = SDL_CreateWindow("Yukon Solitaire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("Kort/Font/ttf/DejaVuSans.ttf", 16);

    if (!font) {
        fprintf(stderr, "Font fejl: %s\n", TTF_GetError());
        exit(1);
    }

    loadAllCardTextures(renderer);

    GamePhase phase = STARTUP;
    char message[256] = "Velkommen til Yukon";
    char lastCommand[100] = "";
    SDL_Event e;
    int running = 1;

    const char *startupCmds[NUM_STARTUP_BUTTONS] = {"LD", "SR", "SI", "SW", "P"};
    const char *playCmds[NUM_PLAY_BUTTONS] = {"Q", "U", "R"};

    BoardStack undoStack, redoStack;
    initStack(&undoStack);
    initStack(&redoStack);
    CardVisual visibleCards[52];
    int numVisibleCards = 0;
    char selectedMessage[100] = "";


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
                    // Tjek knapper, herunder Undo og Redo
                    for (int i = 0; i < NUM_PLAY_BUTTONS; i++) {
                        SDL_Rect r = playButtons[i].rect;

                        if (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h) {
                                phase = playPhase(board, phase, playCmds[i], lastCommand, message, &undoStack, &redoStack);
                            }
                        }
                    }

                    // Tjek klik på kort
                    for (int col = 0; col < 7; col++) {
                        int cx = 50 + col * 100;
                        CardNode *node = board->columns[col].tail;
                        int row = board->columns[col].size - 1;
                        while (node) {
                            int cx = 50 + col * 100;
                            SDL_Rect cardRect = {cx, 100 + row * 30, 80, 120};

                            if (x >= cardRect.x && x <= cardRect.x + cardRect.w &&
                                y >= cardRect.y && y <= cardRect.y + cardRect.h) {

                                if (!node->card.faceUp) break;

                                if (selectedCard == NULL) {
                                    selectedCard = node;
                                    selectedCol = col;
                                    snprintf(message, 256, "Valgt kort: %d%c", node->card.rank, node->card.suit);
                                } else {
                                    if (col != selectedCol) {
                                        CardNode *target = board->columns[col].tail;

                                        // Debug-udskrifter
                                        printf("▶ Forsøger at flytte kort\n");
                                        printf("  - Valgt kort: %d%c\n", selectedCard->card.rank, selectedCard->card.suit);
                                        printf("  - Target column: %d\n", col);
                                        printf("  - Target column size: %d\n", board->columns[col].size);
                                        printf("  - Target eksisterer? %s\n", target ? "ja" : "nej");

                                        if ((target && validMoveC(selectedCard, target)) ||
                                            (!target && selectedCard->card.rank == 13)) {
                                            changeBoardStack(&undoStack, &redoStack, board);
                                            printf("✅ Trækket er gyldigt – udfører flytning\n");

                                            moveBetweenColumns(selectedCard, &board->columns[selectedCol], &board->columns[col]);
                                            flipLastCardIfAny(&board->columns[selectedCol]);
                                            snprintf(message, 256, "Flyttede kortet.");
                                            } else {
                                                printf("Ugyldigt træk\n");
                                                snprintf(message, 256, "Ugyldigt træk.");
                                            }
                                    } else {
                                        printf("Samme kolonne – ugyldigt træk\n");
                                        snprintf(message, 256, "Ugyldigt træk.");
                                    }

                                    selectedCard = NULL;
                                    selectedCol = -1;
                                }
                                break;
                                }

                            node = node->prev;
                            row--;
                        }
                        // Hvis ingen kort blev valgt og kolonnen er tom, tjek klik i tom slot
                        if (selectedCard && board->columns[col].size == 0) {
                            SDL_Rect emptySlot = {20 + col * 100, 20, 80, 120}; // samme som i drawBoardPlayPhase
                            printf("🟦 Tjekker klik i tom kolonne %d – klik (%d,%d)\n", col, x, y);
                            printf("Empty slot: x=%d to %d, y=%d to %d\n", emptySlot.x, emptySlot.x + emptySlot.w, emptySlot.y, emptySlot.y + emptySlot.h);

                            if (x >= emptySlot.x && x <= emptySlot.x + emptySlot.w &&
                                y >= emptySlot.y && y <= emptySlot.y + emptySlot.h) {
                                printf("✅ Klik indenfor tom kolonne %d registreret!\n", col);

                                if (selectedCard->card.rank == 13) {
                                    moveBetweenColumns(selectedCard, &board->columns[selectedCol], &board->columns[col]);
                                    flipLastCardIfAny(&board->columns[selectedCol]);
                                    snprintf(message, 256, "Flyttede konge til tom kolonne.");
                                } else {
                                    snprintf(message, 256, "Kun en konge må flyttes til en tom kolonne.");
                                }
                                selectedCard = NULL;
                                selectedCol = -1;
                                }
                        }

                    }
                    // Tjek klik på foundations
                    if (selectedCard != NULL) {
                        for (int f = 0; f < 4; f++) {
                            SDL_Rect fslot = {800, 20 + 130 * f, 80, 120};
                            printf("👆 Tjekker klik i foundation %d – klik (%d,%d)\n", f, x, y);
                            printf("Foundation slot: x=%d to %d, y=%d to %d\n", fslot.x, fslot.x + fslot.w, fslot.y, fslot.y + fslot.h);

                            if (x >= fslot.x && x <= fslot.x + fslot.w &&
                                y >= fslot.y && y <= fslot.y + fslot.h) {
                                printf("✅ Klik indenfor foundation %d registreret!\n", f);

                                if (validMoveF(selectedCard, board->foundations[f].tail)) {
                                    changeBoardStack(&undoStack, &redoStack, board);
                                    moveToFoundation(selectedCard, &board->columns[selectedCol], &board->foundations[f]);
                                    flipLastCardIfAny(&board->columns[selectedCol]);
                                    snprintf(message, 256, "Flyttede kort til foundation.");
                                } else {
                                    snprintf(message, 256, "Ugyldigt foundation-træk.");
                                }

                                selectedCard = NULL;
                                selectedCol = -1;
                                break;
                                }
                        }
                    }

                }
            }
        }

        if (phase == STARTUP) {
            drawBoardStartUpPhase(renderer, board, font, message);
        } else if (phase == PLAY) {
            drawBoardPlayPhase(renderer, board, font, message);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    freeCardTextures();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
