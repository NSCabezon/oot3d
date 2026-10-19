extern "C" {
#include "z_bg_jya_bombchuiwa.hpp"

#define FLAGS 0x00400001

void BgJyaBombchuiwa_Init(Actor* actor, GameState* state);
void BgJyaBombchuiwa_Destroy(Actor* actor, PlayState* play);
void BgJyaBombchuiwa_Update(Actor* actor, PlayState* play);
void BgJyaBombchuiwa_Draw(Actor* actor, GameState* state);
void FUN_003bce94(BgJyaBombchuiwa* self, PlayState* play);
void Collider_DestroyJntSph(PlayState* play, BgJyaBombchuiwaColliderJntSph* collider);

GLOBAL_ASM("data/z_Bg_Jya_Bombchuiwa.data.s")

ActorInit Bg_Jya_Bombchuiwa_InitVars = {
    ACTOR_BG_JYA_BOMBCHUIWA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaBombchuiwa),
    (ActorFunc)BgJyaBombchuiwa_Init,
    (ActorFunc)BgJyaBombchuiwa_Destroy,
    (ActorFunc)BgJyaBombchuiwa_Update,
    (ActorFunc)BgJyaBombchuiwa_Draw,
};

GLOBAL_ASM("asm/BgJyaBombchuiwa_Init.s")


// Manager at *0x0055A1A8 (same global as Bg_Jya_Bigmirror). _ZTI21BgJyaBigmirrorManager (shared_syms.s) arms armlink
// VFE for this slot-4 call. Call through a typed member, as in Bg_Jya_Bigmirror.
extern "C++" {
struct BgJyaBigmirrorManager {
    virtual void unk_vf0();
    virtual void unk_vf1();
    virtual void unk_vf2();
    virtual void unk_vf3();
    virtual void Remove(void* obj);
};
}
extern BgJyaBigmirrorManager* gUnk_0055A1A8; // *0x0055A1A8, shared with Bg_Jya_Bigmirror
void* BgJyaBombchuiwa_Stub(void* obj) __asm("GraphicsContext_UnknownStub");

void BgJyaBombchuiwa_Destroy(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    BgJyaBombchuiwa* self = (BgJyaBombchuiwa*)thisx;

    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(thisx, &self->skelAnimModel0, 0);
    FUN_00350f34(thisx, &self->skelAnimModel1, &self->skelAnimModel2,
                                                              &self->skelAnimModel3, 0);
    if (self->unk_230 != NULL) {
        void* obj = BgJyaBombchuiwa_Stub(self->unk_230);
        gUnk_0055A1A8->Remove(obj);
    }
    self->unk_230 = NULL;
}

void BgJyaBombchuiwa_Update(Actor* thisx, PlayState* play) {
    BgJyaBombchuiwa* self = (BgJyaBombchuiwa*)thisx;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}

static inline void BgJyaBombchuiwa_SpawnLightRay(BgJyaBombchuiwa* self, PlayState* play) {
    self->actionFunc = NULL;
    self->lightRayIntensity = 153.0f;
    self->drawFlags &= ~7;
    self->drawFlags |= 4;
    self->actor.flags &= ~1;
    Actor_Spawn(&play->actorCtx, play, ACTOR_MIR_RAY, self->actor.world.pos.x, self->actor.world.pos.y,
                self->actor.world.pos.z, 0, 0, 0, 0, 1);
}

// func_808949B8
void FUN_003bce94(BgJyaBombchuiwa* self, PlayState* play) {
    self->timer++;
    if (self->timer & 4) {
        FUN_0037378c(play, &self->actor.world.pos, 60.0f, 3, 100, 100, 0);
    }
    if (Math_StepToF(&self->lightRayIntensity, 1.0f, 0.028f)) {
        BgJyaBombchuiwa_SpawnLightRay(self, play);
    }
}

GLOBAL_ASM("asm/BgJyaBombchuiwa_Draw.s")

GLOBAL_ASM("asm/BgJyaBombchuiwa_WaitForExplosion.s")

}
