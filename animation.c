// animation.c - 動畫和特效功能
#include "animation.h"
#include "ui.h"
#include <math.h>   // 添加數學庫支援
#include <stdlib.h> // 添加記憶體管理支援

// 華麗爆炸動畫版本1 - 多階段特效
void showExplosionAnimation()
{
    const int centerX = 25; // 畫面中心X
    const int centerY = 15; // 畫面中心Y

    // 第一階段：預警閃爍
    for (int i = 0; i < 3; i++)
    {
        clearScreen();
        setColor(COLOR_RED);

        // 警告框架
        gotoxy(5, 10);
        printf("  ╔═══════════════════════════════════════════╗");
        gotoxy(5, 11);
        printf("  ║                                           ║");
        gotoxy(5, 12);
        printf("  ║                ⚠️  警告  ⚠️               ║");
        gotoxy(5, 13);
        printf("  ║                                           ║");
        gotoxy(5, 14);
        printf("  ║               炸彈即將爆炸！              ║");
        gotoxy(5, 15);
        printf("  ║                                           ║");
        gotoxy(5, 16);
        printf("  ╚═══════════════════════════════════════════╝");

        Beep(1000, 150);
        Sleep(200);

        clearScreen();
        Sleep(100);
    }

    // 第二階段：初始爆炸
    const char *explosionCore[] = {
        "        💥        ",
        "      💥💥💥      ",
        "    💥💥💥💥💥    ",
        "  💥💥💥💥💥💥💥  ",
        "💥💥💥💥💥💥💥💥💥"};

    for (int frame = 0; frame < 5; frame++)
    {
        clearScreen();

        // 背景閃爍
        if (frame % 2 == 0)
        {
            setColor(COLOR_RED);
        }
        else
        {
            setColor(COLOR_YELLOW);
        }

        // 繪製爆炸核心
        gotoxy(centerX - 9, centerY - 2 + frame / 2);
        printf("%s", explosionCore[frame]);

        // 添加火花效果
        setColor(COLOR_ORANGE);
        for (int i = 0; i < 8; i++)
        {
            int sparkX = centerX + (rand() % 20 - 10);
            int sparkY = centerY + (rand() % 10 - 5);
            if (sparkX > 0 && sparkX < 70 && sparkY > 5 && sparkY < 25)
            {
                gotoxy(sparkX, sparkY);
                printf("*");
            }
        }

        // 震動效果
        Sleep(50);
        gotoxy(centerX - 9 + (rand() % 3 - 1), centerY - 2 + frame / 2 + (rand() % 3 - 1));
        setColor(COLOR_RED);
        printf("%s", explosionCore[frame]);

        Beep(800 - frame * 50, 100 + frame * 20);
        Sleep(80);
    }

    // 第三階段：衝擊波擴散
    clearScreen();
    for (int wave = 1; wave <= 8; wave++)
    {
        clearScreen();

        // 繪製衝擊波圓圈
        setColor((wave % 2 == 0) ? COLOR_YELLOW : COLOR_RED);

        for (int angle = 0; angle < 360; angle += 15)
        {
            double rad = angle * 3.14159 / 180.0;
            int x = centerX + (int)(wave * 3 * cos(rad));
            int y = centerY + (int)(wave * 1.5 * sin(rad));

            if (x > 2 && x < 50 && y > 5 && y < 25)
            {
                gotoxy(x, y);
                if (wave <= 3)
                    printf("█");
                else if (wave <= 5)
                    printf("▓");
                else
                    printf("░");
            }
        }

        // 中心持續爆炸
        setColor(COLOR_TITLE);
        gotoxy(centerX - 4, centerY);
        printf("💥BOOM💥");

        // 碎片四散
        setColor(COLOR_ORANGE);
        for (int debris = 0; debris < 15; debris++)
        {
            int debrisX = centerX + (rand() % (wave * 4) - wave * 2);
            int debrisY = centerY + (rand() % (wave * 2) - wave);

            if (debrisX > 3 && debrisX < 50 && debrisY > 6 && debrisY < 24)
            {
                gotoxy(debrisX, debrisY);
                char symbols[] = {'*', '+', 'x', '•', '◦'};
                printf("%c", symbols[rand() % 5]);
            }
        }

        Beep(600 - wave * 30, 60);
        Sleep(100);
    }

    // 第四階段：文字爆炸效果
    clearScreen();
    const char *boomText[] = {
        "██████╗  ██████╗  ██████╗ ███╗   ███╗",
        "██╔══██╗██╔═══██╗██╔═══██╗████╗ ████║",
        "██████╔╝██║   ██║██║   ██║██╔████╔██║",
        "██╔══██╗██║   ██║██║   ██║██║╚██╔╝██║",
        "██████╔╝╚██████╔╝╚██████╔╝██║ ╚═╝ ██║",
        "╚═════╝  ╚═════╝  ╚═════╝ ╚═╝     ╚═╝"};

    for (int line = 0; line < 6; line++)
    {
        setColor(COLOR_RED + (line % 3));
        gotoxy(8, 10 + line);

        // 逐字顯示效果
        for (int i = 0; boomText[line][i] != '\0'; i++)
        {
            printf("%c", boomText[line][i]);
            if (i % 3 == 0)
            {
                Beep(1200 + i * 10, 20);
                Sleep(15);
            }
        }
        Sleep(100);
    }

    // 第五階段：煙霧效果
    Sleep(500);
    for (int smoke = 0; smoke < 6; smoke++)
    {
        setColor(COLOR_DEFAULT + (smoke % 2));

        // 清除部分文字，創造煙霧消散效果
        for (int clear = 0; clear < 5; clear++)
        {
            int clearX = 8 + (rand() % 35);
            int clearY = 10 + (rand() % 6);
            gotoxy(clearX, clearY);
            printf(" ");
        }

        // 添加煙霧符號
        for (int i = 0; i < 20; i++)
        {
            int smokeX = centerX + (rand() % 20 - 10);
            int smokeY = centerY + (rand() % 8 - 4);

            if (smokeX > 5 && smokeX < 45 && smokeY > 8 && smokeY < 20)
            {
                gotoxy(smokeX, smokeY);
                char smokeChars[] = {'░', '▒', '▓', '~', '-'};
                printf("%c", smokeChars[rand() % 5]);
            }
        }

        Sleep(300);
    }

    // 第六階段：最終效果
    Sleep(500);
    clearScreen();
    setColor(COLOR_YELLOW);
    gotoxy(centerX - 10, centerY - 1);
    printf("🔥 遊戲結束！Game Over! 🔥");
    gotoxy(centerX - 8, centerY + 1);
    printf("  準備進入結算畫面...");

    // 最終音效
    for (int i = 0; i < 3; i++)
    {
        Beep(800, 100);
        Beep(600, 100);
        Beep(400, 200);
        Sleep(200);
    }

    Sleep(1000);
    setColor(COLOR_DEFAULT);
}

// 終極華麗爆炸動畫版本2.0 - 全新升級
void showExplosionAnimation2()
{
    const int centerX = 25;
    const int centerY = 15;

    // 第一階段：倒計時爆炸預警
    const char *countdown[] = {"３", "２", "１"};
    const int countColors[] = {COLOR_YELLOW, COLOR_ORANGE, COLOR_RED};

    for (int i = 0; i < 3; i++)
    {
        clearScreen();
        setColor(countColors[i]);

        // 危險邊框
        for (int y = 8; y < 20; y++)
        {
            gotoxy(10, y);
            printf("║");
            gotoxy(40, y);
            printf("║");
        }
        for (int x = 10; x <= 40; x++)
        {
            gotoxy(x, 8);
            printf("═");
            gotoxy(x, 19);
            printf("═");
        }

        // 倒計時數字
        gotoxy(centerX - 1, centerY - 2);
        printf("⚠️ 危險 ⚠️");
        gotoxy(centerX - 1, centerY);
        printf("    %s    ", countdown[i]);
        gotoxy(centerX - 3, centerY + 2);
        printf("炸彈引爆中...");

        // 閃爍效果
        for (int flash = 0; flash < 3; flash++)
        {
            Beep(1500 - i * 200, 100);
            Sleep(100);
            setColor(COLOR_DEFAULT);
            gotoxy(centerX - 1, centerY);
            printf("        ");
            Sleep(50);
            setColor(countColors[i]);
            gotoxy(centerX - 1, centerY);
            printf("    %s    ", countdown[i]);
        }
        Sleep(300);
    }

    // 第二階段：核心爆炸 - 多層同心圓
    clearScreen();
    for (int blast = 1; blast <= 12; blast++)
    {
        clearScreen();

        // 繪製多層爆炸圓環
        for (int ring = 1; ring <= blast && ring <= 8; ring++)
        {
            int color = (ring % 4) + 12; // 循環使用亮色
            setColor(color);

            // 繪製圓環
            for (int angle = 0; angle < 360; angle += 8)
            {
                double rad = angle * 3.14159 / 180.0;
                int x = centerX + (int)(ring * 2.5 * cos(rad));
                int y = centerY + (int)(ring * 1.2 * sin(rad));

                if (x > 3 && x < 47 && y > 6 && y < 22)
                {
                    gotoxy(x, y);
                    if (ring <= 2)
                        printf("⬢"); // 實心六角形
                    else if (ring <= 4)
                        printf("◆"); // 菱形
                    else if (ring <= 6)
                        printf("●"); // 圓點
                    else
                        printf("○"); // 空心圓
                }
            }
        }

        // 中心能量球
        setColor(COLOR_TITLE);
        gotoxy(centerX - 2, centerY - 1);
        printf("⚡💥⚡");
        gotoxy(centerX - 3, centerY);
        printf("💥BOOM💥");
        gotoxy(centerX - 2, centerY + 1);
        printf("⚡💥⚡");

        // 能量射線
        setColor(COLOR_YELLOW);
        for (int ray = 0; ray < 8; ray++)
        {
            double rayAngle = ray * 45 * 3.14159 / 180.0;
            for (int len = 1; len <= blast; len++)
            {
                int rayX = centerX + (int)(len * 2 * cos(rayAngle));
                int rayY = centerY + (int)(len * sin(rayAngle));

                if (rayX > 5 && rayX < 45 && rayY > 8 && rayY < 20)
                {
                    gotoxy(rayX, rayY);
                    printf("━");
                }
            }
        }

        // 隨機電光效果
        setColor(COLOR_BLUE);
        for (int lightning = 0; lightning < 25; lightning++)
        {
            int lx = centerX + (rand() % 16 - 8);
            int ly = centerY + (rand() % 8 - 4);
            if (lx > 8 && lx < 42 && ly > 10 && ly < 18)
            {
                gotoxy(lx, ly);
                printf("⚡");
            }
        }

        // 音效與節奏
        Beep(2000 - blast * 50, 50 + blast * 5);
        Sleep(120 - blast * 5);
    }

    // 第三階段：粒子爆發系統
    clearScreen();
    for (int particle_wave = 0; particle_wave < 15; particle_wave++)
    {
        clearScreen();

        // 粒子系統 - 四種不同類型的粒子
        const char *particles[] = {"★", "✦", "✧", "•", "◦", "∘", ".", "'"};
        const int particleColors[] = {COLOR_YELLOW, COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_PURPLE, COLOR_ORANGE};

        for (int p = 0; p < 80; p++)
        {
            setColor(particleColors[p % 6]);

            // 計算粒子位置（爆炸擴散模式）
            double velocity = (particle_wave + 1) * 0.8;
            double angle = (p * 4.5) * 3.14159 / 180.0;
            int px = centerX + (int)(velocity * cos(angle));
            int py = centerY + (int)(velocity * 0.6 * sin(angle));

            // 添加重力效果
            py += (particle_wave * particle_wave) / 30;

            if (px > 5 && px < 45 && py > 8 && py < 22)
            {
                gotoxy(px, py);
                printf("%s", particles[p % 8]);
            }
        }

        // 中心殘留火焰
        if (particle_wave < 8)
        {
            setColor(COLOR_RED + (particle_wave % 3));
            gotoxy(centerX - 1, centerY);
            printf("🔥");
        }

        // 螺旋煙霧效果
        setColor(COLOR_DEFAULT);
        for (int spiral = 0; spiral < particle_wave * 2; spiral++)
        {
            double spiralAngle = spiral * 0.5;
            int sx = centerX + (int)(spiral * 0.3 * cos(spiralAngle));
            int sy = centerY - spiral / 4 + (int)(2 * sin(spiralAngle));

            if (sx > 8 && sx < 42 && sy > 6 && sy < 20)
            {
                gotoxy(sx, sy);
                printf("~");
            }
        }

        Beep(1800 - particle_wave * 80, 40);
        Sleep(100);
    }

    // 第四階段：矩陣數字雨效果
    Sleep(300);
    for (int matrix = 0; matrix < 20; matrix++)
    {
        // 隨機清除部分內容
        for (int clear = 0; clear < 30; clear++)
        {
            gotoxy(rand() % 40 + 5, rand() % 12 + 8);
            printf(" ");
        }

        // 數字雨效果
        setColor(COLOR_GREEN + (matrix % 3));
        for (int col = 8; col < 42; col += 3)
        {
            for (int drop = 0; drop < matrix && drop < 15; drop++)
            {
                int row = 8 + drop;
                if (row < 20)
                {
                    gotoxy(col, row);
                    printf("%d", rand() % 10);
                }
            }
        }

        Sleep(150);
    }

    // 第五階段：文字重組特效
    clearScreen();
    const char *finalText[] = {
        "  ╔════════════════════════════════════╗",
        "  ║          💀 GAME OVER 💀          ║",
        "  ║                                    ║",
        "  ║     系統已被完全摧毀！！！       ║",
        "  ║                                    ║",
        "  ║    🔥 準備重新啟動系統... 🔥     ║",
        "  ╚════════════════════════════════════╝"};

    // 文字碎片化重組效果
    for (int line = 0; line < 7; line++)
    {
        setColor(COLOR_RED + (line % 4));

        // 碎片化顯示
        int textLen = strlen(finalText[line]);
        int *positions = malloc(textLen * sizeof(int));
        for (int i = 0; i < textLen; i++)
            positions[i] = i;

        // 隨機打亂位置
        for (int i = textLen - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            int temp = positions[i];
            positions[i] = positions[j];
            positions[j] = temp;
        }

        // 逐個字符出現
        for (int i = 0; i < textLen; i++)
        {
            gotoxy(10 + positions[i], 10 + line);
            printf("%c", finalText[line][positions[i]]);
            if (i % 5 == 0)
            {
                Beep(800 + i * 20, 30);
                Sleep(50);
            }
        }

        free(positions);
        Sleep(200);
    }

    // 第六階段：最終全屏閃爍
    for (int flash = 0; flash < 8; flash++)
    {
        clearScreen();

        if (flash % 2 == 0)
        {
            setColor(COLOR_RED);
            // 填滿整個畫面
            for (int y = 5; y < 25; y++)
            {
                gotoxy(0, y);
                for (int x = 0; x < 50; x++)
                {
                    printf("█");
                }
            }
        }
        else
        {
            setColor(COLOR_YELLOW);
            gotoxy(centerX - 8, centerY);
            printf("💥 TOTAL DESTRUCTION 💥");
        }

        Beep(flash % 2 ? 2000 : 500, 200);
        Sleep(200);
    }

    // 結束
    clearScreen();
    setColor(COLOR_DEFAULT);
    gotoxy(centerX - 6, centerY);
    printf("系統重啟中...");

    // 最終音效序列
    int finalBeeps[] = {800, 1000, 1200, 1500, 2000};
    for (int i = 0; i < 5; i++)
    {
        Beep(finalBeeps[i], 150);
        Sleep(100);
    }

    Sleep(1000);
}

// 遊戲開始動畫
void showGameStartAnimation()
{
    clearScreen();
    setColor(COLOR_TITLE);
    printf("\n\n\n");
    printf("           準備開始遊戲!\n\n");

    // 動態加載文字效果
    const char *loadingText = "TYPING BOMB";
    printf("            ");

    for (int i = 0; i < strlen(loadingText); i++)
    {
        // 使用預定義的顏色循環
        int colors[] = {COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_ITEM, COLOR_HEART};
        setColor(colors[i % 6]);
        printf("%c", loadingText[i]);
        Beep(500 + i * 50, 50); // 上升音效
        Sleep(100);
    }

    // 閃爍效果
    for (int i = 0; i < 3; i++)
    {
        Sleep(200);
        clearScreen();
        Sleep(100);
        printf("\n\n\n");
        printf("           準備開始遊戲!\n\n");
        printf("            %s", loadingText);
    }

    // 開始遊戲提示
    clearScreen();
    printf("\n\n\n\n");
    setColor(COLOR_GREEN);
    printf("               開始!");
    Beep(1200, 300);
    Sleep(300);

    setColor(COLOR_DEFAULT);
    clearScreen();
}