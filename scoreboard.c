// scoreboard.c - 排行榜功能
#include "scoreboard.h"
#include "ui.h"

// 保存分數到文件
void saveScore(const char *name, int score, int maxCombo, const char *mode)
{
    // 檢查scores.txt是否存在，如果不存在則創建
    FILE *testFile = fopen("scores.txt", "r");
    if (!testFile)
    {
        testFile = fopen("scores.txt", "w");
        if (testFile)
        {
            fclose(testFile);
        }
        else
        {
            printf("\n    ❌ 無法創建分數檔案！\n");
            return;
        }
    }
    else
    {
        fclose(testFile);
    }

    // 讀取現有分數
    ScoreRecord scores[100];
    int count = 0;
    FILE *readFile = fopen("scores.txt", "r");
    if (readFile)
    {
        char line[256];
        while (count < 100 && fgets(line, sizeof(line), readFile))
        {
            line[strcspn(line, "\n")] = 0;
            if (sscanf(line, "%s %d %d %s",
                       scores[count].name,
                       &scores[count].score,
                       &scores[count].maxCombo,
                       scores[count].mode) == 4)
            {
                count++;
            }
        }
        fclose(readFile);
    }

    // 添加新分數
    if (count < 100)
    {
        strcpy(scores[count].name, name);
        scores[count].score = score;
        scores[count].maxCombo = maxCombo;
        strcpy(scores[count].mode, mode);
        count++;
    }

    // 按分數排序
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (scores[j].score < scores[j + 1].score)
            {
                ScoreRecord temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    // 寫入排序後的分數
    FILE *writeFile = fopen("scores.txt", "w");
    if (!writeFile)
    {
        printf("\n    ❌ 無法開啟分數檔案！\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(writeFile, "%s %d %d %s\n",
                scores[i].name,
                scores[i].score,
                scores[i].maxCombo,
                scores[i].mode);
    }

    fclose(writeFile);
    printf("\n    ✅ 成績儲存成功！\n");
}

// 獲取玩家排名
int getPlayerRank(const char *name, int score, const char *mode)
{
    ScoreRecord scores[100]; // 暫時存儲所有分數
    int count = 0;
    int rank = 1;

    // 讀取所有分數
    FILE *file = fopen("scores.txt", "r");
    if (file)
    {
        while (count < 100 &&
               fscanf(file, "%s %d %d %s", scores[count].name,
                      &scores[count].score, &scores[count].maxCombo,
                      scores[count].mode) == 4)
        {
            if (strcmp(scores[count].mode, mode) == 0) // 只比較相同難度
            {
                if (scores[count].score > score)
                {
                    rank++;
                }
            }
            count++;
        }
        fclose(file);
    }

    return rank;
}

// 顯示排行榜
void showScoreboard()
{
    ScoreRecord scores[100]; // 暫時存儲所有分數
    int count = 0;

    // 讀取分數
    FILE *file = fopen("scores.txt", "r");
    if (!file)
    {
        clearScreen();
        setColor(COLOR_RED);
        printf("\n    ❌ 無法開啟分數檔案！\n");
        printf("    按任意鍵返回主選單...\n");
        setColor(COLOR_DEFAULT);
        getch();
        showMainMenu();
        return;
    }

    char line[256];
    while (count < 100 && fgets(line, sizeof(line), file))
    {
        // 移除行尾的換行符
        line[strcspn(line, "\n")] = 0;

        // 使用sscanf更安全地解析每一行
        if (sscanf(line, "%s %d %d %s",
                   scores[count].name,
                   &scores[count].score,
                   &scores[count].maxCombo,
                   scores[count].mode) == 4)
        {
            count++;
        }
    }
    fclose(file);

    while (1)
    {
        clearScreen();
        setColor(COLOR_TITLE);
        printf("\n");
        printf("    ╔══════════════════════════════════════════════╗\n");
        printf("    ║                 🏆 排行榜 🏆                 ║\n");
        printf("    ╚══════════════════════════════════════════════╝\n\n");

        setColor(COLOR_DEFAULT);
        printf("    選擇難度：\n");
        printf("    1. 簡單 模式\n");
        printf("    2. 中等 模式\n");
        printf("    3. 困難 模式\n");
        printf("    4. 返回主選單\n\n");
        printf("    請選擇 (1-4): ");

        int choice;
        scanf("%d", &choice);
        getchar();

        if (choice == 4)
        {
            showMainMenu();
            return;
        }

        const char *selectedMode;
        switch (choice)
        {
        case 1:
            selectedMode = "簡單";
            break;
        case 2:
            selectedMode = "中等";
            break;
        case 3:
            selectedMode = "困難";
            break;
        default:
            continue;
        }

        // 篩選並排序該難度的分數
        ScoreRecord modeScores[100];
        int modeCount = 0;

        for (int i = 0; i < count; i++)
        {
            if (strcmp(scores[i].mode, selectedMode) == 0)
            {
                modeScores[modeCount++] = scores[i];
            }
        }

        // 氣泡排序（按分數降序）
        for (int i = 0; i < modeCount - 1; i++)
        {
            for (int j = 0; j < modeCount - i - 1; j++)
            {
                if (modeScores[j].score < modeScores[j + 1].score)
                {
                    ScoreRecord temp = modeScores[j];
                    modeScores[j] = modeScores[j + 1];
                    modeScores[j + 1] = temp;
                }
            }
        }

        clearScreen();
        setColor(COLOR_TITLE);
        printf("\n");
        printf("    ╔══════════════════════════════════════════════╗\n");
        printf("    ║            🏆 %s 模式排行榜 🏆             ║\n", selectedMode);
        printf("    ╚══════════════════════════════════════════════╝\n\n");

        if (modeCount == 0)
        {
            setColor(COLOR_DEFAULT);
            printf("    ┌──────────────────────────────────────────────┐\n");
            printf("    │            目前還沒有此難度的紀錄            │\n");
            printf("    └──────────────────────────────────────────────┘\n");
        }
        else
        {
            setColor(COLOR_DEFAULT);
            printf("    ┌──────────────────────────────────────────────┐\n");
            int displayCount = (modeCount < MAX_SCORES) ? modeCount : MAX_SCORES;
            for (int i = 0; i < displayCount; i++)
            {
                if (i == 0)
                    setColor(COLOR_YELLOW);
                else if (i == 1)
                    setColor(COLOR_DEFAULT);
                else if (i == 2)
                    setColor(COLOR_RED);
                else
                    setColor(COLOR_DEFAULT);

                char rank[8] = ""; // 增加緩衝區大小以容納表情符號
                switch (i)
                {
                case 0:
                    strcpy(rank, "🥇");
                    break;
                case 1:
                    strcpy(rank, "🥈");
                    break;
                case 2:
                    strcpy(rank, "🥉");
                    break;
                default:
                    sprintf(rank, "%d.", i + 1);
                    break;
                }

                setColor(COLOR_DEFAULT);
                printf("    │ ");

                // 設置排名顏色並輸出排名
                if (i == 0)
                    setColor(COLOR_YELLOW); // 金牌 - 黃色
                else if (i == 1)
                    setColor(COLOR_DEFAULT); // 銀牌 - 白色
                else if (i == 2)
                    setColor(COLOR_RED); // 銅牌 - 紅色
                else
                    setColor(COLOR_DEFAULT); // 其他 - 默認色

                printf("%-2s ", rank);

                // 輸出玩家姓名（保持當前顏色）
                printf("%-12s ", modeScores[i].name);

                // 分數部分使用不同顏色
                setColor(COLOR_GREEN);
                printf("%7d分   ", modeScores[i].score);

                // 連擊部分
                setColor(COLOR_BLUE);
                printf("連擊:%4d", modeScores[i].maxCombo);

                // 輸出邊框的右邊部分（恢復默認顏色）
                setColor(COLOR_DEFAULT);
                printf("        │\n");
            }

            // 底部邊框（確保是默認顏色）
            setColor(COLOR_DEFAULT);
            printf("    └──────────────────────────────────────────────┘\n");
        }

        setColor(COLOR_BLUE);
        printf("\n    按 Enter 返回難度選擇，ESC 返回主選單\n");
        setColor(COLOR_DEFAULT);

        int key;
        while ((key = getch()) != 13 && key != 27)
            ;
        if (key == 27)
        {
            showMainMenu();
            return;
        }
    }
}