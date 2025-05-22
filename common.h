// common.h - 共用的定義、常量和結構
#ifndef COMMON_H
#define COMMON_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

// 確保 getch 和 kbhit 函數可用
#ifndef _getch
#define _getch getch
#endif

#ifndef _kbhit
#define _kbhit kbhit
#endif

// 常量定義
#define MAX_WORD_LEN 50
#define MAX_WORDS 50
#define MAX_NAME_LEN 20
#define MAX_SCORES 10
#define NUM_ITEMS 4
#define ITEM_CHANCE 30 // 30% 機率獲得道具
#define FRAME_WIDTH 51 // 框架寬度

// 顏色常量
#define COLOR_DEFAULT 7 // 白色
#define COLOR_BLUE 9    // 藍色
#define COLOR_GREEN 10  // 綠色
#define COLOR_RED 12    // 紅色
#define COLOR_YELLOW 14 // 黃色
#define COLOR_TITLE 15  // 亮白色
#define COLOR_HEART 13  // 粉紅色（用於愛心）
#define COLOR_ITEM 11   // 淺藍色（用於道具）
#define COLOR_ORANGE 6  // 橙色（用於特效）
#define COLOR_PURPLE 5  // 紫色（用於特效）

// 布爾類型定義
#ifndef BOOL
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

// 遊戲模式
typedef enum
{
    SINGLE_PLAYER,
} GameType;

// 道具類型
typedef enum
{
    HEAL,         // 回血
    TIME_EXTEND,  // 延長時間
    SCORE_DOUBLE, // 雙倍分數
    SHIELD        // 護盾（防止一次爆炸）
} ItemType;

// 道具結構
typedef struct
{
    ItemType type;
    char *name;
    char *symbol;
    int duration; // 持續回合數
    int active;   // 是否啟用
} Item;

// 玩家結構
typedef struct
{
    char name[MAX_NAME_LEN];
    int lives;
    int score;
    int combo;
    int maxCombo;
    Item *activeItems[3]; // 最多同時擁有3個道具
    int itemCount;
    int hasShield;           // 是否有護盾
    int scoreDoubleNext;     // 下次得分加倍標記
    double extraTime;        // 當前回合額外時間
    double baseTimeLimit;    // 基礎時間限制（用於重置）
    double baseScorePerWord; // 基礎分數（用於重置）
} Player;

// 排行榜記錄
typedef struct
{
    char name[MAX_NAME_LEN];
    int score;
    int maxCombo;
    char mode[10]; // 難度欄位
    int rank;      // 排名欄位
} ScoreRecord;

// 模式設定結構
typedef struct
{
    char name[10];
    double timeLimit;
    double minTime;
    double timeStep;
    int upgradeScore;
    double scorePerWord;
    int initialLives;
} GameMode;

// 全域變數 (在每個使用它們的C文件中使用extern聲明)
extern const char *wordBank[];
extern int wordCount;
extern GameMode easy, medium, hard, currentMode;
extern HANDLE hConsole;
extern GameType gameType;
extern Player player1, player2;
extern Item items[];

#endif // COMMON_H