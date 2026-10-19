#ifndef _Z_EN_TAKARA_MAN_H_
#define _Z_EN_TAKARA_MAN_H_

#include "global.hpp"

struct EnTakaraMan;

typedef void (*EnTakaraManActionFunc)(struct EnTakaraMan*, PlayState*);

typedef struct EnTakaraMan {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[10];
    /* 0x0264 */ char unk_264[0x198];
    /* 0x03FC */ Vec3s morphTable[10];
    /* 0x0438 */ char unk_438[0x198];
    /* 0x05D0 */ EnTakaraManActionFunc actionFunc;
    /* 0x05D4 */ s16 dialogState;
    /* 0x05D6 */ s16 eyeTextureIdx;
    /* 0x05D8 */ s16 eyeTimer;
    /* 0x05DA */ s16 unk_212;
    /* 0x05DC */ s16 unk_214;
    /* 0x05DE */ s16 originalRoomNum;
    /* 0x05E0 */ s16 unk_218;
    /* 0x05E2 */ s16 unk_21A;
    /* 0x05E4 */ f32 height;
    /* 0x05E8 */ Vec3f pos;
    /* 0x05F4 */ Vec3s unk_22C;
    /* 0x05FA */ Vec3s unk_232;
    /* 0x0600 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0604 */ FaceAnimations faceAnims;
} EnTakaraMan; // size = 0x07D0

extern ActorInit En_Takara_Man_InitVars;

#endif
