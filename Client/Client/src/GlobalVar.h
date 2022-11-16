#pragma once

class MPEditor;

extern MPEditor g_Editor;

#define SMALL_RES_X 1024
#define SMALL_RES_Y 576

#define MID_RES_X 1280
#define MID_RES_Y 720

#define LARGE_RES_X 1360
#define LARGE_RES_Y 765

#define EXTRA_LARGE_RES_X 1440
#define EXTRA_LARGE_RES_Y 810

#define FULL_LARGE_RES_X 1600
#define FULL_LARGE_RES_Y 900

// 60-30 FPS 
extern bool Is60FPSMode;
#define FPSModeNew Is60FPSMode ? 60:30

#define LARGE_RES_XS 1024.0
#define LARGE_RES_YS 800.0

#define CLIENT_NAME "Royal Pirates Online 1.0"
#define VERSION_NAME "The Frozen Relics"
#define VERSION_NUM "version 1.1 (1001 Release) 2022"

#define IsNitroOn true
#define NitroIndexStart 25 //emoji index to start filter them 