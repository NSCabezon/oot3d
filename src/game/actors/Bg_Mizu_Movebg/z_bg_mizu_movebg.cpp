extern "C" {
#include "z_bg_mizu_movebg.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x80400010

#define MOVEBG_TYPE(params) ((u16)(params) >> 12)

void BgMizuMovebg_Init(Actor* actor, GameState* state);
void BgMizuMovebg_Destroy(Actor* actor, GameState* state);
void BgMizuMovebg_Update(Actor* actor, GameState* state);
void BgMizuMovebg_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Mizu_Movebg.data.s")

ActorInit Bg_Mizu_Movebg_InitVars = {
    ACTOR_BG_MIZU_MOVEBG,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuMovebg),
    (ActorFunc)BgMizuMovebg_Init,
    (ActorFunc)BgMizuMovebg_Destroy,
    (ActorFunc)BgMizuMovebg_Update,
    (ActorFunc)BgMizuMovebg_Draw,
};

GLOBAL_ASM("asm/BgMizuMovebg_Init.s")

GLOBAL_ASM("../src/game/actors/Bg_Mizu_Movebg/data_syms.s")
extern u8 BgMizuMovebg_sUnkFlags; // unknown flags (absolute 0x00512FF8, see data_syms.s)

void BgMizuMovebg_Destroy(Actor* actor, GameState* state) {
    BgMizuMovebg* self = (BgMizuMovebg*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    switch (MOVEBG_TYPE(actor->params)) {
        case 3:
        case 4:
        case 5:
        case 6:
            if (self->sfxFlags & 2) {
                BgMizuMovebg_sUnkFlags &= ~2;
            }
            break;

        case 7:
            if (self->sfxFlags & 1) {
                BgMizuMovebg_sUnkFlags &= ~1;
            }
            break;
    }
    FUN_00350f34(actor, &self->skelAnimModel, &self->skelAnimModel2, 0);
}

void BgMizuMovebg_Update(Actor* actor, GameState* state) {
    BgMizuMovebg* self = (BgMizuMovebg*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

extern s32 BgMizuMovebg_sAnimSpeedMask[]; // int table indexed by type, absolute 0x0051303C

void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);

// BgMizuMovebg_Draw: 5/78 words off after recipe R8 (MTX34_MakeTrans). Only the VFP copy chain of the two 0.0f translate
// components differs: ROM y=s0, z=s1 (vmov s1,s17), x=s2 (vmov s2,s1); best here gives x=s2 first, then z=s1. Tried all
// Translate param/store orders (3-, 2-, 1-param, mode-style, Vector3f ctor), zero locals, y local.
#if 0
static inline void BgMizuMovebg_Translate(MTX34* m, f32 y, f32 z, f32 x) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    MTX34_MakeTrans(m, &t);
}

void BgMizuMovebg_Draw(Actor* actor, GameState* state) {
    BgMizuMovebg* self = (BgMizuMovebg*)actor;
    MTX34 modelMtx;
    MTX34 baseMtx;
    f32 alpha;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    alpha = 1.0f;
    if (PauseContext_GetState() != 0) {
        alpha = 0.0f;
    }

    if (self->skelAnimModel != NULL) {
        if (BgMizuMovebg_sAnimSpeedMask[MOVEBG_TYPE(actor->params)] > -1) {
            self->skelAnimModel->unk_0C->SetAnimSpeed(alpha);
        }
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);

        if (self->dList != NULL) {
            MTX34CopyAsm(&baseMtx, &modelMtx);
            BgMizuMovebg_Translate(&modelMtx, self->unk_1E0 - actor->world.pos.y, 0.0f, 0.0f);
            FUN_0036c174(&modelMtx, &modelMtx, &baseMtx);

            self->skelAnimModel2->SetUnkAC(1);
            self->skelAnimModel2->FUN_003721e0(&modelMtx);
            self->skelAnimModel2->FUN_00372170(0);
        }
    }
}
#endif

GLOBAL_ASM("asm/BgMizuMovebg_Draw.s")
}
