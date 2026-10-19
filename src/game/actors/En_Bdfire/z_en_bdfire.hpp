#ifndef _Z_EN_BDFIRE_H_
#define _Z_EN_BDFIRE_H_

#include "global.hpp"

struct EnBdfire;

typedef void (*EnBdfireActionFunc)(struct EnBdfire*, PlayState*);
typedef void (*EnBdfireDrawFunc)(struct EnBdfire*, PlayState*);

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 scale;
    /* 0x10 */ s32 active;
} EnBdfireTrail; // size = 0x14

typedef struct EnBdfire {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ EnBdfireActionFunc actionFunc;
    /* 0x01A8 */ EnBdfireDrawFunc drawFunc;
    /* 0x01AC */ s16 unk_1AC;
    /* 0x01AE */ s16 unk_1AE;
    /* 0x01B0 */ s16 unk_1B0;
    /* 0x01B2 */ char unk_1B2[4];
    /* 0x01B6 */ s16 unk_1B6;
    /* 0x01B8 */ s16 unk_1B8;
    /* 0x01BA */ char unk_1BA[0x26];
    /* 0x01E0 */ f32 unk_1E0;
    /* 0x01E4 */ f32 unk_1E4;
    /* 0x01E8 */ f32 unk_1E8;
    /* 0x01EC */ f32 unk_1EC;
    /* 0x01F0 */ Vec3f unk_1F0;
    /* 0x01FC */ Vec3f unk_1FC;
    /* 0x0208 */ char unk_208[0x20];
    /* 0x0228 */ void* lightNode;
    /* 0x022C */ char lightInfoNoGlow[0x18];
    /* 0x0244 */ SkeletonAnimationModel* unk_244;
    /* 0x0248 */ void* unk_248[2];
    /* 0x0250 */ EnBdfireTrail trail[30];
    /* 0x04A8 */ char unk_4A8[0x258];
} EnBdfire; // size = 0x700

extern ActorInit En_Bdfire_InitVars;

#endif
