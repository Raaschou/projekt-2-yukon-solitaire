#include <stdio.h>
#include <stdlib.h>
#define TOTAL_CARDS 52


#include <stdbool.h>
#include "../Include/Main.h"

/**
* Starter spillet og spørger brugeren om, hvordan spillet skal spilles:
 * enten terminalbaseret eller i GUI. Brugerinputtet valideres, og det relevante game loop
 * startes afhængigt af brugerens valg.
 *
 * @return int 0 ved succesfuld afslutning.
 */
int main(int argc, char *argv[]) {
    Board board = {0};

    int mode = 0;

    while (1) {
        printf("Vil du køre spillet i:\n");
        printf("1 - Terminal\n");
        printf("2 - GUI\n");
        printf("Tryk 1 eller 2 for at fortsætte: ");
        if (scanf("%d", &mode) != 1|| (mode != 1 && mode != 2)) {
            clearStdin();
            printf("\nUgyldigt input. Du skal skrive 1 eller 2.\n\n");
            continue;
        }
        clearStdin();

        if (mode == 1) {
            gameLoopTerminal(&board);
            break;
        }
        if (mode == 2) {
            gameLoopGUI(&board);
            break;
        }
    }
    return 0;
}
/**
 * Rydder standard input (stdin) frem til og med næste newline ('\n').
 * Bruges efter scanf for at undgå uønsket input i buffer.
 */
void clearStdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

