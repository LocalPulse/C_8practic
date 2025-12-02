#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* [[ Constants ]] */
#define HIGHLIGHT "\033[7m"
#define RESET "\033[0m"

/* [[ Functions Prototypes ]] */
bool _clearConsole();
void CreateMenu(char **strings);

/*[[ Functions ]]*/

/*
 * @brief Очистка консоли
 * Функция очищает консоль. Проверяя систему и наличие ошибок в процессе очистки.
 *
 *  @return true если очистка прошла успешно, иначе false
 */
bool _clearConsole() {
    int result;

    //логика предпроцессора перед очисткой консоли на Windows
    #ifdef _WIN32
        result = system("cls");
    #else
        result = system("clear");
    #endif

    if (result != 0) {
        printf("Error clearing console.\n");
        return false;
    }
    return true;
}
/*
 * @brief Утилита для создания меню игры
 *
 *  Функция принимает массив строк и количество элементов в массиве.
 *  Обрабатывает массив и выводит на экран меню с выбранными элементами.
 *  Давая пользователю возможность выбирать элементы меню.
 *
 *  @param strings Массив строк для создания меню
 *  @param count Количество элементов в массиве
 *  @example Пример использования:
 *      char *menu_items[] = {"Start", "Options", "Exit"};
 *      CreateMenu(menu_items);
 */
void CreateMenu(char **strings) {
    bool resultClear = _clearConsole();
    printf("Clear console result: %s\n", resultClear ? "Success" : "Failed");

    int count = 0;
    while(strings[count] != NULL) count++; //Высчитываем сколько строк в массиве

    printf("\n▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n");
    printf("█            ГЛАВНОЕ МЕНЮ            █\n");
    printf("▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n\n");

    for (int i = 0; i < count; i++) {
        if(i == 0) {
            printf("  ▶ %s%2d. %s%s\n", HIGHLIGHT, i + 1, strings[i], RESET);
        } else {
            printf("    %2d. %s\n", i + 1, strings[i]);
        }

        printf("\n──────────────────────────────────────────\n");
        printf("[↑↓] - Навигация   [Enter] - Выбор   [0] - Выход\n");
        printf("──────────────────────────────────────────\n");
    }
}
