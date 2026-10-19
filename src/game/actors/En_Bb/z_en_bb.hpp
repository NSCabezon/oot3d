#ifndef _Z_EN_BB_H_
#define _Z_EN_BB_H_

#include "global.hpp"

struct EnBb;

typedef void (*EnBbActionFunc)(struct EnBb*, PlayState*);

typedef struct EnBbElem {
    /* 0x00 */ char unk_00[0x0E];
    /* 0x0E */ s16 hitX;
    /* 0x10 */ s16 hitY;
    /* 0x12 */ s16 hitZ;
    /* 0x14 */ char unk_14[0x10];
    /* 0x24 */ u8* acHitElem;
    /* 0x28 */ char unk_28[0x0C];
    /* 0x34 */ f32 unk_34;
    /* 0x38 */ f32 cx;
    /* 0x3C */ f32 cy;
    /* 0x40 */ f32 cz;
    /* 0x44 */ f32 unk_44;
} EnBbElem;

typedef struct EnBb {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x35C];          // SkelAnime etc.
    /* 0x0500 */ char unk_500[0x0C];           // SkeletonAnimationModel
    /* 0x050C */ char unk_50C[0x18];           // SkeletonAnimationModel
    /* 0x0524 */ s32 unk_524;
    /* 0x0528 */ char unk_528[0x4];
    /* 0x052C */ EnBbActionFunc unk_52C;
    /* 0x0530 */ s32 unk_530;
    /* 0x0534 */ s32 unk_534;
    /* 0x0538 */ char unk_538[0x0A];
    /* 0x0542 */ s16 unk_542;
    /* 0x0544 */ char unk_544[0x0C];
    /* 0x0550 */ f32 unk_550;
    /* 0x0554 */ char unk_554[0x08];
    /* 0x055C */ f32 unk_55C;
    /* 0x0560 */ f32 unk_560;
    /* 0x0564 */ char unk_564[0x0F];
    /* 0x0573 */ u8 unk_573;
    /* 0x0574 */ char unk_574[0x03];
    /* 0x0577 */ u8 unk_577;
    /* 0x0578 */ char unk_578[0x04];
    /* 0x057C */ s16 unk_57C;
    /* 0x057E */ u8 unk_57E;
    /* 0x057F */ char unk_57F[0x01];
    /* 0x0580 */ char unk_580[0x10];           // ColliderJntSph
    /* 0x0590 */ u8 unk_590;                   // collider.base.atFlags
    /* 0x0591 */ u8 unk_591;                   // collider.base.acFlags
    /* 0x0592 */ char unk_592[0x0A];
    /* 0x059C */ struct EnBbElem* unk_59C;
    /* 0x05A0 */ char unk_5A0[0x50];
    /* 0x05F0 */ char unk_5F0[0x14];           // BodyBreak
    /* 0x0604 */ struct EnBb* unk_604;
} EnBb; // size = 0x608

extern ActorInit En_Bb_InitVars;

#endif
