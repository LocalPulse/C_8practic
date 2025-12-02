#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "console.h"

/* [[ Constants ]] */
#define HIGHLIGHT "\033[7m"
#define RESET "\033[0m"

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
int CreateMenu(char **strings) {
    bool resultClear = _clearConsole();
    // Очистка консоли выполнена

    int count = 0;
    while(strings[count] != NULL) count++; //Высчитываем сколько строк в массиве
    if (count <= 0) return -1;

    char buf[64]; //Выделяем память под input

    printf("\n===========================================\n");
    printf("|               Main Menu               |\n");
    printf("===========================================\n\n");

    for (int i = 0; i < count; i++) {
        printf("[%d] %s\n", i+1, strings[i]);
    }

    printf("\nEnter option: ");
    if(fgets(buf, sizeof buf, stdin) != NULL) {
       int opt;
       if (sscanf(buf, "%d", &opt) == 1){
           return opt;
       } else {
           printf("Invalid input\n");
       }
    }
    return -1;
}

/*
 * @brief Вывод текста с паузой
 * Выводит текст и ждёт нажатия Enter
 *
 * @param text Текст для вывода
 */
void PrintText(const char *text) {
    printf("%s\n", text);
}

/*
 * @brief Ожидание нажатия Enter
 * Очищает буфер ввода и ждёт нажатия Enter
 */
void WaitForEnter() {
    printf("\nPress Enter to continue...");
    char ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}
