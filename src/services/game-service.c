#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/console.c"

bool isGame = false;

void GameInit() {
    if (isGame) {
        printf("Game is already initialized.\n");
        return;
    }
    isGame = true;

    char *options[] = {"Start", "Exit"};
    CreateMenu(options);
}


void GameStop() {
    isGame = false;
}
