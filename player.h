// player.h - 玩家相關功能
#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

// 基礎音效函數
void playTypingSound();
void playSuccessSound();
void playFailSound();
void playComboSound();

// 新增音效函數
void playTimeoutSound();       // 超時音效
void playConfirmSound();       // 確認音效
void playExitSound();          // 退出音效
void playDeleteSound();        // 刪除音效
void playShieldSound();        // 護盾音效
void playBonusSound();         // 獎勵音效
void playRoundCompleteSound(); // 回合完成音效
void playErrorSound();         // 錯誤音效
void playItemSound();          // 道具音效
void playWarningSound();       // 警告音效
void playLevelUpSound();       // 升級音效

#endif // PLAYER_H


/*
🎵 音效標記總結：
【音效標記 1】⚠️ playWarningSound() - 時間剩餘3/2/1秒時的警告音效
【音效標記 2】🚪 playExitSound() - 道具選擇階段按ESC跳過
【音效標記 3】✔️ playConfirmSound() - 選擇道具時的確認音效
【音效標記 4】❌ playErrorSound() - 無效道具選擇時的錯誤音效
【音效標記 5】🏆 playRoundCompleteSound() - 回合完成時的成就音效
【音效標記 6】🛡️ playShieldSound() - 護盾抵擋超時爆炸
【音效標記 7】💎 playBonusSound() - 雙倍分數生效時的獎勵音效
【音效標記 8】🛡️ playShieldSound() - 護盾抵擋錯誤答案爆炸
【音效標記 9】⏰ playTimeoutSound() - 時間到時的超時音效
【音效標記 10】✔️ playConfirmSound() - 按Enter確認輸入
【音效標記 11】🚪 playExitSound() - 遊戲中按ESC退出
【音效標記 12】🗑️ playDeleteSound() - 按Backspace刪除字符
【音效標記 13】🎹 playTypingSound() - 輸入小寫字母
【音效標記 14】🎹 playTypingSound() - 輸入大寫字母
【音效標記 15】📈 playLevelUpSound() - 難度提升時的升級音效

還有items.c中的：
【音效標記 16】🎁 playItemSound() - 獲得道具時的神秘音效
*/