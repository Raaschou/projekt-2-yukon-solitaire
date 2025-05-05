#include <stdio.h>
#include <stdlib.h>
#define TOTAL_CARDS 52


#include <stdbool.h>
#include "../Include/Main.h"





// int main(void) {
//     // Initialiser spillebrættet
//
//     runGUI(); // Starter SDL GUI
//     return 0;
//
//     Board board = {0};
//     srand((unsigned int)time(NULL));
//     gameLoop(&board);
//     return 0;
// }
int main(void) {
    Board board = {0};

    int mode = 0;

    while (1) {
        printf("Vil du køre spillet i:\n");
        printf("1 - Terminal\n");
        printf("2 - GUI\n");
        printf("Tryk 1 eller 2 for at fortsætte: ");
        if (scanf("%d", &mode) != 1) {
            // Ryd stdin hvis brugeren skriver noget forkert
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Ugyldigt input. Du skal skrive 1 eller 2.\n\n");
            continue;
        }

        if (mode == 1) {
            gameLoopTerminal(&board);
            break;
        } else if (mode == 2) {
            gameLoopGUI(&board);
            break;
        } else {
            printf("Ugyldigt valg. Du skal skrive 1 eller 2.\n\n");
        }
    }

    return 0;
}

