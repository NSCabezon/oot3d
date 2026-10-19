#ifndef _DUP_DECLS_H_
#define _DUP_DECLS_H_

// Declarations shared by the src/game/dup/dup_*.cpp translation units (duplicate-function sweep D1).

#include "global.hpp"

// Actor whose first member after Actor is a SkeletonAnimationModel* at +0x1A4 (EnVase layout).
typedef struct DupModelActor {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkeletonAnimationModel* skelAnimModel;
} DupModelActor;

// Same, with the model pointer at +0x1C8 (BgDdanJd layout).
typedef struct DupModelActor1C8 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x24];
    /* 0x01C8 */ SkeletonAnimationModel* skelAnimModel;
} DupModelActor1C8;

// Actor with a SkelAnime at +0x1A4.
typedef struct DupSkelActor {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
} DupSkelActor;

typedef void (*DupActionFunc)(struct DupActionActor1A4*, PlayState*);
typedef void (*DupActionFunc2)(struct DupActionActor1BC*, PlayState*);

// Actors that only store an action function at +0x1A4 / +0x1BC.
typedef struct DupActionActor1A4 {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ DupActionFunc actionFunc;
} DupActionActor1A4;

typedef struct DupActionActor1BC {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x18];
    /* 0x01BC */ DupActionFunc2 actionFunc;
} DupActionActor1BC;

// Actor with a SkelAnime at +0x1A4, an eye index at +0x1268 and a FaceAnimations at +0x1330 (En_Ru1 layout).
typedef struct DupFaceActor {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ SkelAnime skelAnime;
    /* 0x0228 */ char unk_228[0x1268 - 0x228];
    /* 0x1268 */ s16 eyeIndex;
    /* 0x126A */ char unk_126A[0x1330 - 0x126A];
    /* 0x1330 */ FaceAnimations face;
} DupFaceActor;

#endif
