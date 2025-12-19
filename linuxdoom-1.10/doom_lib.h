#ifndef DOOM_LIB_H
#define DOOM_LIB_H

#include "doomdef.h"
#include "doomstat.h"

void DOOM_Init(int argc, char** argv);

void DOOM_RunTic(void);

void DOOM_RenderFrame(void);

byte* DOOM_GetScreenBuffer(void);

int DOOM_GetScreenWidth(void);

int DOOM_GetScreenHeight(void);

byte* DOOM_GetPalette(void);

boolean DOOM_IsPlayerReady(void);

int DOOM_GetGameTic(void);

int DOOM_GetLevelTime(void);

void DOOM_SaveFramePNG(const char* filename);

#endif