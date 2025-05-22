// sound_test_en.c - English Sound Test Program
#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main() {
    printf("=== SOUND SYSTEM TEST ===\n\n");
    
    // Test 1: Basic Beep
    printf("Test 1: Basic Beep Function\n");
    printf("Playing 1000Hz for 1 second...\n");
    printf("LISTEN CAREFULLY!\n");
    Sleep(1000);
    
    BOOL result = Beep(1000, 1000);  // 1 second beep
    
    if (result) {
        printf("OK: Beep function executed successfully\n");
    } else {
        printf("ERROR: Beep function failed, error code: %lu\n", GetLastError());
    }
    
    printf("\nDid you hear the sound? (Y/N): ");
    char heard = getchar();
    getchar(); // consume newline
    
    if (heard == 'Y' || heard == 'y') {
        printf("GREAT: Sound system is working!\n");
    } else {
        printf("PROBLEM: Sound system may have issues\n");
        printf("\nPossible solutions:\n");
        printf("1. Check system volume\n");
        printf("2. Run as Administrator\n");
        printf("3. Check Windows Audio service\n");
        printf("4. Try different audio device\n");
    }
    
    printf("\nTest 2: Multiple Beeps\n");
    printf("Playing sequence of beeps...\n");
    
    // Game sound simulation
    printf("Beep 1 (800Hz)...\n");
    Beep(800, 300);
    Sleep(200);
    
    printf("Beep 2 (1200Hz)...\n");
    Beep(1200, 300);
    Sleep(200);
    
    printf("Beep 3 (1500Hz)...\n");
    Beep(1500, 300);
    Sleep(200);
    
    printf("Sequence complete!\n");
    
    printf("\nTest 3: System Sounds\n");
    MessageBeep(MB_OK);
    printf("Playing system beep 1...\n");
    Sleep(1000);
    
    MessageBeep(MB_ICONASTERISK);
    printf("Playing system beep 2...\n");
    Sleep(1000);
    
    printf("\n=== TEST SUMMARY ===\n");
    printf("If you heard sounds, your system is OK.\n");
    printf("If no sounds, check:\n");
    printf("- System volume not muted\n");
    printf("- Audio drivers installed\n");
    printf("- Windows Audio service running\n");
    printf("- Run program as Administrator\n");
    
    printf("\nPress any key to exit...\n");
    getch();
    
    return 0;
}