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

extern int maketic;
extern ticcmd_t netcmds[MAXPLAYERS][BACKUPTICS];

void D_Display(void);
void D_PostEvent(event_t* ev);
void I_StartTic(void);
void D_ProcessEvents(void);
void G_BuildTiccmd(ticcmd_t* cmd);

void DOOM_Init(int argc, char** argv) {
    myargc = argc;
    myargv = argv;
    nodrawers = false;
    D_DoomMain();
}

void DOOM_RunTic(void) {
    I_StartTic();
    D_ProcessEvents();
    G_BuildTiccmd(&netcmds[consoleplayer][maketic % BACKUPTICS]);
    G_Ticker();
    gametic++;
    maketic++;
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
    int width = DOOM_GetScreenWidth();
    int height = DOOM_GetScreenHeight();
    unsigned char* rgb_data = (unsigned char*)malloc(width * height * 3);
    for (int i = 0; i < width * height; i++) {
        byte index = screen[i];
        rgb_data[i * 3 + 0] = palette[index * 3 + 0];
        rgb_data[i * 3 + 1] = palette[index * 3 + 1];
        rgb_data[i * 3 + 2] = palette[index * 3 + 2];
    }
    stbi_write_png(filename, width, height, 3, rgb_data, width * 3);
    free(rgb_data);
}

void DOOM_KeyDown(int key) {
    event_t event;
    event.type = ev_keydown;
    event.data1 = key;
    event.data2 = 0;
    event.data3 = 0;
    D_PostEvent(&event);
}

void DOOM_KeyUp(int key) {
    event_t event;
    event.type = ev_keyup;
    event.data1 = key;
    event.data2 = 0;
    event.data3 = 0;
    D_PostEvent(&event);
}

void DOOM_MouseMove(int x, int y) {
    event_t event;
    event.type = ev_mouse;
    event.data1 = 0;
    event.data2 = x << 2;
    event.data3 = y << 2;
    D_PostEvent(&event);
}

void DOOM_MouseButton(int button, boolean pressed) {
    event_t event;
    event.type = ev_mouse;
    event.data1 = pressed ? (1 << button) : 0;
    event.data2 = 0;
    event.data3 = 0;
    D_PostEvent(&event);
}

void DOOM_SendKeyPress(int key) {
    DOOM_KeyDown(key);
}
