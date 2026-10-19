#ifndef _Z_EN_LIGHTBOX_H_
#define _Z_EN_LIGHTBOX_H_

#include "global.hpp"

struct EnLightbox;

typedef struct EnLightbox {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ SkeletonAnimationModel* skelAnimModel;
} EnLightbox; // size = 0x1C0

extern ActorInit En_Lightbox_InitVars;

#endif
