#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "doomdef.h"
#include "doomstat.h"
#include "d_main.h"
#include "m_argv.h"
#include "g_game.h"
#include "w_wad.h"
#include "z_zone.h"
#include "v_video.h"
#include "r_local.h"
#include "i_video.h"
#include "hu_stuff.h"
#include "st_stuff.h"

#define DOOM_DEBUG

#ifdef DOOM_DEBUG
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) do {} while(0)
#endif

extern int myargc;
extern char** myargv;
extern boolean nodrawers;
extern byte* screens[5];
extern gamestate_t gamestate;
extern player_t players[MAXPLAYERS];
extern int consoleplayer;
extern int gametic;

void D_Display(void);

static byte* palette = NULL;

void save_frame_ppm(int frame_number) {
    if (!palette) palette = W_CacheLumpName("PLAYPAL", PU_CACHE);

    if (!screens[0]) {
        printf("ERROR: screens[0] is NULL\n");
        return;
    }

    char filename[256];
    sprintf(filename, "frames/frame%04d.ppm", frame_number);

    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Error: Could not open %s for writing\n", filename);
        return;
    }

    fprintf(f, "P6\n%d %d\n255\n", SCREENWIDTH, SCREENHEIGHT);

    byte rgb[SCREENWIDTH * 3];
    for (int y = 0; y < SCREENHEIGHT; y++) {
        for (int x = 0; x < SCREENWIDTH; x++) {
            byte index = screens[0][y * SCREENWIDTH + x];
            rgb[x * 3 + 0] = palette[index * 3 + 0];
            rgb[x * 3 + 1] = palette[index * 3 + 1];
            rgb[x * 3 + 2] = palette[index * 3 + 2];
        }
        fwrite(rgb, 3, SCREENWIDTH, f);
    }

    fclose(f);
    DEBUG_PRINT("Saved frame %d to %s", frame_number, filename);
}

void print_game_info(void) {
    printf("\n=== Doom Headless Test ===\n");
    printf("Game State: %d\n", gamestate);
    printf("Game Tic: %d\n", gametic);
    if (players[consoleplayer].mo) {
        printf("Player X: %d, Y: %d, Z: %d\n", 
               players[consoleplayer].mo->x >> FRACBITS,
               players[consoleplayer].mo->y >> FRACBITS,
               players[consoleplayer].mo->z >> FRACBITS);
    }
    printf("=========================\n\n");
}

int main(int argc, char** argv) {
    printf("Doom Headless Sandbox Test\n===========================\n\n");

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
        return 1;
    }

    mkdir("frames", 0755);
    nodrawers = false;

    printf("\nInitializing Doom engine...\n");
    D_DoomMain();
    printf("Doom initialization complete!\n");

    if (gamestate != GS_LEVEL) {
        printf("Starting new level...\n");
        G_InitNew(startskill, startepisode, startmap);
        gamestate = GS_LEVEL;
    }

    int wait_tics = 0;
    while (!players[consoleplayer].mo && wait_tics < 100) {
        G_Ticker();
        D_Display();
        gametic++;
        wait_tics++;
    }

    if (!players[consoleplayer].mo) {
        printf("ERROR: Player did not spawn\n");
        return 1;
    }

    printf("Player spawned!\n");
    print_game_info();

    printf("Running 100 tics and saving frames...\n");
    for (int i = 0; i < 100; i++) {
        G_Ticker();
        D_Display();
        gametic++;
        save_frame_ppm(i);
        if (i % 35 == 0) print_game_info();
    }

    printf("Test completed successfully!\nFrames saved to frames/ directory\n");
    return 0;
}
