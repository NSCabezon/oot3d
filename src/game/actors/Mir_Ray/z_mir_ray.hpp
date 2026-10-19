#ifndef _Z_MIR_RAY_H_
#define _Z_MIR_RAY_H_

#include "global.hpp"

typedef struct MirRayDataEntry {
    /* 0x00 */ Vec3s sourcePoint;
    /* 0x06 */ Vec3s poolPoint;
    /* 0x0C */ s16 sourceEndRadius;
    /* 0x0E */ s16 poolEndRadius;
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 lgtPtMaxRad;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ Color_RGB8 color;
    /* 0x1F */ u8 params;
} MirRayDataEntry; // size = 0x20

typedef struct MirRayJntSphElem {
    /* 0x00 */ char unk_00[0x38];
    /* 0x38 */ Vec3f center;
    /* 0x44 */ f32 radius;
    /* 0x48 */ f32 scale;
} MirRayJntSphElem;

struct MirRay;

typedef struct MirRay {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char colliderJntSph[0x1C];
    /* 0x01C0 */ MirRayJntSphElem* jntSphElems;
    /* 0x01C4 */ char jntSphElemStorage[0x214 - 0x1C4];
    /* 0x0214 */ char shieldRay[0x294 - 0x214];
    /* 0x0294 */ f32 reflectIntensity;
    /* 0x0298 */ Vec3f shieldCorners[6];
    /* 0x02E0 */ f32 reflectRange;
    /* 0x02E4 */ Vec3f sourcePt;
    /* 0x02F0 */ Vec3f poolPt;
    /* 0x02FC */ s16 sourceEndRad;
    /* 0x02FE */ s16 poolEndRad;
    /* 0x0300 */ s16 lightPointRad;
    /* 0x0304 */ void* lightNode;
    /* 0x0308 */ char lightInfo[0x18];
    /* 0x0320 */ u8 unLit;
    /* 0x0321 */ char unk_321[0x324 - 0x321];
    /* 0x0324 */ void* cmb;
    /* 0x0328 */ char model[0x35C - 0x328];
} MirRay; // size = 0x35C

extern ActorInit Mir_Ray_InitVars;

#endif
