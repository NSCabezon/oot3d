#ifndef _Z_BG_YDAN_SP_H_
#define _Z_BG_YDAN_SP_H_

#include "global.hpp"
#include "graphics/skeleton_animation_model.hpp"

struct BgYdanSp;

typedef struct BgYdanSp {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ s32 bgId;
    /* 0x01A8 */ char unk_1A8[0x14];
    /* 0x01BC */ void (*actionFunc)(struct BgYdanSp*, PlayState*);
    /* 0x01C0 */ SkeletonAnimationModel* model;
    /* 0x01C4 */ SkeletonAnimationModel* fragments[8];
    /* 0x01E4 */ f32 amplitude;
    /* 0x01E8 */ f32 unk_1E8;
    /* 0x01EC */ Collider collider; // Tris base; element header follows.
    /* 0x0204 */ s32 trisCount;
    /* 0x0208 */ void* trisElements;
    /* 0x020C */ char elements[0xB8]; // two 0x5C-byte tris elements.
    /* 0x02C4 */ u8 isDestroyedSwitchFlag;
    /* 0x02C5 */ u8 burnSwitchFlag;
    /* 0x02C6 */ s8 timer; // actually 3DS timer field used by FloorWebBroken
    /* 0x02C7 */ bool playChime;
    /* 0x02C8 */ bool enablePlayerPush;
    /* 0x02C9 */ bool pushingPlayer;
    /* 0x02CA */ char unk_2CA[2];
} BgYdanSp; // size = 0x2CC

// Player fields used by the webs, mapped from the 3DS assembly. Local view
// avoids changing the shared, incomplete Player declaration.
struct BgYdanSpPlayer {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[5];
    /* 0x01A9 */ u8 heldItemAction;
    /* 0x01AA */ char unk_1AA[0x284 - 0x1AA];
    /* 0x0284 */ s32 action;
    /* 0x0288 */ char unk_288[0x1714 - 0x288];
    /* 0x1714 */ u32 stateFlags2;
    /* 0x1718 */ char unk_1718[0x221C - 0x1718];
    /* 0x221C */ f32 xzSpeed;
    /* 0x2220 */ char unk_2220[0x2248 - 0x2220];
    /* 0x2248 */ s16 stickBurnTimer;
    /* 0x224A */ char unk_224A[0x2282 - 0x224A];
    /* 0x2282 */ s16 fallDistance;
    /* 0x2284 */ char unk_2284[0x229C - 0x2284];
    /* 0x229C */ f32 pushedSpeed;
    /* 0x22A0 */ s16 pushedYaw;
    /* 0x22A2 */ char unk_22A2[6];
    /* 0x22A8 */ Vec3f stickTip;
};

extern ActorInit Bg_Ydan_Sp_InitVars;

#endif
