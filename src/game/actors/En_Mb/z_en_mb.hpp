#ifndef _Z_EN_MB_H_
#define _Z_EN_MB_H_

#include "global.hpp"

struct EnMb;

typedef void (*EnMbActionFunc)(struct EnMb*, struct PlayState*);

typedef struct EnMbQuadCollider {
    /* 0x00 */ Collider base;
    /* 0x18 */ char unk_18[0x28];
    /* 0x40 */ Vec3f quad[4];
    /* 0x70 */ char unk_70[0x10];
} EnMbQuadCollider; // size = 0x80

typedef struct EnMbGenericCollider {
    /* 0x00 */ Collider base;
    /* 0x18 */ char unk_18[0xC0];
} EnMbGenericCollider; // size = 0xD8

typedef struct EnMbJntSph {
    /* 0x00 */ Collider base;
    /* 0x18 */ s32 count;
    /* 0x1C */ ColliderInfo* elements;
    /* 0x20 */ char unk_20[0x50];
} EnMbJntSph; // size = 0x70

typedef struct EnMb {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ Vec3s bodyPartsPos[10];
    /* 0x01E0 */ u8 damageReaction;
    /* 0x01E1 */ char unk_1E1[0x3];
    /* 0x01E4 */ SkelAnime skelAnime;
    /* 0x0268 */ char unk_268[0x680];
    /* 0x08E8 */ s32 unk_8E8;
    /* 0x08EC */ s32 state;
    /* 0x08F0 */ EnMbActionFunc actionFunc;
    /* 0x08F4 */ s16 iceEffectTimer;
    /* 0x08F6 */ s16 timer1;
    /* 0x08F8 */ s16 timer3;
    /* 0x08FA */ s16 timer2;
    /* 0x08FC */ u16 unk_8FC;
    /* 0x08FE */ s16 unk_8FE;
    /* 0x0900 */ s16 unk_900;
    /* 0x0902 */ s16 unk_902;
    /* 0x0904 */ Vec3f effSpawnPos;
    /* 0x0910 */ Vec3f unk_910;
    /* 0x091C */ char unk_91C[0x0C];
    /* 0x0928 */ s8 unk_928;
    /* 0x0929 */ s8 unk_929;
    /* 0x092A */ s8 unk_92A;
    /* 0x092B */ char unk_92B[0x1];
    /* 0x092C */ f32 maxHomeDist;
    /* 0x0930 */ f32 playerDetectionRange;
    /* 0x0934 */ ColliderCylinder bodyCollider;
    /* 0x098C */ EnMbQuadCollider attackCollider;
    /* 0x0A0C */ EnMbGenericCollider frontShieldingCollider;
    /* 0x0AE4 */ s32 csState;
    /* 0x0AE8 */ s32 csTimer;
    /* 0x0AEC */ s32 csCamId;
    /* 0x0AF0 */ Vec3f csEyeTarget;
    /* 0x0AFC */ Vec3f csAtTarget;
    /* 0x0B08 */ Vec3f csEye;
    /* 0x0B14 */ Vec3f csAt;
    /* 0x0B20 */ f32 csStep;
    /* 0x0B24 */ Vec3f csSavedEye;
    /* 0x0B30 */ Vec3f csSavedAt;
    /* 0x0B3C */ EnMbJntSph unk_B3C;
} EnMb; // size = 0x0BAC

extern ActorInit En_Mb_InitVars;

#endif
