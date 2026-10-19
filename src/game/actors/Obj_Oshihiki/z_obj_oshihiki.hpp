#ifndef _Z_OBJ_OSHIHIKI_H_
#define _Z_OBJ_OSHIHIKI_H_

#include "global.hpp"

struct ObjOshihiki;

typedef void (*ObjOshihikiActionFunc)(struct ObjOshihiki*, struct PlayState*);

typedef struct ObjOshihiki {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ObjOshihikiActionFunc actionFunc;
    /* 0x01C0 */ SkeletonAnimationModel* skelAnimModel;
    /* 0x01C4 */ u16 stateFlags;
    /* 0x01C6 */ s16 timer;
    /* 0x01C8 */ f32 yawSin;
    /* 0x01CC */ f32 yawCos;
    /* 0x01D0 */ f32 pushDist;
    /* 0x01D4 */ f32 pushSpeed;
    /* 0x01D8 */ f32 direction;
    /* 0x01DC */ struct ObjOshihiki* blockUnder;
    /* 0x01E0 */ f32 underDistX;
    /* 0x01E4 */ f32 underDistZ;
    /* 0x01E8 */ char unk_1E8[0x8];
    /* 0x01F0 */ s32 floorBgIds[5];
    /* 0x0204 */ f32 floorHeights[5];
    /* 0x0218 */ CollisionPoly* floorPolys[5];
    /* 0x022C */ s16 highestFloor;
    /* 0x022E */ u8 cantMove;
    /* 0x022F */ char unk_22F;
} ObjOshihiki; // size = 0x230

extern ActorInit Obj_Oshihiki_InitVars;

#endif
