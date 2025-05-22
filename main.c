// main.c - 程序入口點
#include "common.h"
#include "game.h"
#include "ui.h"

// 全域變數定義
const char *wordBank[] = {
    "elephant", "strawberry", "pineapple", "television", "adventure",
    "keyboard", "hamburger", "microscope", "university", "chocolate",
    "basketball", "foundation", "dictionary", "photograph", "revolution",
    "programming", "engineering", "mathematics", "technology", "understand"};

// 道具庫
Item items[] = {
    {HEAL, "治癒藥水", "🧪", 1, 0},
    {TIME_EXTEND, "時間延長", "⌛", 3, 0},
    {SCORE_DOUBLE, "雙倍分數", "💎", 5, 0},
    {SHIELD, "防護罩", "🛡️", 1, 0}};

int wordCount = sizeof(wordBank) / sizeof(wordBank[0]);
GameMode easy = {"簡單", 7.0, 3.0, 0.5, 4, 1.0, 3};
GameMode medium = {"中等", 5.0, 3.0, 0.5, 6, 1.5, 3};
GameMode hard = {"困難", 4.0, 2.0, 0.3, 6, 2.0, 3};
GameMode currentMode;
HANDLE hConsole;
GameType gameType;
Player player1, player2;

int main()
{
    srand((unsigned int)time(NULL));
    initConsole();

    // 測試音效是否工作
    printf("測試音效...\n");
    Beep(1000, 500);
    Sleep(800);

    // 🔧 修正：添加主循環，讓用戶可以重複回到主選單
    while (1)
    {
        showMainMenu();

        // 如果 showMainMenu 結束，詢問是否繼續
        clearScreen();
        setColor(COLOR_YELLOW);
        printf("\n\n    遊戲已結束！\n\n");
        setColor(COLOR_DEFAULT);
        printf("    是否重新開始？\n\n");
        printf("    Y - 返回主選單\n");
        printf("    N - 退出遊戲\n\n");
        printf("    請選擇 (Y/N): ");

        char choice = getch();
        choice = toupper(choice);

        if (choice == 'N')
        {
            break; // 退出主循環
        }
        // 如果是 Y 或其他鍵，繼續循環
    }

    // 程序結束
    clearScreen();
    setColor(COLOR_BLUE);
    printf("\n\n");
    printf("    ╔══════════════════════════════════════════════╗\n");
    printf("    ║                  謝謝遊玩！                  ║\n");
    printf("    ║                    再見！                    ║\n");
    printf("    ╚══════════════════════════════════════════════╝\n\n");
    setColor(COLOR_DEFAULT);

    Beep(800, 200);
    Beep(1000, 200);
    Beep(1200, 300);

    Sleep(2000);
    return 0;
}
