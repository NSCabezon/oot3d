#ifndef _Z_EN_WEATHER_TAG_H_
#define _Z_EN_WEATHER_TAG_H_

#include "global.hpp"

struct EnWeatherTag;

typedef void (*EnWeatherTagActionFunc)(Actor* thisx, GameState* state);

typedef struct EnWeatherTag {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnWeatherTagActionFunc actionFunc;
    /* 0x01A8 */ char unk_1A8[0x4];
    /* 0x01AC */ u32 unk_1AC;
    /* 0x01B0 */ u32 unk_1B0;
} EnWeatherTag; // size = 0x1B4

extern ActorInit En_Weather_Tag_InitVars;

#endif
