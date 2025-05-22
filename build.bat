@echo off
chcp 65001
echo 正在編譯打字炸彈遊戲...
gcc -o typing_bomb main.c game.c ui.c player.c items.c scoreboard.c animation.c -lwinmm
if %errorlevel% == 0 (
    echo 編譯成功！執行檔：typing_bomb.exe
) else (
    echo 編譯失敗，錯誤代碼：%errorlevel%
    pause
)