#include "player.h"

// 基礎音效函數
void playTypingSound() {
    Beep(750, 90);
}

void playSuccessSound() {
    Beep(1000, 100); // 高音
    Beep(1500, 100); // 更高音
}

void playFailSound() {
    Beep(500, 100); // 低音
    Beep(300, 200); // 更低音
}

void playComboSound() {
    Beep(800, 100); // 連擊音效
    Beep(1000, 100);
    Beep(1200, 120);
}

// 新增音效函數
void playTimeoutSound() {
    // 超時警告音效
    Beep(300, 300);
    Beep(250, 200);
    Beep(200, 400);
}

void playConfirmSound() {
    // 確認音效 - 清脆的提示音
    Beep(1200, 120);
    Beep(1500, 120);
}

void playExitSound() {
    // 退出音效 - 下降音調
    Beep(1000, 100);
    Beep(800, 100);
    Beep(600, 150);
}

void playDeleteSound() {
    // 刪除音效 - 短促的低音
    Beep(500, 120);
}

void playShieldSound() {
    // 護盾音效 - 魔法般的上升音調
    Beep(600, 100);
    Beep(800, 100);
    Beep(1000, 100);
    Beep(1200, 120);
}

void playBonusSound() {
    // 獎勵音效 - 歡快的旋律
    Beep(800, 90);
    Beep(1000, 90);
    Beep(1200, 90);
    Beep(1500, 100);
    Beep(1800, 120);
}

void playRoundCompleteSound() {
    // 回合完成音效 - 成就感
    Beep(1000, 100);
    Beep(1200, 100);
    Beep(1500, 150);
}

void playErrorSound() {
    // 錯誤音效 - 不和諧的音調
    Beep(300, 150);
    Beep(250, 150);
}

void playItemSound() {
    // 道具獲得音效 - 神秘感
    Beep(900, 100);
    Beep(700, 90);
    Beep(1100, 100);
    Beep(1300, 120);
}

void playWarningSound() {
    // 警告音效 - 危險提示
    Beep(800, 100);
    Sleep(50);
    Beep(800, 100);
    Sleep(50);
    Beep(800, 100);
}

void playLevelUpSound() {
    // 升級音效 - 進步感
    Beep(600, 100);
    Beep(800, 100);
    Beep(1000, 100);
    Beep(1200, 100);
    Beep(1500, 120);
}