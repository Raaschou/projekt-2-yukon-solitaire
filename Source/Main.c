#include <stdio.h>
#include <stdlib.h>
#define TOTAL_CARDS 52


#include <stdbool.h>
#include "../Include/Main.h"

#include <unistd.h>
#include <sys/socket.h>


#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define PORT 4242

int startSocketServer() {
    int serverFd, clientFd;
    struct sockaddr_in addr;

    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    listen(serverFd, 1);
    printf("C-server lytter på port %d...\n", PORT);

    clientFd = accept(serverFd, NULL, NULL);
    if (clientFd < 0) {
        perror("accept");
        exit(1);
    }

    return clientFd;
}
int clientFd = -1;
int main(void) {
    Board board = {0};
    int mode = 0;

    printf("Vil du køre spillet i:\n");
    printf("1 - Terminal\n");
    printf("2 - GUI (med socket)\n");
    printf("Tryk 1 eller 2 for at fortsætte: ");
    scanf("%d", &mode);

    if (mode == 1) {
        printf("Du kan nu spille i terminalen\n");
        gameLoop(&board, 0);  // bruger scanf
    } else if (mode == 2) {
        if (mode == 2) {
            printf("Starter socket-server...\n");
            clientFd = startSocketServer();  // brug den nye navn

            gameLoop(&board, 2);  // socket-mode
        }  // socket-mode
    } else {
        printf("Ugyldigt valg. Afslutter...\n");
    }

    return 0;
}
