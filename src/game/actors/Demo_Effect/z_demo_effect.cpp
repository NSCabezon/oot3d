extern "C" {
#include "z_demo_effect.hpp"
#include "model_manager.hpp"
#include "graphics/skeleton_animation_model.hpp"

#define FLAGS 0x00000030

void DemoEffect_Init(Actor* actor, GameState* state);
void DemoEffect_Destroy(Actor* thisx, PlayState* play);
void DemoEffect_Update(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_Demo_Effect.data.s")

ActorInit Demo_Effect_InitVars = {
    ACTOR_DEMO_EFFECT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(DemoEffect),
    (ActorFunc)DemoEffect_Init,
    (ActorFunc)DemoEffect_Destroy,
    (ActorFunc)DemoEffect_Update,
    NULL,
};

GLOBAL_ASM("asm/DemoEffect_Init.s")

extern "C++" {
class DemoEffectModel {
public:
    virtual void unk_vf0();
    virtual void Destroy();
};
class EffDustFreer {
public:
    virtual void vf00();
    virtual void vf04();
    virtual void* Alloc(s32 size);
    virtual void vf0C();
    virtual void Free(void* model);
};
}
extern EffDustFreer* gUnk_0055A1C0; // src/game/shared_syms.s
extern EffDustFreer* gUnk_0055A1F8; // src/game/shared_syms.s

void FUN_0033579c(void* curve, PlayState* play);
void* FUN_003685a0(void* arg);
void FUN_00348904(void* mgr, void* model);
void* FUN_003488e4(void* model);

void DemoEffect_Destroy(Actor* thisx, PlayState* play) {
    DemoEffect* self = (DemoEffect*)thisx;
    s32 effectType = (u8)self->actor.params;
    s32 i;

    if (effectType == 0xF || effectType == 0x18 || effectType == 0x19) {
        FUN_0033579c(&self->unk_1A4, play);
    }
    for (i = 0; i < 3; i++) {
        if (self->unk_2C0[i] != NULL) {
            void* model = FUN_003685a0(self->unk_2C0[i]);

            gUnk_0055A1C0->Free(model);
            self->unk_2C0[i] = NULL;
        }
    }
    for (i = 0; i < 3; i++) {
        if (self->unk_2A8[i] != NULL) {
            ((DemoEffectModel*)self->unk_2A8[i])->Destroy();
            self->unk_2A8[i] = NULL;
        }
    }
    for (i = 0; i < 2; i++) {
        if (self->unk_2D0[i] != NULL) {
            FUN_00348904(GetModelManager().unk_247C, self->unk_2D0[i]);
            self->unk_2D0[i] = NULL;
        }
    }
    if (self->unk_2CC != NULL) {
        void* model = FUN_003488e4(self->unk_2CC);

        gUnk_0055A1F8->Free(model);
        self->unk_2CC = NULL;
    }
}

void DemoEffect_Update(Actor* thisx, PlayState* play) {
    DemoEffect* self = (DemoEffect*)thisx;

    self->updateFunc(self, play);
}

// DRAFT (not matching, 16 words off at 0x1C..): ROM hoists `mov r1,#1` (the unk_AC value) above the
// cutscene-state branch and keeps the model in r0 for the strb; every variant tried (helper SetUnkAC, local
// model/one, goto/u8 csState, sub-struct at +0x200) keeps the model in r1 instead.
#if 0
struct DemoEffectDrawSub {
    /* 0x000 */ char pad_000[0x90];
    /* 0x090 */ u16 flags;
    /* 0x092 */ s16 csIndex;
};

void DemoEffect_DrawSpiritualStones(Actor* thisx, PlayState* play) {
    DemoEffect* self = (DemoEffect*)thisx;
    DemoEffectDrawSub* sub = (DemoEffectDrawSub*)((char*)self + 0x200);

    if (play->GetCutsceneState() != 0) {
        CsCmdActorAction* npcAction = play->csCtx.npcActions[sub->csIndex];

        if (npcAction != NULL && npcAction->action == 1) {
            return;
        }
    }
    if (sub->flags & 1) {
        return;
    }
    ((SkeletonAnimationModel*)self->unk_2A8[0])->unk_AC = 1;
    ((SkeletonAnimationModel*)self->unk_2A8[0])->FUN_003721e0(&self->actor.modelMtx);
    ((SkeletonAnimationModel*)self->unk_2A8[0])->FUN_00372170(0);
}
#endif

GLOBAL_ASM("asm/DemoEffect_DrawSpiritualStones.s")

}
