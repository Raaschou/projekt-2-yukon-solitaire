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

    printf("Vil du køre spillet i:\n");
    printf("1 - Terminal\n");
    printf("2 - GUI\n");
    printf("Tryk 1 eller 2 for at fortsætte: ");
    scanf("%d", &mode);

    if (mode == 1) {
        gameLoopTerminal(&board);
    } else if (mode == 2) {
        gameLoopGUI(&board);
    } else {
        printf("Ugyldigt valg. Afslutter.\n");
    }

    return 0;
}

