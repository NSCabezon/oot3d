#ifndef _Z_EN_VIEWER_H_
#define _Z_EN_VIEWER_H_

#include "global.hpp"

struct EnViewer;

typedef void (*EnViewerActionFunc)(struct EnViewer*, PlayState*);

typedef struct EnViewer {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x98]; // Skin buffers, freed by FUN_00408aac
    /* 0x023C */ SkeletonAnimationModel* animModels[20]; // per-slot model objects, freed in Destroy for type 5
    /* 0x028C */ char unk_28C[0x1CC];  // FaceAnimations (accessed through a cast)
    /* 0x0458 */ Actor* ganondorfCape; // child spawned in Init
    /* 0x045C */ char unk_45C[0x1];
    /* 0x045D */ u8 drawFuncIndex;
    /* 0x045E */ char unk_45E[0x2];
    /* 0x0460 */ EnViewerActionFunc actionFunc;
    /* 0x0464 */ u8 unused;
    /* 0x0465 */ u8 state;
    /* 0x0466 */ u8 isVisible;
    /* 0x0467 */ char unk_467[0x878 - 0x467];
} EnViewer; // size = 0x878

extern ActorInit En_Viewer_InitVars;

#endif
