extern "C" {
#include "z_bg_jya_bigmirror.hpp"

#define FLAGS 0x80400030

void BgJyaBigmirror_Init(Actor* actor, GameState* state);
void BgJyaBigmirror_Destroy(Actor* actor, GameState* state);
void BgJyaBigmirror_Update(Actor* actor, GameState* state);
void BgJyaBigmirror_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Jya_Bigmirror.data.s")

ActorInit Bg_Jya_Bigmirror_InitVars = {
    ACTOR_BG_JYA_BIGMIRROR,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaBigmirror),
    (ActorFunc)BgJyaBigmirror_Init,
    (ActorFunc)BgJyaBigmirror_Destroy,
    (ActorFunc)BgJyaBigmirror_Update,
    (ActorFunc)BgJyaBigmirror_Draw,
};

GLOBAL_ASM("asm/BgJyaBigmirror_Init.s")

GLOBAL_ASM("../src/game/actors/Bg_Jya_Bigmirror/syms.s")
// Empty C stub in z3D.cpp (linker-inlined to `mov r0,r0`); viewed here as returning its arg.
void* BgJyaBigmirror_Stub(void* obj) __asm("GraphicsContext_UnknownStub");
extern u8 BgJyaBigmirror_sSpawned;

// Manager at *0x0055A1A8: real virtual call, slot 4 (+0x10). _ZTI21BgJyaBigmirrorManager is an EQU in src/game/shared_syms.s so
// armlink VFE (--vfemode=force) knows the call class (same recipe as Item_Ocarina).
extern "C++" {
struct BgJyaBigmirrorManager {
    virtual void unk_vf0();
    virtual void unk_vf1();
    virtual void unk_vf2();
    virtual void unk_vf3();
    virtual void Remove(void* obj);
};
}
extern BgJyaBigmirrorManager* gUnk_0055A1A8;

void BgJyaBigmirror_Destroy(Actor* actor, GameState* state) {
    BgJyaBigmirror* self = (BgJyaBigmirror*)actor;

    if (self->spawned != 0) {
        BgJyaBigmirror_sSpawned = 0;
    }
    FUN_00350f34(actor, &self->skelAnimModel1, &self->skelAnimModel2, 0);
    FUN_00350f34(actor, &self->skelAnimModel3, &self->skelAnimModel4, 0);
    if (self->object != NULL) {
        void* obj = BgJyaBigmirror_Stub(self->object);
        gUnk_0055A1A8->Remove(obj);
    }
    self->object = NULL;
}

GLOBAL_ASM("asm/BgJyaBigmirror_Update.s")

GLOBAL_ASM("asm/BgJyaBigmirror_Draw.s")
}
