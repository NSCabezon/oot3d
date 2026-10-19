#ifndef _Z_EN_MD_H_
#define _Z_EN_MD_H_

#include "global.hpp"

struct EnMd;

typedef void (*EnMdActionFunc)(struct EnMd*, PlayState*);

typedef struct {
    /* 0x00 */ u16 talkState;
    /* 0x02 */ char unk_02[0x06];
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x04];
} EnMdInteractInfo; // size = 0x28

typedef struct EnMd {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ FaceAnimations faceAnim;
    /* 0x03F4 */ EnMdActionFunc actionFunc;
    /* 0x03F8 */ ColliderCylinder collider;
    /* 0x0450 */ EnMdInteractInfo interactInfo;
    /* 0x0478 */ u8 messageEntry;
    /* 0x0479 */ u8 messageState;
    /* 0x047A */ u8 animSequenceEntry;
    /* 0x047B */ u8 animSequence;
    /* 0x047C */ s16 blinkTimer;
    /* 0x047E */ s16 eyeTexIndex;
    /* 0x0480 */ s16 alpha;
    /* 0x0482 */ s16 waypoint;
    /* 0x0484 */ s16 fidgetTableY[0x12];
    /* 0x04A8 */ s16 fidgetTableZ[0x12];
    /* 0x04CC */ char jointTable[0x3A8];
    /* 0x0874 */ char morphTable[0x3A8];
    /* 0x0C1C */ void* zar;
    /* 0x0C20 */ u8 initialized;
    /* 0x0C21 */ char unk_C21[0x03];
} EnMd; // size = 0xC24

extern ActorInit En_Md_InitVars;

#endif
