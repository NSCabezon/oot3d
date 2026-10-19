#ifndef _Z_EN_JJ_H_
#define _Z_EN_JJ_H_

#include "global.hpp"

struct EnJj;

typedef void (*EnJjActionFunc)(struct EnJj* self, PlayState* play);

typedef struct EnJj {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkelAnime skelAnime;
    /* 0x0240 */ char unk_240[0x128]; // FaceAnimations (unk_240 is the FaceAnimations object)
    /* 0x0368 */ char unk_368[0xA4];
    /* 0x040C */ char jointTable[0x270];
    /* 0x067C */ char morphTable[0x270];
    /* 0x08EC */ ColliderCylinder collider;
    /* 0x0944 */ EnJjActionFunc actionFunc;
    /* 0x0948 */ DynaPolyActor* bodyCollisionActor;
    /* 0x094C */ Actor* dust;
    /* 0x0950 */ s16 mouthOpenAngle;
    /* 0x0952 */ u16 flags;
    /* 0x0954 */ s16 cutsceneCountdownTimer;
    /* 0x0956 */ u8 eyeIndex;
    /* 0x0957 */ u8 blinkTimer;
    /* 0x0958 */ u8 extraBlinkCounter;
    /* 0x0959 */ u8 extraBlinkTotal;
} EnJj; // size = 0x095C

typedef enum EnJjType {
    /* -1 */ JABUJABU_MAIN = -1, // Head, drawn body, handles updating
    /*  0 */ JABUJABU_COLLISION, // Static collision for body
    /*  1 */ JABUJABU_UNUSED_COLLISION // Shaped like a screen
} EnJjType;

extern ActorInit En_Jj_InitVars;

#endif
