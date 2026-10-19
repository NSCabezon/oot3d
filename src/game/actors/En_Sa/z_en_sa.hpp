#ifndef _Z_EN_SA_H_
#define _Z_EN_SA_H_

#include "global.hpp"

struct EnSa;

typedef void (*EnSaActionFunc)(struct EnSa*, PlayState*);

typedef struct {
    /* 0x00 */ u16 talkState;
    /* 0x02 */ char unk_02[0x06];
    /* 0x08 */ Vec3s headRot;
    /* 0x0E */ Vec3s torsoRot;
    /* 0x14 */ f32 yOffset;
    /* 0x18 */ Vec3f trackPos;
    /* 0x24 */ char unk_24[0x04];
} EnSaInteractInfo; // size = 0x28

typedef struct EnSa {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ FaceAnimations faceAnim;
    /* 0x03F4 */ EnSaActionFunc actionFunc;
    /* 0x03F8 */ ColliderCylinder collider;
    /* 0x0450 */ EnSaInteractInfo interactInfo;
    /* 0x0478 */ u8 messageIndex;
    /* 0x0479 */ u8 prevTextState;
    /* 0x047A */ u8 animPhase;
    /* 0x047B */ u8 animGroup;
    /* 0x047C */ s16 blinkTimer;
    /* 0x047E */ s16 cueId;
    /* 0x0480 */ s16 rightEyeIndex;
    /* 0x0482 */ s16 leftEyeIndex;
    /* 0x0484 */ s16 alpha;
    /* 0x0486 */ Vec3s initRot;
    /* 0x048C */ char jointTable[0x3DC];
    /* 0x0868 */ char morphTable[0x3DC];
    /* 0x0C44 */ void* zar;
    /* 0x0C48 */ u8 unk_C48;
    /* 0x0C49 */ u8 unk_C49;
    /* 0x0C4A */ char unk_C4A[0x2];
} EnSa; // size = 0x0C4C

extern ActorInit En_Sa_InitVars;

#endif
