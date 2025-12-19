#include "doom_lib.h"
#include "d_main.h"
#include "g_game.h"
#include "w_wad.h"
#include "z_zone.h"
#include "v_video.h"
#include "r_local.h"
#include "i_video.h"
#include "m_argv.h"
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

extern int myargc;
extern char** myargv;
extern boolean nodrawers;

void D_Display(void);

void DOOM_Init(int argc, char** argv) {
    myargc = argc;
    myargv = argv;
    
    nodrawers = false;
    
    D_DoomMain();
}

void DOOM_RunTic(void) {
    G_Ticker();
    gametic++;
}

void DOOM_RenderFrame(void) {
    D_Display();
}

byte* DOOM_GetScreenBuffer(void) {
    return screens[0];
}

int DOOM_GetScreenWidth(void) {
    return SCREENWIDTH;
}

int DOOM_GetScreenHeight(void) {
    return SCREENHEIGHT;
}

byte* DOOM_GetPalette(void) {
    return W_CacheLumpName("PLAYPAL", PU_CACHE);
}

boolean DOOM_IsPlayerReady(void) {
    return (players[consoleplayer].mo != NULL);
}

int DOOM_GetGameTic(void) {
    return gametic;
}

int DOOM_GetLevelTime(void) {
    return leveltime;
}

void DOOM_SaveFramePNG(const char* filename) {
    byte* screen = DOOM_GetScreenBuffer();
    byte* palette = DOOM_GetPalette();
    
    if (!screen || !palette) {
        printf("Error: Screen or palette is NULL\n");
        return;
    }
    
    int width = DOOM_GetScreenWidth();
    int height = DOOM_GetScreenHeight();
    
    unsigned char* rgb_data = (unsigned char*)malloc(width * height * 3);
    if (!rgb_data) {
        printf("Error: Failed to allocate RGB buffer\n");
        return;
    }
    
    for (int i = 0; i < width * height; i++) {
        byte index = screen[i];
        rgb_data[i * 3 + 0] = palette[index * 3 + 0];
        rgb_data[i * 3 + 1] = palette[index * 3 + 1];
        rgb_data[i * 3 + 2] = palette[index * 3 + 2];
    }
    
    if (!stbi_write_png(filename, width, height, 3, rgb_data, width * 3)) {
        printf("Error: Failed to write PNG %s\n", filename);
    }
    
    free(rgb_data);
}