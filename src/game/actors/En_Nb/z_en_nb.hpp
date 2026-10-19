#ifndef _Z_EN_NB_H_
#define _Z_EN_NB_H_

#include "global.hpp"

struct EnNb;

typedef struct EnNb {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0xB18 - 0x228];
    /* 0x0B18 */ char faceAnimations[0xCE4 - 0xB18]; // FaceAnimations, accessed via cast
    /* 0x0CE4 */ s16 eyeIdx;
    /* 0x0CE6 */ s16 blinkTimer;
    /* 0x0CE8 */ s32 action;
    /* 0x0CEC */ s32 drawMode;
    /* 0x0CF0 */ f32 alphaTimer;
    /* 0x0CF4 */ s32 alpha;
    /* 0x0CF8 */ s32 unk_CF8;
    /* 0x0CFC */ s32 cueId;
    /* 0x0D00 */ char unk_D00[0xD04 - 0xD00];
    /* 0x0D04 */ char collider[0xD5C - 0xD04]; // ColliderCylinder, accessed via cast
    /* 0x0D5C */ s32 unk_D5C;
    /* 0x0D60 */ Vec3f initialPos;
    /* 0x0D6C */ Vec3f finalPos;
    /* 0x0D78 */ s16 pathYaw;
    /* 0x0D7A */ u16 movementTimer;
    /* 0x0D7C */ char unk_D7C[0xD84 - 0xD7C];
    /* 0x0D84 */ s16 headRot[3];
    /* 0x0D8A */ s16 torsoRot[3];
    /* 0x0D90 */ char unk_D90[0xDA8 - 0xD90];
} EnNb; // size = 0xDA8

extern ActorInit En_Nb_InitVars;

#endif
