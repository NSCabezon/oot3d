#ifndef _Z_ARMS_HOOK_H_
#define _Z_ARMS_HOOK_H_

#include "global.hpp"

struct ArmsHook;

typedef void (*ArmsHookActionFunc)(struct ArmsHook*, PlayState*);

typedef struct ArmsHookQuad {
    char data[0x80];
} ArmsHookQuad;

typedef struct ArmsHook {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ArmsHookQuad collider;
    /* 0x0224 */ char unk_224[0x34];
    /* 0x0258 */ Vec3f unk_1E8;
    /* 0x0264 */ Vec3f unk_1F4;
    /* 0x0270 */ Actor* attachedActor;
    /* 0x0274 */ char unk_274[0x0C];
    /* 0x0280 */ s16 timer;
    /* 0x0282 */ char unk_282[0x2];
    /* 0x0284 */ ArmsHookActionFunc actionFunc;
    /* 0x0288 */ f32 unk_288;
    /* 0x028C */ void* models[2];
} ArmsHook; // size = 0x294

extern ActorInit Arms_Hook_InitVars;

#endif
