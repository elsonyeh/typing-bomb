// ui.h - 用戶界面相關函數
#ifndef UI_H
#define UI_H

#include "common.h"
#include "items.h"

// 基本UI函數
void clearScreen();
void setColor(int color);
void gotoxy(int x, int y);
void initConsole();
void showMessage(const char *message, int color);
void showProgressBar(double timeLeft, double totalTime);

// 遊戲UI函數
void showPlayerStatus(Player *player, int x, int y);
void showMainMenu();
void showModeMenu();
void showGameOver(Player *player);
void showHelp();
void showWelcome();
void showItemNotification(Player *player);

#endif // UI_H