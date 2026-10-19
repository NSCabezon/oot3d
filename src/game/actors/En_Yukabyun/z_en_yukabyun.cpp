extern "C" {
#include "z_en_yukabyun.hpp"

#define FLAGS 0x00400010

void EnYukabyun_Init(EnYukabyun* self, PlayState* play);
void EnYukabyun_Destroy(EnYukabyun* self, PlayState* play);
void EnYukabyun_Update(EnYukabyun* self, PlayState* play);
void EnYukabyun_Draw(EnYukabyun* self, PlayState* play);

void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_003e6880(EnYukabyun* self, PlayState* play); // N64: func_80B43A94, GLOBAL_ASM in functions_3E0000s.cpp

GLOBAL_ASM("data/z_En_Yukabyun.data.s")

ActorInit En_Yukabyun_InitVars = {
    ACTOR_EN_YUKABYUN,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_YUKABYUN,
    sizeof(EnYukabyun),
    (ActorFunc)EnYukabyun_Init,
    (ActorFunc)EnYukabyun_Destroy,
    (ActorFunc)EnYukabyun_Update,
    (ActorFunc)EnYukabyun_Draw,
};

// N64 EnYukabyun_Init. Init chain / cylinder init live in the data dump at 0x530658 / 0x530660.
void EnYukabyun_Init(EnYukabyun* self, PlayState* play) {
    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)0x00530658);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.4f);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel, 1, 0);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, (ColliderCylinderInit*)0x00530660);
    self->modelIdx = 0;
    self->actor.params++;
    self->timer = (s16)((f32)((u8)self->actor.params * 0xA + 0x14) * 3.0f * 0.5f + 0.5f);
    self->actionFunc = FUN_003e6880;
}

// N64 EnYukabyun_Destroy (3DS build only frees the model)
void EnYukabyun_Destroy(EnYukabyun* self, PlayState* play) {
    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(&self->actor, self->skelAnimModel, 0);
}

void EnYukabyun_Draw(EnYukabyun* self, PlayState* play) {
    SkeletonAnimationModel* model;

    // Empty stub on 3DS; the linker inlines it to a nop but the arg load stays.
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    model = self->skelAnimModel;
    ((u8*)model)[0xAC] = 1;
    model->FUN_003721e0(&self->actor.modelMtx);
    model->FUN_00372170(0);
}

// Draft, 3 words differ (0x4, 0x14, 0x18): Wait action (N64 func_80B43A94), still GLOBAL_ASM in functions_3E0000s.cpp.
#if 0
#define YUKABYUN_ATTENTION_HOSTILE_QUAKE 0x1005
void FUN_003e6880(EnYukabyun* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
        if (self->timer != 0) {
            return;
        }
    }
    self->actor.flags |= YUKABYUN_ATTENTION_HOSTILE_QUAKE;
    self->actionFunc = FUN_003b9e3c;
}
#endif

// Draft from N64 EnYukabyun_Update, NOT matching (asm uses EnTuboTrap_WaitForProximity as the
// rise/fall action, a flying-away sfx/effect call FUN_003741e4 and raw collider offsets). Disabled.
#if 0
void EnYukabyun_Update(EnYukabyun* self, PlayState* play) {
    if ((self->collider.base.atFlags & AT_HIT) || (self->collider.base.acFlags & AC_HIT) ||
        ((self->collider.base.ocFlags1 & OC1_HIT) && !(self->collider.base.oc->id == ACTOR_EN_YUKABYUN))) {
        self->collider.base.atFlags &= ~AT_HIT;
        self->collider.base.acFlags &= ~AC_HIT;
        self->collider.base.ocFlags1 &= ~OC1_HIT;
        self->actor.flags &= ~(ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE);
        self->actionFunc = FUN_00107054; // EnYukabyun_Break
    }
    self->actionFunc(self, play);
    Actor_MoveForward(&self->actor);
    if (self->actionFunc != FUN_003e6880 && self->actionFunc != FUN_00107054) {
        Actor_UpdateBgCheckInfo(play, &self->actor, 5.0f, 20.0f, 8.0f, 0x1 | 0x4);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        self->actor.flags |= 0x1000000;
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
    Actor_SetFocus(&self->actor, 4.0f);
}
#endif

GLOBAL_ASM("asm/EnYukabyun_Update.s")

}
