extern "C" {
#include "z_en_horse_normal.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000000

void EnHorseNormal_Init(Actor* actor, GameState* state);
void EnHorseNormal_Destroy(Actor* actor, GameState* state);
void EnHorseNormal_Update(Actor* actor, GameState* state);
void EnHorseNormal_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Horse_Normal.data.s")

ActorInit En_Horse_Normal_InitVars = {
    ACTOR_EN_HORSE_NORMAL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HORSE_NORMAL,
    sizeof(EnHorseNormal),
    (ActorFunc)EnHorseNormal_Init,
    (ActorFunc)EnHorseNormal_Destroy,
    (ActorFunc)EnHorseNormal_Update,
    (ActorFunc)EnHorseNormal_Draw,
};

GLOBAL_ASM("asm/EnHorseNormal_Init.s")

GLOBAL_ASM("asm/EnHorseNormal_Destroy.s")

GLOBAL_ASM("asm/EnHorseNormal_Update.s")

GLOBAL_ASM("asm/EnHorseNormal_Draw.s")

GLOBAL_ASM("asm/EnHorseNormal_CycleAnimations.s")

GLOBAL_ASM("asm/EnHorseNormal_FollowPath.s")

GLOBAL_ASM("asm/EnHorseNormal_Wait.s")

GLOBAL_ASM("asm/EnHorseNormal_WaitClone.s")

GLOBAL_ASM("asm/EnHorseNormal_Wander.s")

GLOBAL_ASM("asm/FUN_001c3180.s")

GLOBAL_ASM("asm/FUN_003406b8.s")

// FUN_003a5778 (matrix = Trans(trans) * Rx*Ry*Rz * Scale(scale)): draft, recipe R8. The MTX34_MakeTrans part matches; the scale
// matrix at sp+0x30 is ALSO built word-wise with integer ldr/str (an SDK-style MakeScale u32 copy, not covered by the accepted
// helper) -> float stores here. Also stack order / axis Vec3f reg (r7) not tuned. Needs a user decision on MTX34_MakeScale.
#if 0
void FUN_003625f8(MTX34* dst, f32 angle, Vec3f* axis);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);

static inline void EnHorseNormal_MakeScale(MTX34* m, Vec3f* s) {
    m->data[0][0] = s->x;
    m->data[0][1] = 0.0f;
    m->data[0][2] = 0.0f;
    m->data[0][3] = 0.0f;
    m->data[1][0] = 0.0f;
    m->data[1][1] = s->y;
    m->data[1][2] = 0.0f;
    m->data[1][3] = 0.0f;
    m->data[2][0] = 0.0f;
    m->data[2][1] = 0.0f;
    m->data[2][2] = s->z;
    m->data[2][3] = 0.0f;
}

void FUN_003a5778(MTX34* m, Vec3f* scale, Vec3s* rot, Vec3f* trans) {
    MTX34 tmp;
    MTX34 scaleMtx;
    MTX34 rz;
    MTX34 ry;
    MTX34 rx;
    Vec3f axis;

    MTX34_MakeTrans(m, trans);
    axis.x = 1.0f;
    axis.y = 0.0f;
    axis.z = 0.0f;
    FUN_003625f8(&rx, rot->x * 9.58738E-5f, &axis);
    axis.x = 0.0f;
    axis.y = 1.0f;
    axis.z = 0.0f;
    FUN_003625f8(&ry, rot->y * 9.58738E-5f, &axis);
    axis.x = 0.0f;
    axis.y = 0.0f;
    axis.z = 1.0f;
    FUN_003625f8(&rz, rot->z * 9.58738E-5f, &axis);
    FUN_0036c174(&tmp, &rx, &ry);
    FUN_0036c174(&tmp, &tmp, &rz);
    FUN_0036c174(&tmp, m, &tmp);
    EnHorseNormal_MakeScale(&scaleMtx, scale);
    FUN_0036c174(m, &tmp, &scaleMtx);
}
#endif

GLOBAL_ASM("asm/FUN_003a5778.s")

}
