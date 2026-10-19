#ifndef _Z_OBJ_BEAN_H_
#define _Z_OBJ_BEAN_H_

#include "global.hpp"

struct ObjBean;

typedef void (*ObjBeanActionFunc)(struct ObjBean*, PlayState*);
typedef void (*ObjBeanTransformFunc)(struct ObjBean*);

// 3DS layout: N64 offsets + 0x58 up to the collider, +0x74 from the timer on.
typedef struct ObjBean {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01BC */ ObjBeanActionFunc actionFunc;
    /* 0x01C0 */ ColliderCylinder collider;
    /* 0x0218 */ SkeletonAnimationModel* unk_218;
    /* 0x021C */ SkeletonAnimationModel* unk_21C;
    /* 0x0220 */ SkeletonAnimationModel* unk_220;
    /* 0x0224 */ SkeletonAnimationModel* unk_224;
    /* 0x0228 */ s16 timer;
    /* 0x022A */ Vec3s unk_1B6;
    /* 0x0230 */ ObjBeanTransformFunc transformFunc;
    /* 0x0234 */ s16 unk_1C0;
    /* 0x0236 */ s16 unk_1C2;
    /* 0x0238 */ s16 unk_1C4;
    /* 0x023A */ s16 unk_1C6;
    /* 0x023C */ s16 unk_1C8;
    /* 0x023E */ s16 unk_1CA;
    /* 0x0240 */ s16 unk_1CC;
    /* 0x0242 */ s16 unk_1CE;
    /* 0x0244 */ s16 leafRotFactor;
    /* 0x0246 */ u16 unk_1D2;
    /* 0x0248 */ f32 stalkSizeMultiplier;
    /* 0x024C */ Vec3f pathPoints;
    /* 0x0258 */ f32 unk_1E4;
    /* 0x025C */ f32 posOffsetX;
    /* 0x0260 */ f32 posOffsetZ;
    /* 0x0264 */ s16 pathCount;
    /* 0x0266 */ s16 currentPointIndex;
    /* 0x0268 */ s16 nextPointIndex;
    /* 0x026A */ u8 unk_1F6;
    /* 0x026B */ u8 stateFlags;
} ObjBean; // size = 0x026C

extern ActorInit Obj_Bean_InitVars;

static inline void ObjBean_SetDrawMode(ObjBean* self, u8 drawFlag) {
    self->stateFlags &= ~(0x1 | 0x2 | 0x4 | 0x8);
    self->stateFlags |= drawFlag;
}

#endif
