// scoreboard.h - 排行榜功能
#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "common.h"

// 排行榜函數
void saveScore(const char *name, int score, int maxCombo, const char *mode);
int getPlayerRank(const char *name, int score, const char *mode);
void showScoreboard();

#endif // SCOREBOARD_H