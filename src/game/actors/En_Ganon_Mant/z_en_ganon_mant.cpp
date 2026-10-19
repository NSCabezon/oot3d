extern "C" {
#include "z_en_ganon_mant.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000030

void EnGanonMant_Init(Actor* actor, GameState* state);
void EnGanonMant_Destroy(Actor* actor, GameState* state);
void EnGanonMant_Update(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Ganon_Mant.data.s")

ActorInit En_Ganon_Mant_InitVars = {
    ACTOR_EN_GANON_MANT,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnGanonMant),
    (ActorFunc)EnGanonMant_Init,
    (ActorFunc)EnGanonMant_Destroy,
    (ActorFunc)EnGanonMant_Update,
    NULL,
};

void EnGanonMant_Init(Actor* actor, GameState* state) {
    actor->flags &= ~1;
}

// Object behind the global pointer at 0x0055A1F8 (slot 4 frees a model). Real virtual call: VFE needs
// _ZTI16EnGanonMantFreer (absolute EQU placeholder in src/game/shared_syms.s, no image bytes).
extern "C++" {
class EnGanonMantFreer {
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void vf0C();
    virtual void Free(void* model);
};
}
extern EnGanonMantFreer* gUnk_0055A1F8; // src/game/shared_syms.s

void FUN_00348904(void* mgr, void* model);
void* FUN_003488e4(void* model);
void FUN_003445a8(void* p);

// Inlines the shared GetModelManager() static (guard 0x0055A21C, include/model_manager.hpp).
void EnGanonMant_Destroy(Actor* actor, GameState* state) {
    EnGanonMant* self = (EnGanonMant*)actor;

    if (self->mgrModel != NULL) {
        FUN_00348904(GetModelManager().unk_247C, self->mgrModel);
        self->mgrModel = NULL;
    }
    if (self->model != NULL) {
        void* model = FUN_003488e4(self->model);
        gUnk_0055A1F8->Free(model);
        self->model = NULL;
    }
    FUN_003445a8(self->unk_1768);
}

// EnGanonMant_Update (796 B, 3DS-only setup, sets actor update/draw). Left as asm, NOT matched. Blockers:
// (1) fixed-address static loader guard 0x0055A21C / object 0x005BE5B8 with atexit (as Destroy above);
// (2) the ROM embeds the __FILE__ string "...z_en_ganon_mant.cpp" inside the function text (pc-relative add),
//     which C string literals cannot reproduce; (3) raw vtable calls via DCD blx (vtable[3] on 0x0055A1F8, [4] on Destroy);
//     (4) unknown callees FUN_003432d4, FUN_00348be4, FUN_0034897c, FUN_00348a64, FUN_0032b1c4, FUN_0033e2a0,
//     FUN_0033d200, ZAR_GetCTXBByIndex, Object_IsLoaded; (5) inline struct init at actor+0x17BC (6x u32 + u16 fields).
GLOBAL_ASM("asm/EnGanonMant_Update.s")
}
