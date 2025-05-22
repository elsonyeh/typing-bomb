// items.h - 道具系統
#ifndef ITEMS_H
#define ITEMS_H

#include "common.h"

// 道具函數
void giveRandomItem(Player *player);
void useItem(Player *player, int itemIndex);
void resetItemEffects(Player *player);               // 重置道具效果
int applyScoreDouble(Player *player, int baseScore); // 應用雙倍分數
const char *getItemDescription(ItemType type);

#endif // ITEMS_H