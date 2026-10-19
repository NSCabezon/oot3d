extern "C" {
#include "z_en_bubble.hpp"

#define FLAGS 0x00000001

void EnBubble_Init(Actor* thisx, PlayState* play);
void EnBubble_Destroy(Actor* thisx, PlayState* play);
void EnBubble_Update(Actor* thisx, PlayState* play);
void EnBubble_Draw(Actor* thisx, PlayState* play);
void EnBubble_Wait(EnBubble* self, PlayState* play);
void EnBubble_Regrow(EnBubble* self, PlayState* play);
void EnBubble_Fly(EnBubble* self, PlayState* play);
void EnBubble_Pop(EnBubble* self, PlayState* play);

void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_DestroyJntSph(PlayState* play, void* collider);

GLOBAL_ASM("data/z_En_Bubble.data.s")
GLOBAL_ASM("data/z_En_Bubble.data_005214D8.s")
GLOBAL_ASM("../src/game/actors/En_Bubble/data_syms.s")

extern u8 EnBubble_sJntSphInit[];
extern u8 EnBubble_sColChkInfoInit2[];
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
DamageTable* DamageTable_Get(s32 index);


ActorInit En_Bubble_InitVars = {
    ACTOR_EN_BUBBLE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BUBBLE,
    sizeof(EnBubble),
    (ActorFunc)EnBubble_Init,
    (ActorFunc)EnBubble_Destroy,
    (ActorFunc)EnBubble_Update,
    (ActorFunc)EnBubble_Draw,
};


void EnBubble_Destroy(Actor* thisx, PlayState* play) {
    EnBubble* self = (EnBubble*)thisx;

    FUN_00350f34(thisx, &self->skelAnimModel, 0);
    Collider_DestroyJntSph(play, &self->colliderJntSph);
}

void EnBubble_Update(Actor* thisx, PlayState* play) {
    EnBubble* self = (EnBubble*)thisx;

    self->frameCount++;
    Math_SmoothStepToF(&self->graphicRotSpeed, 16.0f * 2.0f / 3.0f, 0.2f, 1000.0f, 0.0f);
    Math_SmoothStepToF(&self->graphicEccentricity, 0.08f, 0.2f, 1000.0f, 0.0f);
    FUN_0036b96c(thisx);
    Actor_UpdateBgCheckInfo(play, thisx, 16.0f, 16.0f, 0.0f, 7);
    self->actionFunc(self, play);
    Actor_SetFocus(thisx, self->actor.shape.yOffset);
}

void EnBubble_Regrow(EnBubble* self, PlayState* play) {
    self->expansionWidth += 1.0f / 18.0f;
    self->expansionHeight += 1.0f / 18.0f;
    if (DECR(self->explosionCountdown) == 0) {
        self->actionFunc = EnBubble_Wait;
    }
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderJntSph.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->colliderJntSph.base);
}

static inline void EnBubble_Vec3fNormalize(Vec3f* vec) {
    f32 norm = sqrtf((vec->x * vec->x) + (vec->y * vec->y) + (vec->z * vec->z));

    if (norm != 0.0f) {
        vec->x /= norm;
        vec->y /= norm;
        vec->z /= norm;
    } else {
        vec->x = vec->y = vec->z = 0.0f;
    }
}

static inline void EnBubble_SetDimensions(EnBubble* self, f32 dim) {
    f32 a;
    f32 b;
    f32 c;
    f32 d;

    self->actor.flags |= 1;
    Actor_SetScale(&self->actor, 1.0f);
    self->actor.shape.yOffset = 16.0f;
    self->graphicRotSpeed = 16.0f * 2.0f / 3.0f;
    self->graphicEccentricity = 0.08f;
    self->expansionWidth = dim;
    self->expansionHeight = dim;
    a = Rand_ZeroOne();
    b = Rand_ZeroOne();
    c = Rand_ZeroOne();
    d = (a * a) + (b * b) + (c * c);
    self->unk_274.x = a / d;
    self->unk_274.y = b / d;
    self->unk_274.z = c / d;
    self->unk_290 = 1.0f;
    self->unk_294 = 1.0f;
}

static s32 func_809CBFD4(EnBubble* self) {
    if (DECR(self->explosionCountdown) != 0) {
        return -1;
    }
    EnBubble_SetDimensions(self, -1.0f);
    return 18;
}

void EnBubble_Disappear(EnBubble* self, PlayState* play) {
    s32 temp_v0;

    temp_v0 = func_809CBFD4(self);
    if (temp_v0 >= 0) {
        self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        self->explosionCountdown = temp_v0;
        self->actionFunc = EnBubble_Regrow;
    }
}

// EnBubble_Wait draft: 71..85 words off. ROM inlines DetectPop/AC-hit/StartPop and keeps play (sb), 0 (r6), 1 (r8),
// 12 (sl) and EnBubble_Pop (r7) in callee-saved regs with the hit flag in r1; ours hoists only 1/12 (+1 insn, 456 vs
// 452 B). Tried: ret types, static vs inline, single-exit locals, play2/s2 aliases, flat N64 shape.
#if 0
void FUN_0035e6e0(PlayState* play, Actor* actor, s16 yaw, f32 speedXZ, f32 speedY);

static inline u32 EnBubble_StartPop(EnBubble* self) {
    EnBubbleColliderElement* elem = &self->colliderJntSph.elements[0];

    elem->dmgFlags = 8;
    elem->hitSpecialEffect = 0;
    elem->damage = 4;
    elem->atElemFlags = 1;
    self->actor.velocity.y = 0.0f;
    return 9;
}

static inline u32 EnBubble_AcHit(EnBubble* self) {
    if (((self->colliderJntSph.base.acFlags & 2) != 0) == false) {
        return false;
    }
    self->colliderJntSph.base.acFlags &= ~2;
    if (self->colliderJntSph.elements[1].acElemFlags & 2) {
        self->unk_268.x = self->colliderJntSph.base.ac->velocity.x * 0.1f;
        self->unk_268.y = self->colliderJntSph.base.ac->velocity.y * 0.1f;
        self->unk_268.z = self->colliderJntSph.base.ac->velocity.z * 0.1f;
        self->graphicRotSpeed = 128.0f * 2.0f / 3.0f;
        self->graphicEccentricity = 0.48f;
        return false;
    }
    self->unk_280 = 12;
    return true;
}

static inline u32 EnBubble_DetectPop(EnBubble* self, PlayState* play) {
    if (DECR(self->unk_280) != 0 || self->actionFunc == EnBubble_Pop) {
        return false;
    }
    if (self->colliderJntSph.base.ocFlags2 & 1) {
        s32 damage;

        self->colliderJntSph.base.ocFlags2 &= ~1;
        damage = -self->colliderJntSph.elements[0].damage;
        (*(void (**)(PlayState*, s32))((u8*)play + 0x5BAC))(play, damage);
        FUN_0035e6e0(play, &self->actor, self->actor.yawTowardsPlayer, 6.0f, 6.0f);
        self->unk_280 = 12;
        return true;
    }
    return EnBubble_AcHit(self);
}

void EnBubble_Wait(EnBubble* self, PlayState* play) {
    if (EnBubble_DetectPop(self, play)) {
        self->explosionCountdown = EnBubble_StartPop(self);
        self->actionFunc = EnBubble_Pop;
    } else {
        EnBubble_Fly(self, play);
        self->actor.shape.yOffset = ((self->expansionHeight + 1.0f) * 16.0f);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->colliderJntSph.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->colliderJntSph.base);
    }
}
#endif

GLOBAL_ASM("asm/EnBubble_Wait.s")

void EnBubble_Init(Actor* thisx, PlayState* play) {
    EnBubble* self = (EnBubble*)thisx;

    FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0);
    ActorShape_Init(&self->actor.shape, 16.0f, ActorShadow_DrawCircle, 0.2f);
    Collider_InitJntSph(play, &self->colliderJntSph);
    Collider_SetJntSph(play, &self->colliderJntSph, &self->actor, EnBubble_sJntSphInit,
                       self->colliderJntSphElements);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(9),
                            (CollisionCheckInfoInit2*)EnBubble_sColChkInfoInit2);
    self->actor.naviEnemyId = 0x16;
    self->bounceDirection.x = Rand_ZeroOne();
    self->bounceDirection.y = Rand_ZeroOne();
    self->bounceDirection.z = Rand_ZeroOne();
    EnBubble_Vec3fNormalize(&self->bounceDirection);
    self->velocityFromBounce.x = self->bounceDirection.x * 2.0f;
    self->velocityFromBounce.y = self->bounceDirection.y * 2.0f;
    self->velocityFromBounce.z = self->bounceDirection.z * 2.0f;
    EnBubble_SetDimensions(self, 0.0f);
    self->actionFunc = EnBubble_Wait;
}

// EnBubble_Draw draft: 8 B over ROM (632 vs 624) and many words off. ROM hoists `mov r6,#1` (the SetUnkAC 1) before the
// first scale, keeps ONE 1.0 const (s16) and re-materialises the scale z/y copies from it per call (ours CSEs a second
// copy in s16/s17). Same SetUnkAC family as BgMizuWater_Draw (see CLAUDE.md).
#if 0
void FUN_00371fac(MTX34* mtx, void* arg1);
void Matrix_RotateZ(MTX34* mtx, s32 mode, f32 angle);

static inline void EnBubble_UpdateDim(EnBubble* self, MTX34* mtx) {
    EnBubbleDim* dim = &self->colliderJntSph.elements[0].dim;
    Vec3f src;
    Vec3f dest;

    src.x = dim->modelCenter.x;
    src.y = dim->modelCenter.y;
    src.z = dim->modelCenter.z;
    VEC3TransformAsm((Vector3f*)&dest, mtx, (Vector3f*)&src);
    dim->worldCenter.x = dest.x;
    dim->worldCenter.y = dest.y;
    dim->worldCenter.z = dest.z;
    dim->worldRadius = (s16)(dim->modelRadius * (1.0f + self->expansionWidth));
    self->colliderJntSph.elements[1].dim = *dim;
}

static inline void EnBubble_ScaleMtx(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
}

void EnBubble_Draw(Actor* thisx, PlayState* play) {
    EnBubble* self = (EnBubble*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    if (self->actionFunc != EnBubble_Disappear) {
        FUN_00371fac(&mtx, (u8*)play + 0x2FC);
        EnBubble_ScaleMtx(&mtx, self->expansionWidth + 1.0f, self->expansionHeight + 1.0f, 1.0f);
        Matrix_RotateZ(&mtx, 1, self->graphicRotSpeed * (0.017453292f * (f32)self->frameCount));
        EnBubble_ScaleMtx(&mtx, self->graphicEccentricity + 1.0f, 1.0f, 1.0f);
        Matrix_RotateZ(&mtx, 1, self->graphicRotSpeed * (-0.017453292f * (f32)self->frameCount));
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
    if (self->actionFunc != EnBubble_Disappear) {
        self->actor.shape.shadowScale = (self->expansionWidth + 1.0f) * 0.2f;
        EnBubble_UpdateDim(self, &mtx);
    }
}
#endif

GLOBAL_ASM("asm/EnBubble_Draw.s")

GLOBAL_ASM("asm/EnBubble_Fly.s")

}
