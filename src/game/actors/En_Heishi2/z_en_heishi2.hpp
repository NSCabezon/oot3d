#ifndef _Z_EN_HEISHI2_H_
#define _Z_EN_HEISHI2_H_

#include "global.hpp"

struct EnHeishi2;

typedef void (*EnHeishi2ActionFunc)(struct EnHeishi2*, PlayState*);

typedef struct EnHeishi2Block {
    /* 0x00 */ s32 words[12];
} EnHeishi2Block; // size = 0x30

typedef struct EnHeishi2 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x680];
    /* 0x08A8 */ EnHeishi2ActionFunc actionFunc;
    /* 0x08AC */ Vec3s unk_8AC;
    /* 0x08B2 */ char unk_8B2[0x6];
    /* 0x08B8 */ Vec3s unk_8B8;
    /* 0x08BE */ char unk_8BE[0x2];
    /* 0x08C0 */ Vec3f unk_8C0;
    /* 0x08CC */ Vec3f subCamAt;
    /* 0x08D8 */ Vec3f subCamEye;
    /* 0x08E4 */ Vec3f unk_8E4;
    /* 0x08F0 */ char unk_8F0[0x18];
    /* 0x0908 */ Vec3f unk_908;
    /* 0x0914 */ char unk_914[0x18];
    /* 0x092C */ f32 unk_92C;
    /* 0x0930 */ f32 unk_930;
    /* 0x0934 */ char unk_934[0x4];
    /* 0x0938 */ f32 unk_938;
    /* 0x093C */ char unk_93C[0x2];
    /* 0x093E */ s16 unk_93E[5];
    /* 0x0948 */ u16 unk_948;
    /* 0x094A */ s16 unk_94A;
    /* 0x094C */ s16 unk_94C;
    /* 0x094E */ char unk_94E[0x6];
    /* 0x0954 */ u8 unk_954;
    /* 0x0955 */ u8 unk_955;
    /* 0x0956 */ u8 unk_956;
    /* 0x0957 */ u8 unk_957;
    /* 0x0958 */ u8 unk_958;
    /* 0x0959 */ char unk_959[0x1];
    /* 0x095A */ u16 unk_95A;
    /* 0x095C */ s16 subCamId;
    /* 0x095E */ char unk_95E[0x2];
    /* 0x0960 */ Actor* gate;
    /* 0x0964 */ char unk_964[0x40];
    /* 0x09A4 */ ColliderCylinder collider;
    /* 0x09FC */ SkeletonAnimationModel* skelAnimModel;
    /* 0x0A00 */ SkeletonAnimationModel* unk_A00;
    /* 0x0A04 */ EnHeishi2Block unk_A04;
} EnHeishi2; // size = 0xA34

extern ActorInit En_Heishi2_InitVars;

#endif
