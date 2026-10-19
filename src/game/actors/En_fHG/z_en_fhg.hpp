#ifndef _Z_EN_FHG_H_
#define _Z_EN_FHG_H_

#include "global.hpp"

struct EnfHG;

typedef void (*EnfHGActionFunc)(struct EnfHG*, PlayState*);

typedef struct EnfHG {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ u8 bossGndSignal;
    /* 0x01A5 */ u8 bossGndInPainting;
    /* 0x01A6 */ u8 killActor;
    /* 0x01A7 */ u8 fhgFireKillWarp;
    /* 0x01A8 */ Vec3f subCamEye;
    /* 0x01B4 */ Vec3f subCamAt;
    /* 0x01C0 */ Vec3f subCamEyeVel;
    /* 0x01CC */ Vec3f subCamAtVel;
    /* 0x01D8 */ Vec3f hoofSfxPos;
    /* 0x01E4 */ Vec3f inPaintingPos;
    /* 0x01F0 */ f32 inPaintingVelX;
    /* 0x01F4 */ f32 inPaintingVelZ;
    /* 0x01F8 */ f32 damageSpeedMod;
    /* 0x01FC */ f32 approachRate;
    /* 0x0200 */ f32 subCamVelFactor;
    /* 0x0204 */ f32 subCamPanZ;
    /* 0x0208 */ char unk_208[0x10];
    /* 0x0218 */ s16 gallopTimer;
    /* 0x021A */ s16 curPainting;
    /* 0x021C */ s16 targetPainting;
    /* 0x021E */ s16 turnTarget;
    /* 0x0220 */ s16 spawnedWarp;
    /* 0x0222 */ s16 cutsceneState;
    /* 0x0224 */ s16 subCamId;
    /* 0x0226 */ char unk_226[6];
    /* 0x022C */ s16 timers[5];
    /* 0x0236 */ s16 hitTimer;
    /* 0x0238 */ s16 turnRot;
    /* 0x023A */ char unk_23A[6];
    /* 0x0240 */ f32 warpColorFilterR;
    /* 0x0244 */ f32 warpColorFilterG;
    /* 0x0248 */ f32 warpColorFilterB;
    /* 0x024C */ f32 warpColorFilterUnk1;
    /* 0x0250 */ f32 warpColorFilterUnk2;
    /* 0x0254 */ EnfHGActionFunc actionFunc;
    /* 0x0258 */ char unk_258[4];
    /* 0x025C */ char unk_25C[0x10];
    /* 0x026C */ SkelAnime skelAnime;
    /* 0x02F0 */ f32 unk_2F0;
    /* 0x02F4 */ char unk_2F4[0xA30];
    /* 0x0D24 */ s32 unk_D24;
    /* 0x0D28 */ void* unk_D28;
} EnfHG; // size = 0xD2C

extern ActorInit En_fHG_InitVars;

#endif
