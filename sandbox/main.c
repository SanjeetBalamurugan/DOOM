#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

#include "doom_lib.h"

int main(int argc, char** argv) {
    printf("Doom Headless Sandbox - Automated Gameplay Demo\n");
    printf("================================================\n\n");

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
    printf("Starting automated gameplay demo:\n");
    printf("- Moving forward\n");
    printf("- Turning left and right\n");
    printf("- Shooting\n");
    printf("- Saving 500 frames\n\n");
    
    DOOM_KeyDown(DOOM_KEY_UPARROW);
    
    int total_frames = 500;
    int shoot_interval = 20;
    int turn_cycle = 70;
    
    for (int i = 0; i < total_frames; i++) {
        float angle = (float)i / turn_cycle * 2.0f * 3.14159f;
        int mouse_x = (int)(sin(angle) * 8.0f);
        
        if (mouse_x != 0) {
            DOOM_MouseMove(mouse_x, 0);
        }
        
        if (i % shoot_interval == 0) {
            DOOM_KeyDown(DOOM_KEY_RCTRL);
        }
        if (i % shoot_interval == 2) {
            DOOM_KeyUp(DOOM_KEY_RCTRL);
        }
        
        if ((i / 100) % 2 == 0 && (i % 100) < 50) {
            if ((i % 10) == 0) {
                DOOM_KeyDown(DOOM_KEY_RALT);
                DOOM_KeyDown(DOOM_KEY_RIGHTARROW);
            }
            if ((i % 10) == 5) {
                DOOM_KeyUp(DOOM_KEY_RIGHTARROW);
                DOOM_KeyUp(DOOM_KEY_RALT);
            }
        }
        
        DOOM_RunTic();
        DOOM_RenderFrame();
        
        char filename[256];
        sprintf(filename, "frames/frame%04d.png", i);
        DOOM_SaveFramePNG(filename);
        
        if (i % 35 == 0) {
            int seconds = i / 35;
            printf("Second %2d | Tic %3d | GameTic: %d | LevelTime: %d | Mouse: %+3d\n", 
                   seconds, i, DOOM_GetGameTic(), DOOM_GetLevelTime(), mouse_x);
        }
    }
    
    DOOM_KeyUp(DOOM_KEY_UPARROW);

    printf("\nDemo completed!\n");
    printf("%d PNG frames saved to frames/ directory\n", total_frames);
    printf("\nTo create a video from frames, run:\n");
    printf("  ffmpeg -framerate 35 -i frames/frame%%04d.png -c:v libx264 -pix_fmt yuv420p doom_demo.mp4\n");

    return 0;
}