// items.c - 道具系統（完整修正版：避免效果累積）
#include "items.h"
#include "ui.h"
#include "game.h"
#include "player.h"

// 給予隨機道具 - 降低連續獲得相同道具的機率
void giveRandomItem(Player *player)
{
    if (player->itemCount >= 3)
        return;

    static int lastItem = -1; // 記錄上次獲得的道具
    int itemIndex = rand() % NUM_ITEMS;

    // 如果獲得了和上次相同的道具，有70%的機率重新隨機
    if (itemIndex == lastItem && lastItem != -1 && NUM_ITEMS > 1)
    {
        // 70%機率重新隨機，30%機率保持原樣（允許連續獲得）
        if (rand() % 100 < 70)
        {
            // 重新隨機，最多嘗試3次避免無限循環
            int attempts = 0;
            do
            {
                itemIndex = rand() % NUM_ITEMS;
                attempts++;
            } while (itemIndex == lastItem && attempts < 3);
        }
        // 如果30%機率或重試失敗，就保持原來的道具（允許連續獲得）
    }

    lastItem = itemIndex;

    // 確保道具索引在有效範圍內
    if (itemIndex < 0 || itemIndex >= NUM_ITEMS)
    {
        itemIndex = 0; // 默認給第一個道具
    }

    player->activeItems[player->itemCount] = &items[itemIndex];
    player->itemCount++;

    updatePlayerStats(player);

    // 顯示獲得道具的訊息（包含道具符號和名稱）
    char itemMsg[80];

    // 如果連續獲得相同道具，添加特殊提示
    if (itemIndex == lastItem && player->itemCount > 1 &&
        player->activeItems[player->itemCount - 2]->type == player->activeItems[player->itemCount - 1]->type)
    {
        sprintf(itemMsg, "🎁 幸運！再次獲得：%s %s",
                player->activeItems[player->itemCount - 1]->symbol,
                player->activeItems[player->itemCount - 1]->name);
    }
    else
    {
        sprintf(itemMsg, "🎁 獲得道具：%s %s",
                player->activeItems[player->itemCount - 1]->symbol,
                player->activeItems[player->itemCount - 1]->name);
    }
    updateMessage(itemMsg, COLOR_ITEM);

    // 播放獲得道具的音效
    playItemSound(); // 🔊 【音效標記 15】獲得道具音效：獲得道具時的提示音

    Sleep(350); // 等待音效播放完成

    Sleep(1150); // 調整剩余等待時間
}

// 使用道具
void useItem(Player *player, int itemIndex)
{
    if (itemIndex >= player->itemCount)
        return;

    Item *item = player->activeItems[itemIndex];
    char message[80];

    switch (item->type)
    {
    case HEAL:
        if (player->lives < 3)
        {
            player->lives++;
            sprintf(message, "✨ 使用%s，生命值+1！", item->name);
        }
        else
        {
            sprintf(message, "❌ 生命值已滿，無法使用%s", item->name);
            updateMessage(message, COLOR_RED);
            Sleep(1200);
            return; // 不消耗道具
        }
        break;

    case TIME_EXTEND:
        // 只對當前回合增加時間，不累積
        player->extraTime += 2.0;
        sprintf(message, "⌛ 使用%s，本回合時限+2秒！", item->name);
        break;

    case SCORE_DOUBLE:
        // 只對下一次得分加倍，不累積
        if (!player->scoreDoubleNext)
        {
            player->scoreDoubleNext = 1;
            sprintf(message, "💎 使用%s，下次得分加倍！", item->name);
        }
        else
        {
            sprintf(message, "❌ 雙倍分數已啟用，無法重複使用");
            updateMessage(message, COLOR_RED);
            Sleep(1200);
            return; // 不消耗道具
        }
        break;

    case SHIELD:
        if (!player->hasShield)
        {
            player->hasShield = 1;
            sprintf(message, "🛡️ 使用%s，可抵擋一次失敗！", item->name);
        }
        else
        {
            sprintf(message, "❌ 防護罩已啟用，無法重複使用");
            updateMessage(message, COLOR_RED);
            Sleep(1200);
            return; // 不消耗道具
        }
        break;
    }

    // 移除已使用的道具
    for (int i = itemIndex; i < player->itemCount - 1; i++)
    {
        player->activeItems[i] = player->activeItems[i + 1];
    }
    player->itemCount--;

    updatePlayerStats(player);
    updateMessage(message, COLOR_ITEM);

    // 播放使用道具的音效
    Beep(1200, 100);
    Beep(1000, 100);
    Sleep(1500);
}

// 重置道具效果（每回合開始時調用）
void resetItemEffects(Player *player)
{
    // 重置臨時效果
    player->extraTime = 0.0;
}

// 應用得分加倍效果並重置
int applyScoreDouble(Player *player, int baseScore)
{
    if (player->scoreDoubleNext)
    {
        player->scoreDoubleNext = 0; // 使用後重置
        return baseScore * 2;
    }
    return baseScore;
}

// 獲取道具描述
const char *getItemDescription(ItemType type)
{
    switch (type)
    {
    case HEAL:
        return "回復生命值";
    case TIME_EXTEND:
        return "延長時間";
    case SCORE_DOUBLE:
        return "雙倍得分";
    case SHIELD:
        return "防護罩";
    default:
        return "未知道具";
    }
}