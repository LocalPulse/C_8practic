#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdbool.h>

/* [[ Functions Prototypes ]] */
bool _clearConsole();
int CreateMenu(char **strings);
void PrintText(const char *text);
void WaitForEnter();

#endif

