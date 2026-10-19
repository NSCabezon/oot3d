#ifndef _Z_EN_ANUBICE_H_
#define _Z_EN_ANUBICE_H_

#include "global.hpp"

struct EnAnubice;

typedef void (*EnAnubiceActionFunc)(struct EnAnubice*, PlayState*);

typedef struct EnAnubice {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ Vec3s jointTable[10];
    /* 0x0264 */ char unk_264[0x1CC];
    /* 0x0430 */ Vec3s morphTable[10];
    /* 0x046C */ char unk_46C[0x1CC];
    /* 0x0638 */ EnAnubiceActionFunc actionFunc;
    /* 0x063C */ s16 timeAlive;
    /* 0x063E */ s16 isNearWall;
    /* 0x0640 */ s16 fallTargetYaw;
    /* 0x0642 */ s16 deathTimer;
    /* 0x0644 */ s16 knockbackTimer;
    /* 0x0646 */ s16 isMirroringPlayer;
    /* 0x0648 */ s16 isPlayerOutOfRange;
    /* 0x064A */ s16 isKnockedback;
    /* 0x064C */ s16 hasSearchedForFlameCircles;
    /* 0x064E */ char unk_64E[0x2];
    /* 0x0650 */ f32 hoverVelocityTimer;
    /* 0x0654 */ f32 animLastFrame;
    /* 0x0658 */ f32 targetHeight;
    /* 0x065C */ f32 playerHeightOffset;
    /* 0x0660 */ f32 headPitch;
    /* 0x0664 */ f32 focusHeightOffset;
    /* 0x0668 */ Vec3f headPos;
    /* 0x0674 */ Vec3f fireballRot;
    /* 0x0680 */ Vec3f home;
    /* 0x068C */ Vec3f knockbackRecoveryVelocity;
    /* 0x0698 */ struct BgHidanCurtain* flameCircles[5];
    /* 0x06AC */ char unk_6AC[0x4];
    /* 0x06B0 */ ColliderCylinder collider;
} EnAnubice; // size = 0x708

extern ActorInit En_Anubice_InitVars;

#endif
