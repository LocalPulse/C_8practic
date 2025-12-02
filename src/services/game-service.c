#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/console.h"
#include "../models/game.h"

bool isGame = false;

/*
 * @brief Показ вступительного текста
 */
void ShowIntro() {
    _clearConsole();
    printf("\n=======================================================\n");
    printf("|                                                   |\n");
    printf("|        TAJNA STAROGO MANUSKRIPTA                 |\n");
    printf("|                                                   |\n");
    printf("=======================================================\n\n");
    
    printf("Вы - начинающий повар, ищущий легендарный рецепт\n");
    printf("древнего блюда, который был утерян много лет назад.\n\n");
    printf("Слухи гласят, что рецепт хранится в заброшенном особняке\n");
    printf("известного кулинара. Вы решили рискнуть и отправиться\n");
    printf("на поиски этого сокровища...\n\n");
    
    printf("Ваша цель: найти все необходимые предметы и разгадать\n");
    printf("тайну древнего рецепта!\n\n");
    
    WaitForEnter();
}

/*
 * @brief Главное меню игры
 */
int ShowMainMenu() {
    char *options[] = {"Начать игру", "Выход", NULL};
    return CreateMenu(options);
}

/*
 * @brief Отображение экрана победы
 */
void ShowWinScreen() {
    _clearConsole();
    printf("\n=======================================================\n");
    printf("|                                                   |\n");
    printf("|          POZDRAVLYAEM! VY POBEDILI!              |\n");
    printf("|                                                   |\n");
    printf("=======================================================\n\n");
    
    printf("Вы нашли древний манускрипт с секретным рецептом!\n\n");
    printf("В манускрипте записан рецепт легендарного блюда,\n");
    printf("которое было утеряно много лет назад.\n\n");
    printf("Теперь вы сможете воссоздать это произведение\n");
    printf("кулинарного искусства и прославиться как великий повар!\n\n");
    printf("Ваше приключение завершено успешно!\n\n");
    printf("=======================================================\n");
    printf("|      Spasibo za igru! Do novyh vstrech!            |\n");
    printf("=======================================================\n\n");
    
    WaitForEnter();
}

/*
 * @brief Основной игровой цикл
 */
void GameLoop() {
    int choice;
    
    while (!IsGameWon() && !IsGameOver()) {
        DisplayLocation();
        
        // Проверка условий победы
        if (CheckWinCondition()) {
            break;
        }
        
        Location *loc = GetCurrentLocation();
        
        printf("\n[0] Инвентарь\n");
        printf("Выберите действие (0-%d): ", loc->actionCount);
        
        char buf[64];
        if (fgets(buf, sizeof buf, stdin) != NULL) {
            if (sscanf(buf, "%d", &choice) == 1) {
                if (choice == 0) {
                    DisplayInventory();
                } else if (choice >= 1 && choice <= loc->actionCount) {
                    ExecuteAction(choice - 1);
                } else {
                    printf("Неверный выбор! Попробуйте снова.\n");
                    WaitForEnter();
                }
            } else {
                printf("Неверный ввод! Попробуйте снова.\n");
                WaitForEnter();
            }
        }
    }
    
    if (IsGameWon()) {
        ShowWinScreen();
    }
}

/*
 * @brief Инициализация игры
 */
void GameInit() {
    if (isGame) {
        printf("Игра уже инициализирована.\n");
        return;
    }
    
    isGame = true;
    
    int menuChoice = ShowMainMenu();
    
    if (menuChoice == 1) {
        ShowIntro();
        InitGameModel();
        GameLoop();
    } else if (menuChoice == 2) {
        printf("До свидания!\n");
        exit(0);
    } else {
        printf("Неверный выбор.\n");
    }
}

/*
 * @brief Остановка игры
 */
void GameStop() {
    isGame = false;
}
