
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doomdef.h"
#include "doomstat.h"
#include "d_main.h"
#include "m_argv.h"
#include "g_game.h"
#include "w_wad.h"

extern int myargc;
extern char** myargv;

void print_game_info(void) {
    printf("\n=== Doom Headless Test ===\n");
    printf("Game State: %d\n", gamestate);
    printf("Game Tic: %d\n", gametic);
    printf("Level Time: %d\n", leveltime);
    printf("=========================\n\n");
}

int main(int argc, char** argv) {
    printf("Doom Headless Sandbox Test\n");
    printf("===========================\n\n");

 
    myargc = argc;
    myargv = argv;

    
    int wad_found = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-iwad") == 0 && i + 1 < argc) {
            wad_found = 1;
            printf("Using IWAD: %s\n", argv[i + 1]);
            break;
        }
    }

    if (!wad_found) {
        printf("\nUsage: %s -iwad <path_to_doom.wad> [options]\n", argv[0]);
        printf("\nOptions:\n");
        printf("  -iwad <file>     Specify IWAD file (doom.wad, doom2.wad, etc.)\n");
        printf("  -warp <episode> <map>  Warp to specific level\n");
        printf("  -skill <1-5>     Set difficulty level\n");
        printf("  -nomonsters      No monsters\n");
        printf("  -devparm         Developer mode\n");
        printf("\nExample:\n");
        printf("  %s -iwad doom.wad -warp 1 1 -skill 3\n", argv[0]);
        return 1;
    }

    printf("\nInitializing Doom engine...\n");

    D_DoomMain();

    printf("\nDoom initialization complete!\n");
    
    printf("\nRunning 100 game tics...\n");
    for (int i = 0; i < 100; i++) {
        G_Ticker();
        
        if (i % 35 == 0) {
            print_game_info();
        }
    }

    printf("\nTest completed successfully!\n");
    printf("The Doom engine is running headless.\n");

    return 0;
}