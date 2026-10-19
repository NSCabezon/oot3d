extern "C" {
#include "z_bg_hidan_kowarerukabe.hpp"

#define FLAGS 0x80400030

void BgHidanKowarerukabe_Init(Actor* actor, PlayState* play);
void BgHidanKowarerukabe_Destroy(Actor* actor, GameState* state);
void BgHidanKowarerukabe_Update(Actor* actor, GameState* state);
void BgHidanKowarerukabe_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Hidan_Kowarerukabe.data.s")

ActorInit Bg_Hidan_Kowarerukabe_InitVars = {
    ACTOR_BG_HIDAN_KOWARERUKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanKowarerukabe),
    (ActorFunc)BgHidanKowarerukabe_Init,
    (ActorFunc)BgHidanKowarerukabe_Destroy,
    (ActorFunc)BgHidanKowarerukabe_Update,
    (ActorFunc)BgHidanKowarerukabe_Draw,
};

#if 0 // Init draft: 20 words off (only r0/r1 swap in the table-ladder tail: ROM type in r0 / base in r1, ours reversed; ~25 variants tried).
// Needs syms.s (EQUs for the data tables; file exists in this dir) wired via GLOBAL_ASM("../src/game/actors/Bg_Hidan_Kowarerukabe/syms.s").

extern s32 BgHidanKowarerukabe_sModelIdx[];
extern s32 BgHidanKowarerukabe_sDynaInfoIdx[];
extern u8 BgHidanKowarerukabe_sJntSphInit[];
extern InitChainEntry BgHidanKowarerukabe_sInitChain[];
// Sphere radii (s16[5]) @+0, sphere Y (s16[5]) @+0xA, Y offsets (f32[5]) @+0x14: one base in the ROM.
extern s16 BgHidanKowarerukabe_sTable[];
void Collider_InitJntSph(PlayState* play, BgHidanKowarerukabeColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgHidanKowarerukabeColliderJntSph* collider, Actor* actor, void* init,
                        BgHidanKowarerukabeColliderElement* elements);

#define TYPE(a) ((u16)(a)->params & 0xFF)

void BgHidanKowarerukabe_Init(Actor* actor, PlayState* play) {
    BgHidanKowarerukabe* self = (BgHidanKowarerukabe*)actor;

    FUN_00372f38(actor, play, &self->skelAnimModel, BgHidanKowarerukabe_sModelIdx[TYPE(actor)], 0);
    {
        CollisionHeader* colHeader = NULL;
        if (BgHidanKowarerukabe_sDynaInfoIdx[TYPE(actor)] <= -1) {
            self->dyna.bgId = -1;
        } else {
            DynaPolyActor_Init(&self->dyna, 0);
            ReadCollisionHeaderFromZsi(
                DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, BgHidanKowarerukabe_sDynaInfoIdx[TYPE(actor)]),
                &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
        }
    }

    {
        s16 params = actor->params;
        if ((u32)(params & 0xFF) >= 5 || Flags_GetSwitch(play, (params >> 8) & 0x3F)) {
            Actor_Kill(actor);
            return;
        }
    }

    Actor_ProcessInitChain(actor, BgHidanKowarerukabe_sInitChain);
    Actor_SetScale(actor, 0.1f);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, actor, BgHidanKowarerukabe_sJntSphInit, self->colliderElements);
    {
        s16* tbl = BgHidanKowarerukabe_sTable;
        self->collider.elements[0].modelRadius = tbl[TYPE(actor)];
        tbl += 5;
        self->collider.elements[0].modelCenterY = tbl[TYPE(actor)];
        tbl += 5;
        actor->world.pos.y = actor->home.pos.y + ((f32*)tbl)[TYPE(actor)];
    }
    actor->unk_19B = 2;
}
#endif
GLOBAL_ASM("asm/BgHidanKowarerukabe_Init.s")

void Collider_DestroyJntSph(PlayState* play, BgHidanKowarerukabeColliderJntSph* collider);
void Collider_UpdateSpheres(s32 index, BgHidanKowarerukabeColliderJntSph* collider, MTX34* mtx);

void BgHidanKowarerukabe_Destroy(Actor* actor, GameState* state) {
    BgHidanKowarerukabe* self = (BgHidanKowarerukabe*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

GLOBAL_ASM("asm/BgHidanKowarerukabe_Update.s")

void BgHidanKowarerukabe_Draw(Actor* actor, GameState* state) {
    BgHidanKowarerukabe* self = (BgHidanKowarerukabe*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
        Collider_UpdateSpheres(0, &self->collider, &mtx);
    }
}
}
