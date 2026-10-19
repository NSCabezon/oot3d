extern "C" {
#include "z_eff_dust.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000030

void EffDust_Init(Actor* actor, GameState* state);
void EffDust_Destroy(Actor* actor, GameState* state);
void EffDust_Update(Actor* actor, GameState* state);
void EffDust_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Eff_Dust.data.s")

ActorInit Eff_Dust_InitVars = {
    ACTOR_EFF_DUST,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EffDust),
    (ActorFunc)EffDust_Init,
    (ActorFunc)EffDust_Destroy,
    (ActorFunc)EffDust_Update,
    (ActorFunc)EffDust_Draw,
};

GLOBAL_ASM("asm/EffDust_Init.s")

// Object behind the global pointer at 0x0055A1F8 (slot 4 frees a model). Real virtual call: VFE needs
// _ZTI12EffDustFreer (absolute EQU placeholder in syms.s, no image bytes).
extern "C++" {
class EffDustFreer {
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void vf0C();
    virtual void Free(void* model);
};
}
extern EffDustFreer* gUnk_0055A1F8; // syms.s

void FUN_00348904(void* mgr, void* model);
void* FUN_003488e4(void* model);

// Inlines the shared GetModelManager() static (guard 0x0055A21C, include/model_manager.hpp).
void EffDust_Destroy(Actor* actor, GameState* state) {
    EffDust* self = (EffDust*)actor;
    s32 i;

    for (i = 0; i < 1; i++) {
        FUN_00348904(GetModelManager().unk_247C, self->unk_5C0[i + 1]);
        self->unk_5C0[i + 1] = NULL;
        if (self->unk_5C0[i] != NULL) {
            void* model = FUN_003488e4(self->unk_5C0[i]);
            gUnk_0055A1F8->Free(model);
        }
        self->unk_5C0[i] = NULL;
    }
}

void EffDust_Update(Actor* actor, GameState* state) {
    EffDust* self = (EffDust*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void EffDust_Draw(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;

    ((EffDust*)actor)->drawFunc(actor, play);
}
}
