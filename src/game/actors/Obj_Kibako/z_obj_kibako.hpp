#ifndef _Z_OBJ_KIBAKO_H_
#define _Z_OBJ_KIBAKO_H_

#include "global.hpp"

struct ObjKibako;

typedef void (*ObjKibakoActionFunc)(struct ObjKibako*, struct PlayState*);

typedef struct ObjKibako {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ ObjKibakoActionFunc actionFunc;
    /* 0x01A8 */ ColliderCylinder collider;
    /* 0x0200 */ SkeletonAnimationModel* model;
    /* 0x0204 */ f32 unk_204;
    /* 0x0208 */ u32 unk_208;
} ObjKibako; // size = 0x20C

extern ActorInit Obj_Kibako_InitVars;

#endif
