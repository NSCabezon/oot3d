#ifndef _Z_DEMO_KEKKAI_H_
#define _Z_DEMO_KEKKAI_H_

#include "global.hpp"

struct DemoKekkai;

typedef void (*DemoKekkaiUpdateFunc)(struct DemoKekkai* self, PlayState* play);

typedef struct DemoKekkai {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ColliderCylinder collider1;
    /* 0x01FC */ ColliderCylinder collider2;
    /* 0x0254 */ f32 energyAlpha;
    /* 0x0258 */ f32 orbScale;
    /* 0x025C */ f32 barrierScroll;
    /* 0x0260 */ f32 barrierScrollRate;
    /* 0x0264 */ u16 timer;
    /* 0x0266 */ u16 sfxFlag;
    /* 0x0268 */ DemoKekkaiUpdateFunc updateFunc;
    /* 0x026C */ SkeletonAnimationModel* skelAnimModel[3];
    /* 0x0278 */ void* cmbData;
} DemoKekkai; // size = 0x27C

extern ActorInit Demo_Kekkai_InitVars;

#endif
