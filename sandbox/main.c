#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "doom_lib.h"

int main(int argc, char** argv) {
    printf("Doom Headless Sandbox - PNG Output\n");
    printf("===================================\n\n");

    int wad_found = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-iwad") == 0 && i + 1 < argc) {
            wad_found = 1;
            printf("Using IWAD: %s\n", argv[i + 1]);
            break;
        }
    }

    if (!wad_found) {
        printf("\nUsage: %s -iwad <path_to_doom.wad> -warp <ep> <map>\n", argv[0]);
        printf("\nExample:\n");
        printf("  %s -iwad doom.wad -warp 1 1\n", argv[0]);
        return 1;
    }

    mkdir("frames", 0755);

    printf("\nInitializing Doom engine...\n");
    DOOM_Init(argc, argv);

    printf("\nDoom initialization complete!\n");
    
    if (DOOM_GetScreenBuffer() == NULL) {
        printf("ERROR: Screen buffer not allocated!\n");
        return 1;
    }

    printf("Waiting for player to spawn...\n");
    
    int wait_tics = 0;
    while (!DOOM_IsPlayerReady() && wait_tics < 35) {
        DOOM_RunTic();
        wait_tics++;
    }
    
    if (!DOOM_IsPlayerReady()) {
        printf("ERROR: Player did not spawn\n");
        return 1;
    }
    
    printf("Player spawned!\n\n");
    printf("Running 100 tics and saving PNG frames...\n");
    
    for (int i = 0; i < 100; i++) {
        DOOM_RunTic();
        DOOM_RenderFrame();
        
        char filename[256];
        sprintf(filename, "frames/frame%04d.png", i);
        DOOM_SaveFramePNG(filename);
        
        if (i % 35 == 0) {
            printf("Tic %3d | GameTic: %d | LevelTime: %d | Saved: %s\n", 
                   i, DOOM_GetGameTic(), DOOM_GetLevelTime(), filename);
        }
    }

    printf("\nTest completed!\n");
    printf("100 PNG frames saved to frames/ directory\n");

    return 0;
}