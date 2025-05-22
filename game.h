// game.h - 遊戲核心邏輯（完整版）
#ifndef GAME_H
#define GAME_H

#include "common.h"

// 遊戲控制函數
void selectGameMode();
void playSinglePlayerGame();
int playRound(double *currentTime, Player *player);
int timedInput(char *buffer, int maxLen, double timeoutSec, const char *target, Player *player);
void adjustDifficultyCurve(Player *player, double *currentTime, int round);
void handleGameOver(Player *player, BOOL showAnimation);
void showRoundStartCountdown(int round);

// 畫面更新函數
void drawMainGameScreen(Player *player);
void updatePlayerStats(Player *player);
void updateProgressBar(double timeLeft, double timeoutSec);
void updateTargetWord(const char *target);
void updateMessage(const char *message, int color);
void clearInputArea();
void updateInputDisplay(const char *input);
void clearTargetAndMessage();
void askToUseItems(Player *player);

// 遊戲內功能
const char *getRandomWord();

#endif // GAME_H