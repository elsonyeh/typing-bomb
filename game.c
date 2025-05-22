// game.c - 遊戲核心邏輯
#include "game.h"
#include "ui.h"
#include "player.h"
#include "items.h"
#include "animation.h"
#include "scoreboard.h"

/*
 * 遊戲界面座標配置說明：
 * Y=1-3   : 遊戲標題區域
 * Y=5-10  : 玩家狀態框（生命、分數、連擊、道具）
 * Y=12-14 : 進度條區域
 * Y=16-20 : 目標單字和訊息合併區域（大框框）
 * Y=22-25 : 輸入框區域
 * Y=26-27 : 操作提示區域
 */

// 計算中文字符顯示寬度的函數
int getDisplayWidth(const char *str)
{
    int width = 0;
    int len = strlen(str);

    for (int i = 0; i < len; i++)
    {
        // 檢查是否為UTF-8中文字符的開始
        if ((unsigned char)str[i] >= 0x80)
        {
            // UTF-8中文字符通常佔3個字節，顯示寬度為2
            if ((unsigned char)str[i] >= 0xE0)
            {
                width += 2; // 中文字符寬度
                i += 2;     // 跳過後續字節
            }
            else
            {
                width += 1;
                i += 1;
            }
        }
        else
        {
            width += 1; // ASCII字符寬度
        }
    }
    return width;
}

// 選擇遊戲模式
void selectGameMode()
{
    clearScreen();
    setColor(COLOR_TITLE);
    printf("\n");
    printf("    ╔══════════════════════════════════╗\n");
    printf("    ║           開始新遊戲             ║\n");
    printf("    ╚══════════════════════════════════╝\n\n");

    // 倒數動畫
    clearScreen();
    setColor(COLOR_YELLOW);
    printf("\n\n");
    printf("        準備開始遊戲！\n\n");

    const char *countdownFrames[] = {
        "     ╔═══════════════════════╗\n"
        "     ║          ３           ║\n"
        "     ║      ⚡ 準備 ⚡       ║\n"
        "     ╚═══════════════════════╝",

        "     ╔═══════════════════════╗\n"
        "     ║          ２           ║\n"
        "     ║      💫 預備 💫       ║\n"
        "     ╚═══════════════════════╝",

        "     ╔═══════════════════════╗\n"
        "     ║          １           ║\n"
        "     ║      🔥 瞄準 🔥       ║\n"
        "     ╚═══════════════════════╝",

        "     ╔═══════════════════════╗\n"
        "     ║       ✨ GO! ✨       ║\n"
        "     ║     開始你的表演！    ║\n"
        "     ╚═══════════════════════╝"};

    const int colors[] = {
        COLOR_RED,
        COLOR_YELLOW,
        COLOR_GREEN,
        COLOR_BLUE};

    for (int i = 0; i < 4; i++)
    {
        clearScreen();
        setColor(COLOR_TITLE);
        printf("\n\n");
        printf("             %s 模式\n\n", currentMode.name);

        setColor(colors[i]);
        printf("%s\n", countdownFrames[i]);

        if (i < 3)
        {
            Beep(800 + i * 200, 200);
        }
        else
        {
            Beep(1000, 100);
            Beep(1200, 100);
            Beep(1500, 100);
            Beep(2000, 200);
        }

        Sleep(800);
    }

    setColor(COLOR_DEFAULT);
    playSinglePlayerGame();
}

// 繪製主遊戲畫面（統一座標版本）
void drawMainGameScreen(Player *player)
{
    clearScreen();

    // 【Y=1-3】顯示頂部標題區域
    setColor(COLOR_TITLE);
    printf("\n");                                                      // Y=0
    printf("  ╔═════════════════════════════════════════════════╗\n"); // Y=1
    printf("  ║              打字炸彈 TYPING BOMB               ║\n"); // Y=2
    printf("  ╚═════════════════════════════════════════════════╝\n"); // Y=3

    // 【Y=4-10】顯示玩家狀態框架
    printf("\n");                                                        // Y=4
    printf("  ╔═════════════════════════════════════════════════╗\n");   // Y=5
    printf("  ║  玩家狀態                                       ║\n");   // Y=6
    printf("  ║  生命：                                         ║\n");   // Y=7 - 生命值顯示行
    printf("  ║  分數：         連擊：        最高連擊：        ║\n");   // Y=8 - 分數連擊顯示行
    printf("  ║  道具欄：                                       ║\n");   // Y=9 - 道具欄顯示行
    printf("  ╚═════════════════════════════════════════════════╝\n\n"); // Y=10

    // 【Y=12-14】進度條框架
    printf("  ╔═════════════════════════════════════════════════╗\n");   // Y=12
    printf("  ║                                                 ║\n");   // Y=13 - 進度條顯示行
    printf("  ╚═════════════════════════════════════════════════╝\n\n"); // Y=14

    // 【Y=16-20】目標單字和訊息合併框架（大框框）
    printf("  ╔═════════════════════════════════════════════════╗\n");   // Y=16
    printf("  ║                                                 ║\n");   // Y=17 - 目標單字顯示行
    printf("  ║                                                 ║\n");   // Y=18 - 分隔行
    printf("  ║                                                 ║\n");   // Y=19 - 訊息顯示行
    printf("  ╚═════════════════════════════════════════════════╝\n\n"); // Y=20

    // 【Y=22-25】輸入框架
    printf("  ╔═════════════════════════════════════════════════╗\n"); // Y=22
    printf("  ║  輸入：                                         ║\n"); // Y=23
    printf("  ║  ➜ [                                         ]  ║\n"); // Y=24 - 輸入顯示行
    printf("  ╚═════════════════════════════════════════════════╝\n"); // Y=25

    // 【Y=27-28】操作提示區域
    setColor(COLOR_DEFAULT);
    printf("\n      Enter確認 | Backspace刪除 | ESC返回主選單\n"); // Y=27

    // 初始化顯示
    updatePlayerStats(player);
}

// 玩家統計信息（使用固定座標避免衝突）
void updatePlayerStats(Player *player)
{
    // 【Y=7, X=12】更新生命值顯示
    gotoxy(12, 7);
    setColor(COLOR_HEART);
    for (int i = 0; i < player->lives; i++)
        printf("❤ ");
    for (int i = player->lives; i < 3; i++)
        printf("♡ ");
    printf("                                ");

    // 【Y=8】更新分數、連擊、最高連擊
    gotoxy(11, 8);
    setColor(COLOR_YELLOW);
    printf("%-8d", player->score);

    gotoxy(26, 8);
    setColor(COLOR_GREEN);
    printf("%-8d", player->combo);

    gotoxy(44, 8);
    setColor(COLOR_BLUE);
    printf("%-8d", player->maxCombo);

    // 【Y=9, X=14】道具欄顯示
    gotoxy(14, 9);
    setColor(COLOR_ITEM);
    if (player->itemCount > 0)
    {
        for (int i = 0; i < player->itemCount; i++)
        {
            printf("%s ", player->activeItems[i]->symbol);
        }
        int spaces = 31 - player->itemCount * 2;
        for (int i = 0; i < spaces; i++)
            printf(" ");
    }
    else
    {
        printf("無");
        printf("%-29s", "");
    }

    setColor(COLOR_DEFAULT);
}

// 進度條（修正座標到Y=13）
void updateProgressBar(double timeLeft, double timeoutSec)
{
    gotoxy(4, 13);           // 【Y=13, X=4】
    const int barWidth = 41; // 進度條寬度
    int progress = (int)((timeLeft / timeoutSec) * barWidth);

    // 🔊 【音效標記 1】時間警告音效（只在特定時刻播放避免過於頻繁）
    static int lastWarningTime = -1;
    int currentWarningTime = (int)timeLeft;

    if (timeLeft <= 3.0 && timeLeft > 0 && currentWarningTime != lastWarningTime)
    {
        if (currentWarningTime == 3 || currentWarningTime == 2 || currentWarningTime == 1)
        {
            // ⚠️ 播放警告音效：剩餘3/2/1秒時的緊急提示音
            playWarningSound();
            lastWarningTime = currentWarningTime;
        }
    }

    // 重置警告時間當新回合開始時
    if (timeLeft > timeoutSec * 0.9)
    {
        lastWarningTime = -1;
    }

    for (int i = 0; i < barWidth; i++)
    {
        if (i < progress)
        {
            if (timeLeft > timeoutSec * 0.6)
                setColor(COLOR_GREEN);
            else if (timeLeft > timeoutSec * 0.3)
                setColor(COLOR_YELLOW);
            else
                setColor(COLOR_RED);
            printf("█");
        }
        else
        {
            setColor(COLOR_DEFAULT);
            printf("░");
        }
    }
    setColor(COLOR_DEFAULT);
    printf(" %.1f秒 ", timeLeft);
}

// 目標單字（Y=17）
void updateTargetWord(const char *target)
{
    gotoxy(3, 17); // 【Y=17, X=4】
    int targetLen = strlen(target);
    int padding = (47 - targetLen - 9) / 2;

    printf("                                               ");
    gotoxy(3, 17);

    for (int i = 0; i < padding; i++)
        printf(" ");
    setColor(COLOR_YELLOW);
    printf("目標單字：%s", target);
    setColor(COLOR_DEFAULT);
    int rightPadding = 47 - padding - 9 - targetLen;
    for (int i = 0; i < rightPadding; i++)
        printf(" ");
}

// 訊息框（Y=19）
void updateMessage(const char *message, int color)
{
    gotoxy(3, 19); // 【Y=19, X=3】
    printf("                                               ");

    // 使用修正的寬度計算函數
    int displayWidth = getDisplayWidth(message);
    int padding = (49 - displayWidth) / 2;

    // 確保padding不為負數
    if (padding < 0)
        padding = 0;

    gotoxy(3, 19);
    setColor(color);

    // 添加左側空白
    for (int i = 0; i < padding; i++)
        printf(" ");

    printf("%s", message);

    // 添加右側空白以填滿整行
    int rightPadding = 49 - padding - displayWidth;
    if (rightPadding > 0)
    {
        for (int i = 0; i < rightPadding; i++)
            printf(" ");
    }

    setColor(COLOR_DEFAULT);
}

// 清空輸入框（Y=24）
void clearInputArea()
{
    gotoxy(10, 24); // 【Y=24, X=10】
    printf("                                     ");
    gotoxy(10, 24);
}

// 顯示輸入文字（修正到Y=24）
void updateInputDisplay(const char *input)
{
    gotoxy(10, 24); // 【Y=24, X=10】
    setColor(COLOR_GREEN);
    printf("%s", input);
    setColor(COLOR_DEFAULT);
}

// 清空目標單字和訊息區域
void clearTargetAndMessage()
{
    gotoxy(4, 17);
    printf("                                               ");
    gotoxy(4, 19);
    printf("                                               ");
    clearInputArea();
}
// 道具詢問（安全的字符串處理）
void askToUseItems(Player *player)
{
    if (player->itemCount == 0)
        return;

    // 安全的字符串組合
    char itemMsg[150]; // 增大緩衝區
    strcpy(itemMsg, "可用道具:");

    for (int i = 0; i < player->itemCount && i < 3; i++)
    {
        char temp[30];
        snprintf(temp, sizeof(temp), "%d.%s%s ", i + 1,
                 player->activeItems[i]->symbol,
                 player->activeItems[i]->name);

        // 檢查緩衝區空間
        if (strlen(itemMsg) + strlen(temp) < sizeof(itemMsg) - 1)
        {
            strcat(itemMsg, temp);
        }
    }

    updateMessage(itemMsg, COLOR_ITEM);
    Sleep(1500);

    // 3秒倒計時詢問
    for (int countdown = 3; countdown > 0; countdown--)
    {
        char promptMsg[80];
        snprintf(promptMsg, sizeof(promptMsg),
                 "按1~%d使用道具 (%d秒) ESC跳過",
                 player->itemCount, countdown);
        updateMessage(promptMsg, COLOR_YELLOW);

        time_t startTime = time(NULL);
        while (difftime(time(NULL), startTime) < 1.0)
        {
            if (_kbhit())
            {
                int key = _getch();

                if (key == 27) // ESC鍵
                {
                    updateMessage("跳過道具使用", COLOR_DEFAULT);
                    // 🔊 【音效標記 2】ESC退出音效：道具選擇階段按ESC跳過
                    playExitSound();
                    Sleep(500);
                    return;
                }
                else if (key >= '1' && key <= '9')
                {
                    int itemIndex = key - '1';
                    if (itemIndex < player->itemCount)
                    {
                        char selectedMsg[80];
                        snprintf(selectedMsg, sizeof(selectedMsg),
                                 "選擇: %s%s",
                                 player->activeItems[itemIndex]->symbol,
                                 player->activeItems[itemIndex]->name);
                        updateMessage(selectedMsg, COLOR_GREEN);
                        // 🔊 【音效標記 3】確認音效：選擇道具時的確認提示音
                        playConfirmSound();
                        Sleep(800);

                        useItem(player, itemIndex);
                        updatePlayerStats(player);
                        return;
                    }
                    else
                    {
                        updateMessage("❌ 無效選擇，跳過道具使用", COLOR_RED);
                        // 🔊 【音效標記 4】錯誤音效：無效道具選擇時的錯誤提示
                        playErrorSound();
                        Sleep(800);
                        return;
                    }
                }
                else
                {
                    updateMessage("跳過道具使用", COLOR_DEFAULT);
                    Sleep(500);
                    return;
                }
            }
            Sleep(50);
        }
    }

    updateMessage("⏳ 時間到，準備開始遊戲...", COLOR_DEFAULT);
    Sleep(600);
}

// 回合開始倒計時
void showRoundStartCountdown(int round)
{
    clearTargetAndMessage();

    char roundMsg[50];
    sprintf(roundMsg, "第 %d 回合", round);
    updateMessage(roundMsg, COLOR_TITLE);
    Sleep(800);

    if (player1.itemCount > 0)
    {
        askToUseItems(&player1);
    }

    for (int i = 3; i > 0; i--)
    {
        char countMsg[20];
        sprintf(countMsg, "%d", i);
        updateMessage(countMsg, COLOR_YELLOW);
        Beep(800, 150);
        Sleep(400);
    }

    updateMessage("開始！", COLOR_GREEN);
    Beep(1000, 200);
    Sleep(300);

    clearTargetAndMessage();
}

// 單人遊戲主循環
void playSinglePlayerGame()
{
    Player *currentPlayer = &player1;

    // 重新初始化玩家資料
    currentPlayer->lives = currentMode.initialLives;
    currentPlayer->score = 0;
    currentPlayer->combo = 0;
    currentPlayer->maxCombo = 0;
    currentPlayer->itemCount = 0;
    currentPlayer->hasShield = 0;

    // 初始化道具效果控制欄位
    currentPlayer->scoreDoubleNext = 0;
    currentPlayer->extraTime = 0.0;
    currentPlayer->baseTimeLimit = currentMode.timeLimit;
    currentPlayer->baseScorePerWord = currentMode.scorePerWord;

    int round = 1;
    double currentTime = currentMode.timeLimit;

    // 遊戲開始前的準備動畫
    showGameStartAnimation();

    // 只繪製一次主遊戲畫面
    drawMainGameScreen(currentPlayer);

    // 主遊戲循環
    while (currentPlayer->lives > 0)
    {
        // 每回合開始時重置道具效果
        resetItemEffects(currentPlayer);

        // 顯示回合開始倒數
        showRoundStartCountdown(round);

        // 執行回合
        int result = playRound(&currentTime, currentPlayer);

        // 處理回合結果
        if (result == -1) // 用戶按ESC退出
        {
            return; // 直接返回，不進入遊戲結束流程
        }
        else if (!result) // 遊戲正常結束
        {
            handleGameOver(currentPlayer, TRUE);
            break;
        }

        // 檢查是否獲得道具
        if (rand() % 100 < ITEM_CHANCE)
        {
            giveRandomItem(currentPlayer);
        }

        // 節奏控制：根據玩家表現調整難度曲線
        adjustDifficultyCurve(currentPlayer, &currentTime, round);

        // 顯示回合完成訊息
        char completeMsg[50];
        sprintf(completeMsg, "回合 %d 完成！", round);
        updateMessage(completeMsg, COLOR_GREEN);
        playRoundCompleteSound(); // 🔊 【音效標記 5】回合完成音效：每回合成功完成時的成就感音效
        Sleep(800);

        round++;
    }

    // 遊戲結束，儲存分數
    saveScore(currentPlayer->name, currentPlayer->score, currentPlayer->maxCombo, currentMode.name);
}

// 回合遊戲邏輯
int playRound(double *currentTime, Player *player)
{
    const char *target = getRandomWord();
    char input[MAX_WORD_LEN];

    // 更新目標單字
    updateTargetWord(target);
    updateMessage("開始輸入！", COLOR_DEFAULT);

    // 計算本回合實際時間限制（包含道具效果）
    double actualTime = *currentTime + player->extraTime;

    // 直接進入輸入階段
    int success = timedInput(input, MAX_WORD_LEN, actualTime, target, player);

    // 處理結果
    if (success == -1) // 用戶按了ESC鍵
    {
        updateMessage("遊戲已暫停，返回主選單...", COLOR_YELLOW);
        Sleep(1000);
        return -1; // 特殊返回值表示用戶退出
    }
    else if (!success) // 時間到
    {
        // 時間到 - 炸彈爆炸
        updateMessage("⏰ 時間到！炸彈爆炸了！", COLOR_RED);
        Sleep(1500);

        // 檢查是否有護盾
        if (player->hasShield)
        {
            player->hasShield = 0;
            updateMessage("🛡️ 防護罩抵擋了爆炸！", COLOR_BLUE);
            updatePlayerStats(player);
            // 🔊 【音效標記 6】護盾音效：護盾抵擋爆炸時的魔法音階
            playShieldSound();
            Sleep(1500);
            return 1;
        }

        // 生命值減少
        player->lives--;
        player->combo = 0;
        updatePlayerStats(player);
        playFailSound();

        if (player->lives > 0)
        {
            char lifeMsg[50];
            sprintf(lifeMsg, "繼續挑戰！剩餘生命：%d", player->lives);
            updateMessage(lifeMsg, COLOR_YELLOW);
            Sleep(1500);
            return 1;
        }

        // 遊戲結束
        updateMessage("遊戲結束！", COLOR_RED);
        Sleep(2000);
        return 0;
    }

    // 檢查輸入是否正確
    if (strcmp(input, target) == 0)
    {
        // 輸入正確
        updateMessage("✅ 成功拆除炸彈！", COLOR_GREEN);

        // 計算得分（包含道具效果）
        double scoreMultiplier = 1.0 + player->combo * 0.1;
        int baseScore = (int)(player->baseScorePerWord * scoreMultiplier);

        // 應用雙倍分數效果（如果有的話）
        int earnedScore = applyScoreDouble(player, baseScore);

        player->score += earnedScore;
        player->combo++;

        if (player->combo > player->maxCombo)
        {
            player->maxCombo = player->combo;
        }

        // 即時更新玩家狀態
        updatePlayerStats(player);

        // 顯示得分和連擊信息
        if (player->combo >= 3)
        {
            char comboMsg[50];
            sprintf(comboMsg, "🔥 %d 連擊！得分 +%d", player->combo, earnedScore);
            updateMessage(comboMsg, COLOR_GREEN);
            playComboSound();
        }
        else
        {
            char scoreMsg[50];
            sprintf(scoreMsg, "得分 +%d！", earnedScore);
            updateMessage(scoreMsg, COLOR_GREEN);
            playSuccessSound();
        }

        // 如果使用了雙倍分數，顯示特殊提示
        if (earnedScore > baseScore)
        {
            Sleep(800);
            updateMessage("💎 雙倍分數生效！", COLOR_ITEM);
            playBonusSound(); // 🔊 【音效標記 7】獎勵音效：雙倍分數生效時的歡快旋律
            Sleep(800);
        }

        Sleep(1200);
        return 1;
    }
    else
    {
        // 輸入錯誤
        char errorMsg[50];
        sprintf(errorMsg, "❌ 錯誤！正確答案：%s", target);
        updateMessage(errorMsg, COLOR_RED);
        Sleep(1500);

        // 檢查是否有護盾
        if (player->hasShield)
        {
            player->hasShield = 0;
            updateMessage("🛡️ 防護罩抵擋了爆炸！", COLOR_BLUE);
            updatePlayerStats(player);
            playShieldSound(); // 🔊 【音效標記 8】護盾音效：護盾抵擋錯誤答案爆炸時的魔法音階
            Sleep(1500);
            return 1;
        }

        // 生命值減少
        player->lives--;
        player->combo = 0;
        updatePlayerStats(player);
        playFailSound();

        if (player->lives > 0)
        {
            char lifeMsg[50];
            sprintf(lifeMsg, "繼續挑戰！剩餘生命：%d", player->lives);
            updateMessage(lifeMsg, COLOR_YELLOW);
            Sleep(1500);
            return 1;
        }

        // 遊戲結束
        updateMessage("遊戲結束！", COLOR_RED);
        Sleep(2000);
        return 0;
    }
}

// 限時輸入函數
int timedInput(char *buffer, int maxLen, double timeoutSec, const char *target, Player *player)
{
    int chars = 0;
    int c;
    time_t start = time(NULL);
    buffer[0] = '\0';
    double timeLeft = timeoutSec;

    // 清空輸入區域並設置初始光標位置
    clearInputArea();

    // 清空輸入緩衝區
    while (_kbhit())
    {
        _getch();
    }
    // 輸入循環
    while (1)
    {
        // 計算剩餘時間
        timeLeft = timeoutSec - difftime(time(NULL), start);

        // 檢查時間是否用完
        if (timeLeft <= 0)
        {
            // 先顯示0秒再返回
            timeLeft = 0.0;
            updateProgressBar(timeLeft, timeoutSec);
            playTimeoutSound(); // 🔊 【音效標記 9】超時音效：時間到時的低沉警告音
            Sleep(500);         // 讓玩家看到0秒顯示
            return 0;
        }

        // 更新進度條
        updateProgressBar(timeLeft, timeoutSec);

        // 處理用戶輸入
        if (_kbhit())
        {
            c = _getch();

            if (c == 13) // Enter 鍵
            {
                buffer[chars] = '\0';
                playConfirmSound(); // 🔊 【音效標記 10】確認音效：按Enter確認輸入時的清脆提示音
                return 1;
            }
            else if (c == 27) // ESC 鍵
            {
                playExitSound(); // 🔊 【音效標記 11】退出音效：遊戲中按ESC退出時的下降音階
                return -1;       // 返回-1表示用戶選擇退出
            }
            else if (c == 8) // 退格鍵
            {
                if (chars > 0)
                {
                    chars--;
                    buffer[chars] = '\0';
                    clearInputArea();
                    updateInputDisplay(buffer);
                    playDeleteSound(); // 🔊 【音效標記 12】刪除音效：按Backspace刪除字符時的短促低音
                }
            }
            else if (c >= 'a' && c <= 'z' && chars < maxLen - 1 && chars < 35)
            {
                // 只接受小寫字母，限制輸入長度
                buffer[chars] = c;
                chars++;
                buffer[chars] = '\0';
                updateInputDisplay(buffer);

                playTypingSound(); // 🔊 【音效標記 13】打字音效：輸入小寫字母時的清脆短音
            }
            else if (c >= 'A' && c <= 'Z' && chars < maxLen - 1 && chars < 35)
            {
                // 自動轉換大寫為小寫
                buffer[chars] = c + 32; // 轉換為小寫
                chars++;
                buffer[chars] = '\0';
                updateInputDisplay(buffer);

                playTypingSound(); // 🔊 【音效標記 14】打字音效：輸入大寫字母時的清脆短音
            }
        }

        // 短暫休眠避免CPU占用過高
        Sleep(10);
    }
}

// 根據玩家表現動態調整難度曲線
void adjustDifficultyCurve(Player *player, double *currentTime, int round)
{
    // 基本難度調整
    if (round % currentMode.upgradeScore == 0 && *currentTime > currentMode.minTime)
    {
        *currentTime -= currentMode.timeStep;
        if (*currentTime < currentMode.minTime)
        {
            *currentTime = currentMode.minTime;
        }

        // 在訊息框顯示升級提示
        char upgradeMsg[50];
        sprintf(upgradeMsg, "📈 難度提升！時間限制：%.1f秒", *currentTime);
        updateMessage(upgradeMsg, COLOR_BLUE);
        playLevelUpSound(); // 🔊 【音效標記 15】升級音效：難度提升時的進步音階
        Sleep(1500);
    }

    // 動態難度調整
    if (player->combo >= 5)
    {
        *currentTime *= 0.97;
        if (*currentTime < currentMode.minTime)
        {
            *currentTime = currentMode.minTime;
        }

        // 高連擊獎勵
        if (player->combo >= 10 && rand() % 100 < 20)
        {
            giveRandomItem(player);
        }
    }
    else if (player->lives == 1 && round > 5)
    {
        *currentTime *= 1.05;
        if (*currentTime > currentMode.timeLimit)
        {
            *currentTime = currentMode.timeLimit;
        }
    }

    if (*currentTime < currentMode.minTime * 0.8)
    {
        *currentTime = currentMode.minTime;
    }
}

// 遊戲結束後的恢復與轉場
void handleGameOver(Player *player, BOOL showAnimation)
{
    // 清空輸入區域和進度條
    clearInputArea();
    gotoxy(4, 13);
    printf("                                         ");

    if (showAnimation)
    {
        // 播放爆炸動畫
        showExplosionAnimation();
    }

    // 快速進入選項選單
    showGameOver(player);
}

// 獲取隨機單字
const char *getRandomWord()
{
    int index = rand() % wordCount;
    return wordBank[index];
}