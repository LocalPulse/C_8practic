#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "../utils/console.h"

/* [[ Глобальное состояние игры ]] */
static GameState game;

/* [[ Прототипы внутренних функций ]] */
static void InitializeLocations();
static void InitializeItems();
static Item* CreateItem(const char *name, const char *description);
static void AddActionToLocation(LocationType loc, const char *text, int targetLoc, 
                                bool requiresItem, const char *reqItem, 
                                const char *resultText, Item* givesItem);

/* [[ Функции игры ]] */

/*
 * @brief Инициализация игровой модели
 * Создаёт все локации, предметы и начальное состояние
 */
void InitGameModel() {
    memset(&game, 0, sizeof(GameState));
    game.currentLocation = LOCATION_KITCHEN;
    game.gameWon = false;
    game.gameOver = false;
    
    InitializeItems();
    InitializeLocations();
}

/*
 * @brief Получить текущую локацию
 * @return Указатель на текущую локацию
 */
Location* GetCurrentLocation() {
    return &game.locations[game.currentLocation];
}

/*
 * @brief Проверка наличия предмета в инвентаре
 * @param itemName Имя предмета для проверки
 * @return true если предмет есть в инвентаре
 */
bool HasItem(const char *itemName) {
    for (int i = 0; i < game.inventoryCount; i++) {
        if (strcmp(game.inventory[i].name, itemName) == 0) {
            return true;
        }
    }
    return false;
}

/*
 * @brief Добавить предмет в инвентарь
 * @param item Предмет для добавления
 */
void AddToInventory(Item *item) {
    if (game.inventoryCount >= MAX_INVENTORY_SIZE) {
        printf("Инвентарь переполнен!\n");
        return;
    }
    
    strcpy(game.inventory[game.inventoryCount].name, item->name);
    strcpy(game.inventory[game.inventoryCount].description, item->description);
    game.inventory[game.inventoryCount].isCollected = true;
    game.inventoryCount++;
    printf("✓ Добавлено в инвентарь: %s\n", item->name);
}

/*
 * @brief Переместиться в другую локацию
 * @param newLocation Тип новой локации
 */
void MoveToLocation(LocationType newLocation) {
    if (newLocation >= 0 && newLocation < LOCATION_COUNT) {
        game.currentLocation = newLocation;
    }
}

/*
 * @brief Отобразить инвентарь
 */
void DisplayInventory() {
    _clearConsole();
    printf("\n=====================================\n");
    printf("|         INVENTORY                 |\n");
    printf("|===================================|\n");
    
    if (game.inventoryCount == 0) {
        printf("|  (pusto)                         |\n");
    } else {
        for (int i = 0; i < game.inventoryCount; i++) {
            printf("|  [%d] %-28s |\n", i + 1, game.inventory[i].name);
        }
    }
    
    printf("=====================================\n");
    WaitForEnter();
}

/*
 * @brief Отобразить текущую локацию
 */
void DisplayLocation() {
    Location *loc = GetCurrentLocation();
    
    _clearConsole();
    printf("\n=======================================================\n");
    printf("|  %-53s |\n", loc->name);
    printf("=======================================================\n\n");
    
    printf("%s\n\n", loc->description);
    
    // Отображаем доступные предметы
    if (loc->itemCount > 0) {
        printf("Вы видите:\n");
        for (int i = 0; i < loc->itemCount; i++) {
            if (!loc->items[i].isCollected) {
                printf("  • %s - %s\n", loc->items[i].name, loc->items[i].description);
            }
        }
        printf("\n");
    }
    
    // Отображаем доступные действия
    printf("Доступные действия:\n");
    for (int i = 0; i < loc->actionCount; i++) {
        if (loc->actions[i].available) {
            printf("  [%d] %s\n", i + 1, loc->actions[i].text);
        }
    }
    
    loc->visited = true;
}

/*
 * @brief Выполнить действие
 * @param actionIndex Индекс действия
 */
bool ExecuteAction(int actionIndex) {
    Location *loc = GetCurrentLocation();
    
    if (actionIndex < 0 || actionIndex >= loc->actionCount) {
        printf("Неверное действие!\n");
        return false;
    }
    
    Action *action = &loc->actions[actionIndex];
    
    if (!action->available) {
        printf("Это действие недоступно!\n");
        return false;
    }
    
    // Проверка требований
    if (action->requiresItem && !HasItem(action->requiredItemName)) {
        printf("Вам нужен предмет: %s\n", action->requiredItemName);
        WaitForEnter();
        return false;
    }
    
    // Вывод результата
    if (strlen(action->resultText) > 0) {
        printf("\n%s\n", action->resultText);
    }
    
    // Перемещение
    if (action->targetLocation >= 0) {
        MoveToLocation(action->targetLocation);
    }
    
    // Добавление предмета
    if (action->givesItem != NULL && !action->givesItem->isCollected) {
        AddToInventory(action->givesItem);
        action->givesItem->isCollected = true;
    }
    
    WaitForEnter();
    return true;
}

/*
 * @brief Подобрать предмет
 * @param itemName Имя предмета
 */
bool PickUpItem(const char *itemName) {
    Location *loc = GetCurrentLocation();
    
    for (int i = 0; i < loc->itemCount; i++) {
        if (strcmp(loc->items[i].name, itemName) == 0 && !loc->items[i].isCollected) {
            AddToInventory(&loc->items[i]);
            loc->items[i].isCollected = true;
            return true;
        }
    }
    
    printf("Такого предмета здесь нет!\n");
    return false;
}

/*
 * @brief Проверка победы
 */
bool CheckWinCondition() {
    // Для победы нужен манускрипт с рецептом (главная цель)
    bool hasManuscript = HasItem("Древний манускрипт");
    
    // Игра считается выигранной, если у игрока есть манускрипт
    // Это главная цель - найти секретный рецепт
    if (hasManuscript) {
        game.gameWon = true;
        return true;
    }
    
    return false;
}

bool IsGameWon() {
    return game.gameWon;
}

bool IsGameOver() {
    return game.gameOver;
}

/* [[ Внутренние функции инициализации ]] */

static Item* CreateItem(const char *name, const char *description) {
    static Item items[20];
    static int itemIndex = 0;
    
    if (itemIndex >= 20) return NULL;
    
    strcpy(items[itemIndex].name, name);
    strcpy(items[itemIndex].description, description);
    items[itemIndex].isCollected = false;
    
    return &items[itemIndex++];
}

static void InitializeItems() {
    // Предметы будут созданы в InitializeLocations
}

static void InitializeLocations() {
    // КУХНЯ
    Location *kitchen = &game.locations[LOCATION_KITCHEN];
    kitchen->id = LOCATION_KITCHEN;
    strcpy(kitchen->name, "Кухня");
    strcpy(kitchen->description, "Старая кухня, покрытая пылью и паутиной. Стол завален остатками давно испорченной еды. На полках стоят пустые банки. Странный запах старого дерева висит в воздухе.");
    kitchen->itemCount = 1;
    kitchen->items[0] = *CreateItem("Газета", "Старая газета с вырезкой о пропавшем поваре");
    kitchen->items[0].isCollected = false;
    kitchen->actionCount = 2;
    
    strcpy(kitchen->actions[0].text, "Идти в столовую");
    kitchen->actions[0].available = true;
    kitchen->actions[0].targetLocation = LOCATION_DINING_ROOM;
    kitchen->actions[0].requiresItem = false;
    strcpy(kitchen->actions[0].resultText, "Вы выходите из кухни в столовую.");
    
    strcpy(kitchen->actions[1].text, "Взять газету");
    kitchen->actions[1].available = true;
    kitchen->actions[1].targetLocation = -1;
    kitchen->actions[1].requiresItem = false;
    strcpy(kitchen->actions[1].resultText, "Вы подобрали газету. В ней написано о таинственном рецепте.");
    kitchen->actions[1].givesItem = &kitchen->items[0];
    
    // СТОЛОВАЯ
    Location *dining = &game.locations[LOCATION_DINING_ROOM];
    dining->id = LOCATION_DINING_ROOM;
    strcpy(dining->name, "Столовая");
    strcpy(dining->description, "Просторная столовая с массивным дубовым столом посередине. На стенах висят портреты предков, которые смотрят на вас загадочными взглядами. На стене висит старая карта особняка.");
    dining->itemCount = 1;
    dining->items[0] = *CreateItem("Карта", "Старая карта особняка с отметками");
    dining->items[0].isCollected = false;
    dining->actionCount = 5;
    
    strcpy(dining->actions[0].text, "Идти в библиотеку");
    dining->actions[0].available = true;
    dining->actions[0].targetLocation = LOCATION_LIBRARY;
    dining->actions[0].requiresItem = false;
    strcpy(dining->actions[0].resultText, "Вы направляетесь в библиотеку.");
    
    strcpy(dining->actions[1].text, "Вернуться на кухню");
    dining->actions[1].available = true;
    dining->actions[1].targetLocation = LOCATION_KITCHEN;
    dining->actions[1].requiresItem = false;
    strcpy(dining->actions[1].resultText, "Вы возвращаетесь на кухню.");
    
    strcpy(dining->actions[2].text, "Идти в подвал");
    dining->actions[2].available = true;
    dining->actions[2].targetLocation = LOCATION_BASEMENT;
    dining->actions[2].requiresItem = false;
    strcpy(dining->actions[2].resultText, "Вы спускаетесь в тёмный подвал.");
    
    strcpy(dining->actions[3].text, "Выйти в сад");
    dining->actions[3].available = true;
    dining->actions[3].targetLocation = LOCATION_GARDEN;
    dining->actions[3].requiresItem = false;
    strcpy(dining->actions[3].resultText, "Вы выходите через заднюю дверь в сад.");
    
    strcpy(dining->actions[4].text, "Взять карту");
    dining->actions[4].available = true;
    dining->actions[4].targetLocation = -1;
    dining->actions[4].requiresItem = false;
    strcpy(dining->actions[4].resultText, "Карта добавлена в инвентарь.");
    dining->actions[4].givesItem = &dining->items[0];
    
    // БИБЛИОТЕКА
    Location *library = &game.locations[LOCATION_LIBRARY];
    library->id = LOCATION_LIBRARY;
    strcpy(library->name, "Библиотека");
    strcpy(library->description, "Библиотека с высокими стеллажами до потолка, полными старых томов. В углу стоит деревянная лестница, ведущая наверх. На столе лежит открытая книга рецептов, а рядом блестит бронзовый ключ.");
    library->itemCount = 2;
    library->items[0] = *CreateItem("Ключ от чердака", "Бронзовый ключ с гравировкой");
    library->items[1] = *CreateItem("Книга рецептов", "Старая книга с кулинарными рецептами");
    library->items[0].isCollected = false;
    library->items[1].isCollected = false;
    library->actionCount = 4;
    
    strcpy(library->actions[0].text, "Вернуться в столовую");
    library->actions[0].available = true;
    library->actions[0].targetLocation = LOCATION_DINING_ROOM;
    library->actions[0].requiresItem = false;
    strcpy(library->actions[0].resultText, "Вы возвращаетесь в столовую.");
    
    strcpy(library->actions[1].text, "Взять ключ");
    library->actions[1].available = true;
    library->actions[1].targetLocation = -1;
    library->actions[1].requiresItem = false;
    strcpy(library->actions[1].resultText, "Вы взяли ключ от чердака.");
    library->actions[1].givesItem = &library->items[0];
    
    strcpy(library->actions[2].text, "Прочитать книгу рецептов");
    library->actions[2].available = true;
    library->actions[2].targetLocation = -1;
    library->actions[2].requiresItem = false;
    strcpy(library->actions[2].resultText, "В книге упоминается секретный ингредиент, хранящийся в подвале.");
    
    strcpy(library->actions[3].text, "Подняться на чердак");
    library->actions[3].available = true;
    library->actions[3].targetLocation = LOCATION_ATTIC;
    library->actions[3].requiresItem = false;
    strcpy(library->actions[3].resultText, "Вы поднимаетесь по лестнице на чердак.");
    
    // ПОДВАЛ
    Location *basement = &game.locations[LOCATION_BASEMENT];
    basement->id = LOCATION_BASEMENT;
    strcpy(basement->name, "Подвал");
    strcpy(basement->description, "Тёмный и сырой подвал с низким потолком. Влажный воздух заставляет вас кашлять. На полках стоят банки с консервами, покрытые толстым слоем пыли. В углу стоит старый деревянный сундук.");
    basement->itemCount = 1;
    basement->items[0] = *CreateItem("Старый ключ", "Ржавый железный ключ");
    basement->items[0].isCollected = false;
    basement->actionCount = 3;
    
    strcpy(basement->actions[0].text, "Вернуться в столовую");
    basement->actions[0].available = true;
    basement->actions[0].targetLocation = LOCATION_DINING_ROOM;
    basement->actions[0].requiresItem = false;
    strcpy(basement->actions[0].resultText, "Вы поднимаетесь обратно в столовую.");
    
    strcpy(basement->actions[1].text, "Открыть сундук");
    basement->actions[1].available = true;
    basement->actions[1].targetLocation = -1;
    basement->actions[1].requiresItem = false;
    strcpy(basement->actions[1].resultText, "Сундук открыт! Внутри вы находите старый ключ.");
    basement->actions[1].givesItem = &basement->items[0];
    
    strcpy(basement->actions[2].text, "Осмотреть банки");
    basement->actions[2].available = true;
    basement->actions[2].targetLocation = -1;
    basement->actions[2].requiresItem = false;
    strcpy(basement->actions[2].resultText, "Все банки пусты, кроме одной с загадочной этикеткой.");
    
    // ЧЕРДАК
    Location *attic = &game.locations[LOCATION_ATTIC];
    attic->id = LOCATION_ATTIC;
    strcpy(attic->name, "Чердак");
    strcpy(attic->description, "Пыльный чердак, заваленный древними вещами и сундуками. Сквозь пыльные окна пробивается тусклый свет. В центре стоит старый письменный стол, на котором лежит древний манускрипт с восковыми печатями.");
    attic->itemCount = 1;
    attic->items[0] = *CreateItem("Древний манускрипт", "Старинная рукопись с секретным рецептом");
    attic->items[0].isCollected = false;
    attic->actionCount = 2;
    
    strcpy(attic->actions[0].text, "Взять манускрипт");
    attic->actions[0].available = true;
    attic->actions[0].targetLocation = -1;
    attic->actions[0].requiresItem = false;
    strcpy(attic->actions[0].resultText, "Вы взяли древний манускрипт! В нём описан секретный рецепт!");
    attic->actions[0].givesItem = &attic->items[0];
    
    strcpy(attic->actions[1].text, "Вернуться в библиотеку");
    attic->actions[1].available = true;
    attic->actions[1].targetLocation = LOCATION_LIBRARY;
    attic->actions[1].requiresItem = false;
    strcpy(attic->actions[1].resultText, "Вы спускаетесь обратно в библиотеку.");
    
    // САД
    Location *garden = &game.locations[LOCATION_GARDEN];
    garden->id = LOCATION_GARDEN;
    strcpy(garden->name, "Сад");
    strcpy(garden->description, "Заброшенный сад с заросшими дорожками и буйной растительностью. В центре стоит полуразрушенная беседка. Рядом растут редкие травы, которые когда-то использовались в кулинарии. В беседке лежит старая восковая свеча.");
    garden->itemCount = 1;
    garden->items[0] = *CreateItem("Восковая свеча", "Старая восковая свеча");
    garden->items[0].isCollected = false;
    garden->actionCount = 2;
    
    strcpy(garden->actions[0].text, "Взять свечу");
    garden->actions[0].available = true;
    garden->actions[0].targetLocation = -1;
    garden->actions[0].requiresItem = false;
    strcpy(garden->actions[0].resultText, "Вы взяли восковую свечу из беседки.");
    garden->actions[0].givesItem = &garden->items[0];
    
    strcpy(garden->actions[1].text, "Вернуться в столовую");
    garden->actions[1].available = true;
    garden->actions[1].targetLocation = LOCATION_DINING_ROOM;
    garden->actions[1].requiresItem = false;
    strcpy(garden->actions[1].resultText, "Вы возвращаетесь в особняк.");
}

