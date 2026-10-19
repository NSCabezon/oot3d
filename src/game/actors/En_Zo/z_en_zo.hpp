#ifndef _Z_EN_ZO_H_
#define _Z_EN_ZO_H_

#include "global.hpp"

struct EnZo;
typedef void (*EnZoActionFunc)(struct EnZo*, PlayState*);

typedef struct EnZoEffect {
    /* 0x00 */ u8 type;
    /* 0x01 */ char pad_01[3];
    /* 0x04 */ f32 scale;
    /* 0x08 */ f32 targetScale;
    /* 0x0C */ Color_RGBA8 color;
    /* 0x10 */ u32 pad_10;
    /* 0x14 */ Vec3f pos;
    /* 0x20 */ Vec3f vel;
    /* 0x2C */ Vec3f accel;
} EnZoEffect;

typedef struct EnZo {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ FaceAnimations faceAnimations;
    /* 0x03F4 */ ZARInfo* zar;
    /* 0x03F8 */ char jointTable[0x444];
    /* 0x083C */ char morphTable[0x444];
    /* 0x0C80 */ void* unk_C80;
    /* 0x0C84 */ void* unk_C84;
    /* 0x0C88 */ void* unk_C88;
    /* 0x0C8C */ EnZoActionFunc actionFunc;
    /* 0x0C90 */ s16 talkState;
    /* 0x0C92 */ char unk_C92[0x06];
    /* 0x0C98 */ Vec3s headRot;
    /* 0x0C9E */ Vec3s torsoRot;
    /* 0x0CA4 */ f32 trackingHeight;
    /* 0x0CA8 */ Vec3f trackPos;
    /* 0x0CB4 */ char unk_CB4[4];
    /* 0x0CB8 */ ColliderCylinder collider;
    /* 0x0D10 */ u8 canSpeak;
    /* 0x0D11 */ char unk_D11[3];
    /* 0x0D14 */ EnZoEffect effects[15];
    /* 0x105C */ f32 dialogRadius;
    /* 0x1060 */ f32 alpha;
    /* 0x1064 */ s16 trackingMode;
    /* 0x1066 */ s16 rippleTimer;
    /* 0x1068 */ s16 timeToDive;
    /* 0x106A */ s16 blinkTimer;
    /* 0x106C */ s16 eyeTexture;
    /* 0x106E */ s16 fidgetTableY[21];
    /* 0x1098 */ s16 fidgetTableZ[21];
    /* 0x10C2 */ char unk_10C2[0x02];
} EnZo; // size = 0x10C4

typedef char EnZoSizeCheck[(sizeof(EnZo) == 0x10C4) ? 1 : -1];

extern ActorInit En_Zo_InitVars;

#endif
