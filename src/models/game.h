#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define MAX_INVENTORY_SIZE 10
#define MAX_LOCATION_NAME 64
#define MAX_ITEM_NAME 32
#define MAX_DESCRIPTION 256
#define MAX_ACTIONS 6

/* [[ Типы локаций ]] */
typedef enum {
    LOCATION_KITCHEN,
    LOCATION_DINING_ROOM,
    LOCATION_LIBRARY,
    LOCATION_BASEMENT,
    LOCATION_ATTIC,
    LOCATION_GARDEN,
    LOCATION_COUNT
} LocationType;

/* [[ Структура предмета ]] */
typedef struct {
    char name[MAX_ITEM_NAME];
    char description[MAX_DESCRIPTION];
    bool isCollected;
} Item;

/* [[ Структура действия ]] */
typedef struct {
    char text[128];
    bool available;
    int targetLocation;
    bool requiresItem;
    char requiredItemName[MAX_ITEM_NAME];
    char resultText[MAX_DESCRIPTION];
    Item* givesItem;
} Action;

/* [[ Структура локации ]] */
typedef struct {
    LocationType id;
    char name[MAX_LOCATION_NAME];
    char description[MAX_DESCRIPTION];
    Item items[3];
    int itemCount;
    Action actions[MAX_ACTIONS];
    int actionCount;
    bool visited;
} Location;

/* [[ Структура игры ]] */
typedef struct {
    Location locations[LOCATION_COUNT];
    Item inventory[MAX_INVENTORY_SIZE];
    int inventoryCount;
    LocationType currentLocation;
    bool gameWon;
    bool gameOver;
} GameState;

/* [[ Функции игры ]] */
void InitGameModel();
Location* GetCurrentLocation();
bool HasItem(const char *itemName);
void AddToInventory(Item *item);
void MoveToLocation(LocationType newLocation);
void DisplayInventory();
void DisplayLocation();
bool ExecuteAction(int actionIndex);
bool PickUpItem(const char *itemName);
bool CheckWinCondition();
bool IsGameWon();
bool IsGameOver();

#endif
