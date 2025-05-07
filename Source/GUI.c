#include <SDL.h>
#include <SDL_ttf.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "../Include/Board.h"
#include "../Include/Card.h"
#include "../Include/Game.h"
#include "../Include/Foundations.h"
#include "../Include/GUI.h"




char commandArg[64] = "";
int typing = 0;

Button startupButtons[NUM_STARTUP_BUTTONS] = {
    {{20, 550, 100, 40}, "Load"},
    {{130, 550, 100, 40}, "Save"},
    {{240, 550, 100, 40}, "Shuffle I"},
    {{350, 550, 100, 40}, "Shuffle R"},
    {{460, 550, 100, 40}, "Show"},
    {{570,550,100,40},"Play"},
{{680,550,100,40},"Load game"}


};

Button playButtons[NUM_PLAY_BUTTONS] = {
    {{20, 550, 100, 40}, "Quit"},
    {{130, 550, 100, 40}, "Undo"},
    {{240, 550, 100, 40}, "Redo"},
    {{350,550,100,40},"Save game"}

};
SDL_Texture *cardTextures[13][4];
SDL_Texture *backTexture = NULL;
CardNode *selectedCard = NULL;
int selectedCol = -1;

/**
 * Loader en BMP-billedfil og konverterer den til en SDL_Texture.
 *
 * @param renderer SDL renderer der bruges til at oprette teksturen.
 * @param filename Filsti til BMP-billedet.
 * @return Pointer til oprettet SDL_Texture, ellers NULL ved fejl.
 */
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

/**
 * Tegner tekst på skærmen ved angivne koordinater.
 *
 * @param renderer SDL renderer.
 * @param font Font til teksttegning.
 * @param text Teksten der skal tegnes.
 * @param x X-koordinat for placering.
 * @param y Y-koordinat for placering.
 */
void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
    SDL_Color color = {0, 0, 0, 255};  // sort tekst
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

/**
 * Loader alle kortbilleder og bagside til kort fra disk.
 *
 * @param renderer SDL renderer der bruges til at oprette teksturerne.
 */
void loadAllCardTextures(SDL_Renderer *renderer) {
    const char *suits = "HDCS";
    const char *ranks = "A23456789TJQK";

    for (int r = 0; r < 13; r++) {
        for (int s = 0; s < 4; s++) {
            char filename[64];
            snprintf(filename, sizeof(filename), "../Kort/bmp/%c%c.bmp", ranks[r], suits[s]);
            cardTextures[r][s] = loadCardTexture(renderer, filename);
        }
    }
    backTexture = loadCardTexture(renderer, "../Kort/bmp/back.bmp");
}

/**
 * Frigiver hukommelse for alle kortteksturer, inkl. bagside.
 */
void freeCardTextures() {
    for (int r = 0; r < 13; r++) {
        for (int s = 0; s < 4; s++) {
            if (cardTextures[r][s]) SDL_DestroyTexture(cardTextures[r][s]);
        }
    }
    if (backTexture) SDL_DestroyTexture(backTexture);
}

/**
 * Tegner en enkelt knap med label.
 *
 * @param renderer SDL renderer.
 * @param button Pointer til knapstruktur med position og tekst.
 * @param font TTF-font til teksttegning.
 */
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

/**
 * Tegner en statusbesked nederst i vinduet.
 *
 * @param renderer SDL renderer.
 * @param message Teksten der skal vises.
 * @param font Font der bruges til tekst.
 * @param windowHeight Højden på vinduet, bruges til placering.
 * @param buttonHeight Højden på knapperne, bruges til at undgå overlap.
 */
void drawMessage(SDL_Renderer *renderer, const char *message, TTF_Font *font, int windowHeight, int buttonHeight) {
    if (!message || strlen(message) == 0) return;

    SDL_Color color = {0, 0, 0};
    SDL_Surface *msgSurf = TTF_RenderText_Solid(font, message, color);
    if (!msgSurf) return;

    SDL_Texture *msgTex = SDL_CreateTextureFromSurface(renderer, msgSurf);
    if (!msgTex) {
        SDL_FreeSurface(msgSurf);
        return;
    }

    int msgY = windowHeight - 20 - buttonHeight - 10 - msgSurf->h;
    SDL_Rect msgRect = {10, msgY, msgSurf->w, msgSurf->h};

    SDL_RenderCopy(renderer, msgTex, NULL, &msgRect);
    SDL_FreeSurface(msgSurf);
    SDL_DestroyTexture(msgTex);
}
/**
 * Tegner et enkelt kort (face up eller down) på en position.
 *
 * @param renderer SDL renderer.
 * @param x X-koordinat.
 * @param y Y-koordinat.
 * @param card Pointer til kortstruktur der skal tegnes.
 */
void drawCard(SDL_Renderer *renderer, int x, int y, Card *card) {
    SDL_Rect dst = {x, y, 80, 120};

    if (!card->faceUp) {
        SDL_RenderCopy(renderer, backTexture, NULL, &dst);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &dst);
        return;
    }

    int rankIndex = card->rank - 1;
    int suitIndex = (card->suit == 'H')
                        ? 0
                        : (card->suit == 'D')
                              ? 1
                              : (card->suit == 'C')
                                    ? 2
                                    : (card->suit == 'S')
                                          ? 3
                                          : -1;

    SDL_Texture *tex = cardTextures[rankIndex][suitIndex];
    if (tex) {
        SDL_RenderCopy(renderer, tex, NULL, &dst);
    } else {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &dst);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &dst);
}
/**
 * Tegner GUI'en i STARTUP-fasen, før spillet starter.
 *
 * @param renderer SDL renderer.
 * @param window SDL vinduet, bruges til layout.
 * @param board Spillets aktuelle tilstand.
 * @param font Font til tekst.
 * @param message Statusbesked der vises nederst.
 */
void drawBoardStartUpPhase(SDL_Renderer *renderer, SDL_Window *window, Board *board, TTF_Font *font,
                           const char *message) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Tegn foundation slots
    for (int i = 0; i < 4; i++) {
        SDL_Rect fslot = {800, 20 + i * 130, 80, 120};
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
        int x = (i % 7) * 100 + 20;
        int y = row * 30 + 20;
        drawCard(renderer, x, y, &current->card);
        i++;
        current = current->next;
        if (i % 7 == 0) row++;
    }

    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);
    int buttonHeight = 40; // hvis det er fast
    int buttonY = winH - 20 - buttonHeight;

    // Tegn knapper
    for (int i = 0; i < NUM_STARTUP_BUTTONS; i++) {
        startupButtons[i].rect.y = buttonY;
        drawButton(renderer, &startupButtons[i], font);
    }

    drawMessage(renderer, message, font, winH, buttonHeight);


    int inputX = winW  - 220;
    int inputY = winH - buttonHeight - 60;
    SDL_Rect inputBox = {inputX, inputY, 200, 30};
    if (typing) {
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);  // rød
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);  // grå
    }
    SDL_RenderFillRect(renderer, &inputBox);
    SDL_RenderDrawRect(renderer, &inputBox);
    drawText(renderer, font, commandArg, inputBox.x + 5, inputBox.y + 5);

}

/**
 * Tegner spilbrættet i PLAY-fasen, inkl. kort og knapper.
 *
 * @param renderer SDL renderer.
 * @param window SDL vinduet, bruges til layout.
 * @param board Spillets aktuelle tilstand.
 * @param font Font der bruges til knaptekst m.m.
 * @param message Status- eller fejlbesked der vises nederst.
 */
void drawBoardPlayPhase(SDL_Renderer *renderer, SDL_Window *window, Board *board, TTF_Font *font, const char *message) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Foundation slots + kort
    for (int i = 0; i < 4; i++) {
        SDL_Rect fslot = {800, 20 + 130 * i, 80, 120};
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
            int y = 20 + i * spacingY;
            drawCard(renderer, x, y, &node->card);
            if (selectedCard == node) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // rød ramme ved klik
                SDL_Rect border = {x, y, 80, 120};
                SDL_RenderDrawRect(renderer, &border);
            }
            node = node->next;
            i++;
        }
    }
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);
    int buttonHeight = 40; // hvis det er fast
    int buttonY = winH - 20 - buttonHeight;

    for (int i = 0; i < NUM_PLAY_BUTTONS; i++) {
        playButtons[i].rect.y = buttonY;
        drawButton(renderer, &playButtons[i], font);
    }

    drawMessage(renderer, message, font, winH, buttonHeight);


    int inputX = winW  - 220;
    int inputY = winH - buttonHeight - 60;
    SDL_Rect inputBox = {inputX, inputY, 200, 30};
    if (typing) {
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);  // rød
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);  // grå
    }
    SDL_RenderFillRect(renderer, &inputBox);
    SDL_RenderDrawRect(renderer, &inputBox);
    drawText(renderer, font, commandArg, inputBox.x + 5, inputBox.y + 5);
}

/**
 * Kører GUI-versionen af spillet.
 *
 * @param board Pointer til den aktuelle spiltilstand.
 */
void gameLoopGUI(Board *board) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    initBoard(board);
    SDL_StartTextInput();

    SDL_Window *window = SDL_CreateWindow("Yukon Solitaire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 750,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    SDL_SetWindowMinimumSize(window, 900, 750); // bredde, højde
    SDL_SetWindowMaximumSize(window, 900, 1000);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("../Kort/Font/ttf/DejaVuSans.ttf", 16);

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

    const char *startupCmds[NUM_STARTUP_BUTTONS] = {"LD","SD","SI","SR", "SW", "P","L"};
    const char *playCmds[NUM_PLAY_BUTTONS] = {"Q", "U", "R","S"};

    BoardStack undoStack, redoStack;
    initStack(&undoStack);
    initStack(&redoStack);
    CardVisual visibleCards[52];
    int numVisibleCards = 0;
    char selectedMessage[100] = "";


    while (running) {
        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_TEXTINPUT && typing) {
                if (strlen(commandArg) + strlen(e.text.text) < sizeof(commandArg) - 1) {
                    strcat(commandArg, e.text.text);
                }
            }

            if (e.type == SDL_KEYDOWN && typing) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(commandArg) > 0) {
                    commandArg[strlen(commandArg) - 1] = '\0';
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    typing = 0;
                    SDL_StopTextInput();
                }
            }
            if (e.type == SDL_QUIT) {
                running = 0;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                int winW, winH;
                SDL_GetWindowSize(window, &winW, &winH);
                int buttonHeight = 40;
                int buttonY = winH - 20 - buttonHeight;
                int buttonCount = (phase == STARTUP) ? NUM_STARTUP_BUTTONS : NUM_PLAY_BUTTONS;
                Button *btnArray = (phase == STARTUP) ? startupButtons : playButtons;
                SDL_Rect *lastBtn = &playButtons[NUM_PLAY_BUTTONS - 1].rect;
                int inputX = winW  - 220;
                int inputY = winH - buttonHeight - 60;
                SDL_Rect inputBox = {inputX, inputY, 200, 30};

                // Først: check input-boksen (uanset fase)
                if (x >= inputBox.x && x <= inputBox.x + inputBox.w &&
                    y >= inputBox.y && y <= inputBox.y + inputBox.h) {
                    typing = 1;
                    SDL_StartTextInput();
                    } else {
                        typing = 0;
                        SDL_StopTextInput();
                    }

                // Derefter: knap-klik
                if (phase == STARTUP) {
                    for (int i = 0; i < NUM_STARTUP_BUTTONS; i++) {
                        SDL_Rect r = startupButtons[i].rect;
                        if (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h) {
                            char combinedInput[128];
                            if (strlen(commandArg) > 0) {
                                snprintf(combinedInput, sizeof(combinedInput), "%s %s", startupCmds[i], commandArg);
                            } else {
                                snprintf(combinedInput, sizeof(combinedInput), "%s", startupCmds[i]);
                            }
                            phase = startupPhase(board, phase, combinedInput, commandArg, message,&undoStack, &redoStack);
                            commandArg[0] = '\0';  // ryd input efter brug
                        }
                    }
                } else if (phase == PLAY) {
                    for (int i = 0; i < NUM_PLAY_BUTTONS; i++) {
                        SDL_Rect r = playButtons[i].rect;
                        if (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h) {
                            char combinedInput[128];
                            if (strlen(commandArg) > 0) {
                                snprintf(combinedInput, sizeof(combinedInput), "%s %s", playCmds[i], commandArg);
                            } else {
                                snprintf(combinedInput, sizeof(combinedInput), "%s", playCmds[i]);
                            }
                            phase = playPhase(board, phase, combinedInput, commandArg, message, &undoStack, &redoStack);
                            commandArg[0] = '\0';
                            commandArg[0] = '\0';  // ryd input efter brug
                        }
                    }
                }

                int spacingX = 100;
                int spacingY = 30;
                int clickedOnSomething = 0;
                for (int col = 0; col < 7; col++) {
                    int cx = 20 + col * spacingX;
                    CardNode *node = board->columns[col].tail;
                    int row = board->columns[col].size - 1;

                    while (node) {
                        int cardHeight = (node == board->columns[col].tail || (
                                              node->prev == NULL && node->next == NULL))
                                             ? 120
                                             : spacingY;
                        SDL_Rect cardRect = {cx, 20 + row * spacingY, 80, cardHeight};

                        // Klik-tjek
                        if (x >= cardRect.x && x <= cardRect.x + cardRect.w &&
                            y >= cardRect.y && y <= cardRect.y + cardRect.h) {
                            if (!node->card.faceUp) break;

                            if (selectedCard == NULL) {
                                selectedCard = node;
                                selectedCol = col;
                                snprintf(message, 256, "Valgt kort: %d%c", node->card.rank, node->card.suit);
                                clickedOnSomething = 1;
                            } else {
                                CardNode *target = board->columns[col].tail;

                                if ((col != selectedCol) || selectedCol == -1) {
                                    if ((target && validMoveC(selectedCard, target)) ||
                                        (!target && selectedCard->card.rank == 13)) {
                                        changeBoardStack(&undoStack, &redoStack, board);
                                        if (selectedCol != -1) {
                                            // Flyt fra kolonne
                                            moveBetweenColumns(selectedCard, &board->columns[selectedCol],
                                                               &board->columns[col]);
                                            flipLastCardIfAny(&board->columns[selectedCol]);
                                        } else {
                                            // Flyt fra foundation
                                            for (int f = 0; f < 4; f++) {
                                                if (board->foundations[f].tail == selectedCard) {
                                                    moveBetweenColumns(selectedCard, &board->foundations[f],
                                                                       &board->columns[col]);
                                                    flipLastCardIfAny(&board->foundations[f]);
                                                    break;
                                                }
                                            }
                                        }

                                        clickedOnSomething = 1;
                                        snprintf(message, 256, "Flyttede kortet.");
                                        } else {
                                            snprintf(message, 256, "Ugyldigt træk.");
                                        }
                                } else {
                                    snprintf(message, 256, "Ugyldigt træk.");
                                }
                                selectedCard = NULL;
                                selectedCol = -1;
                            }
                        }

                            node = node->prev;
                            row--;
                        }

                        // Klik på tom slot, hvis kolonnen er tom
                        if (selectedCard && board->columns[col].size == 0) {
                            SDL_Rect emptySlot = {20 + col * spacingX, 20, 80, 120};

                            if (x >= emptySlot.x && x <= emptySlot.x + emptySlot.w &&
                                y >= emptySlot.y && y <= emptySlot.y + emptySlot.h) {
                                if (selectedCard->card.rank == 13) {
                                    moveBetweenColumns(selectedCard, &board->columns[selectedCol],
                                                       &board->columns[col]);
                                    flipLastCardIfAny(&board->columns[selectedCol]);
                                    snprintf(message, 256, "Flyttede konge til tom kolonne.");
                                    clickedOnSomething = 1;
                                } else {
                                    snprintf(message, 256, "Kun en konge må flyttes til en tom kolonne.");
                                }

                                selectedCard = NULL;
                                selectedCol = -1;
                            }
                        }
                    }
                    // Tjek klik på foundations

                    for (int f = 0; f < 4; f++) {
                        SDL_Rect fslot = {800, 20 + 130 * f, 80, 120};
                        CardNode *fCard = board->foundations[f].tail;

                        if (x >= fslot.x && x <= fslot.x + fslot.w &&
                            y >= fslot.y && y <= fslot.y + fslot.h) {
                            if (selectedCard != NULL) {
                                if (validMoveF(selectedCard, fCard)) {
                                    changeBoardStack(&undoStack, &redoStack, board);

                                    if (selectedCol != -1) {
                                        moveToFoundation(selectedCard, &board->columns[selectedCol],
                                                         &board->foundations[f]);
                                        flipLastCardIfAny(&board->columns[selectedCol]);
                                    } else {
                                        // Find hvilken foundation kortet kommer fra
                                        for (int i = 0; i < 4; i++) {
                                            if (board->foundations[i].tail == selectedCard) {
                                                moveToFoundation(selectedCard, &board->foundations[i],
                                                                 &board->foundations[f]);
                                                flipLastCardIfAny(&board->foundations[i]);
                                                break;
                                            }
                                        }
                                    }
                                    snprintf(message, 256, "Flyttede kort til foundation.");
                                    clickedOnSomething = 1;
                                } else {
                                    snprintf(message, 256, "Ugyldigt foundation-træk.");
                                }

                                selectedCard = NULL;
                                selectedCol = -1;
                            } else if (fCard && fCard->card.faceUp) {
                                selectedCard = fCard;
                                selectedCol = -1;
                                clickedOnSomething = 1;
                                snprintf(message, 256, "Valgt kort fra foundation.");
                            }

                            break;
                        }
                    }
                    if (!clickedOnSomething && selectedCard != NULL) {
                        selectedCard = NULL;
                        selectedCol = -1;
                        snprintf(message, 256, "Valg annulleret.");
                    }
                }
            }


            if (phase == STARTUP) {
                drawBoardStartUpPhase(renderer, window, board, font, message);
            } else if (phase == PLAY) {
                if (isWinState(board)) {
                    strcpy(message, "Du har vundet,flot, du har spildt noget af dit liv - Tak for i dag!.");
                   drawBoardPlayPhase(renderer, window, board, font, message);
                }
                drawBoardPlayPhase(renderer, window, board, font, message);
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
