#include <stdbool.h>
#include <stdio.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include "services/game-service.h"

/*
 * @brief Инициализация консоли и кодировки для Windows
 */
void InitConsole() {
#ifdef _WIN32
    // Меняем кодовую страницу консоли на CP1251 (Windows-1251)
    // CP1251 - стандартная кодировка Windows для русского языка
    system("chcp 1251 >nul 2>&1");
    
    // Устанавливаем кодировку CP1251 через WinAPI
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    
    // Альтернатива: UTF-8 (если CP1251 не работает, закомментировать строки выше и раскомментировать ниже):
    // system("chcp 65001 >nul 2>&1");
    // SetConsoleOutputCP(65001);
    // SetConsoleCP(65001);
#endif
    // Устанавливаем локаль для поддержки русского языка
    setlocale(LC_ALL, "Russian");
    setlocale(LC_CTYPE, "Russian");
    
    // Альтернативный вариант локали (если не работает):
    // setlocale(LC_ALL, "ru_RU.CP1251");
    // setlocale(LC_CTYPE, "ru_RU.CP1251");
}

int main() {
   InitConsole();
   GameInit();
   return 0;
}
