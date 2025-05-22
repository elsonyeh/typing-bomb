// ui.c - 用戶界面相關函數（精簡版，只保留必要功能）
#include "ui.h"
#include "game.h"
#include "scoreboard.h"

// ========== 基礎UI功能 ==========

// 清屏
void clearScreen()
{
    system("cls");
}

// 設置控制台顏色
void setColor(int color)
{
    SetConsoleTextAttribute(hConsole, color);
}

// 設置光標位置
void gotoxy(int x, int y)
{
    COORD pos = {x, y};
    SetConsoleCursorPosition(hConsole, pos);
}

// 初始化控制台
void initConsole()
{
    // 獲取控制台句柄
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 設置控制台編碼為 UTF-8
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // 設置控制台標題
    SetConsoleTitleA("Typing Bomb Game");

    // 設置控制台字體（支持中文和表情符號）
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"NSimSun"); // 使用新宋體，好支持中文
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

    // 清屏
    clearScreen();
}

// ========== 選單界面功能 ==========

// 主選單
void showMainMenu()
{
    clearScreen();
    setColor(COLOR_TITLE);
    printf("\n");
    printf("    ╔══════════════════════════════════════════════╗\n");
    printf("    ║            打字炸彈 Typing Bomb              ║\n");
    printf("    ╚══════════════════════════════════════════════╝\n\n");

    setColor(COLOR_DEFAULT);
    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                1. 開始遊戲                   │\n");
    printf("    │                2. 查看排行榜                 │\n");
    printf("    │                3. 遊戲說明                   │\n");
    printf("    │                4. 退出遊戲                   │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    setColor(COLOR_BLUE);
    printf("    請選擇選項 (1-4): ");
    setColor(COLOR_DEFAULT);

    int choice;
    scanf("%d", &choice);
    getchar(); // 清除輸入緩衝區

    switch (choice)
    {
    case 1:
        showModeMenu();
        break;
    case 2:
        showScoreboard();
        break;
    case 3:
        showHelp();
        break;
    case 4:
        exit(0);
    default:
        printf("\n    ❌ 無效的選擇，請重試\n");
        Sleep(1000);
        showMainMenu();
    }
}

// 顯示遊戲模式選單
void showModeMenu()
{
    clearScreen();
    setColor(COLOR_TITLE);
    printf("\n");
    printf("    ╔══════════════════════════════════════════════╗\n");
    printf("    ║                 選擇遊戲難度                 ║\n");
    printf("    ╚══════════════════════════════════════════════╝\n\n");

    setColor(COLOR_GREEN);
    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                  1. 簡單模式                 │\n");
    printf("    │             • 初始時間: 7.0 秒               │\n");
    printf("    │             • 最短時間: 3.0 秒               │\n");
    printf("    │             • 生命值: 3                      │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    setColor(COLOR_YELLOW);
    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                  2. 中等模式                 │\n");
    printf("    │             • 初始時間: 5.0 秒               │\n");
    printf("    │             • 最短時間: 3.0 秒               │\n");
    printf("    │             • 生命值: 3                      │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    setColor(COLOR_RED);
    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                  3. 困難模式                 │\n");
    printf("    │             • 初始時間: 4.0 秒               │\n");
    printf("    │             • 最短時間: 2.0 秒               │\n");
    printf("    │             • 生命值: 3                      │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    setColor(COLOR_BLUE);
    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                 4. 返回主選單                │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    setColor(COLOR_DEFAULT);
    printf("    請選擇難度 (1-4): ");

    int choice;
    scanf("%d", &choice);
    getchar(); // 清除輸入緩衝區

    switch (choice)
    {
    case 1:
        currentMode = easy;
        break;
    case 2:
        currentMode = medium;
        break;
    case 3:
        currentMode = hard;
        break;
    case 4:
        showMainMenu();
        return;
    default:
        printf("\n    ❌ 無效的選擇，請重試\n");
        Sleep(1000);
        showModeMenu();
        return;
    }

    // 顯示確認訊息
    setColor(COLOR_BLUE);
    printf("\n    ┌──────────────────────────────────┐\n");
    printf("    │   已選擇 %s 模式               │\n", currentMode.name);
    printf("    │   按 Enter 開始遊戲...           │\n");
    printf("    │   按 Esc 重新選擇                │\n");
    printf("    └──────────────────────────────────┘\n");
    setColor(COLOR_DEFAULT);

    // 等待使用者按鍵
    while (1)
    {
        int key = getch();
        if (key == 13)
        { // Enter 鍵
            selectGameMode();
            break;
        }
        else if (key == 27)
        { // Esc 鍵
            showModeMenu();
            break;
        }
    }
}

// 遊戲說明
void showHelp()
{
    clearScreen();
    setColor(COLOR_TITLE);
    printf("\n");
    printf("    ╔══════════════════════════════════════════════╗\n");
    printf("    ║                   遊戲說明                   ║\n");
    printf("    ╚══════════════════════════════════════════════╝\n\n");

    setColor(COLOR_DEFAULT);
    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                   遊戲規則                   │\n");
    printf("    │          • 在時間內輸入顯示的單字            │\n");
    printf("    │          • 輸入正確可獲得分數                │\n");
    printf("    │          • 連續正確可獲得連擊加成            │\n");
    printf("    │          • 輸入錯誤或超時失去生命            │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                   道具系統                   │\n");
    printf("    │           🧪 治癒藥水 - 回復生命             │\n");
    printf("    │           ⌛ 時間延長 - 增加時間             │\n");
    printf("    │           💎 雙倍分數 - 得分加倍             │\n");
    printf("    │           🛡️ 防護罩 - 抵擋失敗               │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                   操作說明                   │\n");
    printf("    │           • 只能輸入小寫字母                 │\n");
    printf("    │           • 按 Enter 確認輸入                │\n");
    printf("    │           • 按 Backspace 刪除字符            │\n");
    printf("    │           • 道具只能在回合開始前使用         │\n");
    printf("    │           • 按 ESC 返回主選單                │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    setColor(COLOR_BLUE);
    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │             按 Enter 返回主選單              │\n");
    printf("    └──────────────────────────────────────────────┘\n");
    setColor(COLOR_DEFAULT);

    // 修改按鍵處理邏輯
    int key;
    while ((key = getch()) != 13)
        ; // 等待 Enter 鍵
    showMainMenu();
}

// 遊戲結束菜單
void showGameOver(Player *player)
{
    clearScreen();
    setColor(COLOR_TITLE);
    printf("\n");
    printf("    ╔══════════════════════════════════════════════╗\n");
    printf("    ║                   遊戲結束                   ║\n");
    printf("    ╚══════════════════════════════════════════════╝\n\n");

    setColor(COLOR_DEFAULT);
    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                   最終成績                   │\n");
    printf("    │    得分：");
    setColor(COLOR_YELLOW);
    printf("%-30d", player->score);
    setColor(COLOR_DEFAULT);
    printf("      │\n");
    printf("    │    最高連擊：");
    setColor(COLOR_GREEN);
    printf("%-26d", player->maxCombo);
    setColor(COLOR_DEFAULT);
    printf("      │\n");
    printf("    │    難度：");
    setColor(COLOR_BLUE);
    printf("%-31s", currentMode.name);
    setColor(COLOR_DEFAULT);
    printf("       │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    setColor(COLOR_BLUE);
    printf("    ┌──────────────────────────────────────────────┐\n");
    printf("    │                1. 儲存成績                   │\n");
    printf("    │                2. 重新開始                   │\n");
    printf("    │                3. 返回主選單                 │\n");
    printf("    │                4. 退出遊戲                   │\n");
    printf("    └──────────────────────────────────────────────┘\n\n");

    setColor(COLOR_DEFAULT);
    printf("    請選擇選項 (1-4): ");

    int choice;
    while (1)
    {
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ; // 清除輸入緩衝區
            printf("\n    ❌ 無效的選擇，請重試: ");
            continue;
        }
        getchar(); // 清除輸入緩衝區

        switch (choice)
        {
        case 1:
            // 輸入姓名並儲存成績
            clearScreen();
            setColor(COLOR_GREEN);
            printf("\n");
            printf("    ┌──────────────────────────────────┐\n");
            printf("    │    請輸入您的名稱 (2-12字元)     │\n");
            printf("    │          不可包含空格            │\n");
            printf("    └──────────────────────────────────┘\n\n");

            setColor(COLOR_DEFAULT);
            char tempName[MAX_NAME_LEN];
            while (1)
            {
                printf("    名稱：");
                if (scanf("%19s", tempName) != 1)
                {
                    while (getchar() != '\n')
                        ;
                    printf("    ❌ 輸入錯誤，請重試\n");
                    continue;
                }
                getchar();

                if (strlen(tempName) < 2 || strlen(tempName) > 12)
                {
                    printf("    ❌ 名稱長度必須在2-12字元之間\n");
                    continue;
                }

                int hasSpace = 0;
                for (int i = 0; tempName[i]; i++)
                {
                    if (isspace(tempName[i]))
                    {
                        hasSpace = 1;
                        break;
                    }
                }
                if (hasSpace)
                {
                    printf("    ❌ 名稱不可包含空格\n");
                    continue;
                }

                strcpy(player->name, tempName);
                break;
            }

            // 儲存分數並顯示排名
            saveScore(player->name, player->score, player->maxCombo, currentMode.name);
            int rank = getPlayerRank(player->name, player->score, currentMode.name);
            printf("    🏆 您在%s模式中排名第 %d 名！\n", currentMode.name, rank);
            printf("\n    按任意鍵返回主選單...");
            getch();
            showMainMenu();
            return;

        case 2:
            showModeMenu();
            return;
        case 3:
            showMainMenu();
            return;
        case 4:
            exit(0);
        default:
            printf("\n    ❌ 無效的選擇，請重試: ");
        }
    }
}