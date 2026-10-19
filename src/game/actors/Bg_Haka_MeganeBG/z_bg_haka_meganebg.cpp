extern "C" {
#include "z_bg_haka_meganebg.hpp"

#define FLAGS 0x80400000

void BgHakaMeganeBG_Init(Actor* actor, GameState* state);
void BgHakaMeganeBG_Destroy(Actor* actor, GameState* state);
void BgHakaMeganeBG_Update(Actor* actor, GameState* state);
void BgHakaMeganeBG_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Haka_Megane_BG.data.s")

ActorInit Bg_Haka_MeganeBG_InitVars = {
    ACTOR_BG_HAKA_MEGANEBG,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKA_OBJECTS,
    sizeof(BgHakaMeganeBG),
    (ActorFunc)BgHakaMeganeBG_Init,
    (ActorFunc)BgHakaMeganeBG_Destroy,
    (ActorFunc)BgHakaMeganeBG_Update,
    (ActorFunc)BgHakaMeganeBG_Draw,
};

// Init: still asm (draft below, under #if 0). A C Init changes the link layout (non-function bytes differ).
GLOBAL_ASM("asm/BgHakaMeganeBG_Init.s")

// 6-arg form of the model free

void BgHakaMeganeBG_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgHakaMeganeBG* self = (BgHakaMeganeBG*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(&self->dyna.actor, &self->skelAnimModel[0],
                                                 &self->skelAnimModel[1], &self->skelAnimModel[2],
                                                 &self->skelAnimModel[3], 0);
}

void BgHakaMeganeBG_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgHakaMeganeBG* self = (BgHakaMeganeBG*)thisx;

    self->actionFunc(self, play);
}

// Draw: still asm (3-word register-scheduling diff in C draft, below).
GLOBAL_ASM("asm/BgHakaMeganeBG_Draw.s")

#if 0 // Drafts for asm-left functions (not compiled; see STATUS.md)
// Actor fields without repo names (3DS offsets)
#define CULLING_VOLUME_SCALE(a) (*(f32*)((u8*)(a) + 0x100))
#define CULLING_VOLUME_DOWNWARD(a) (*(f32*)((u8*)(a) + 0x104))
#define DYNA_POS 1
#define DYNA_POS_ROT_Y 3
#define FLAG_CULLING_DISABLED 0x10
#define FLAG_REACT_TO_LENS 0x80

// Action functions still in asm (functions_*.cpp)
void FUN_0019d0d4(BgHakaMeganeBG* self, PlayState* play); // hidden moving platform idle
void FUN_001779b0(BgHakaMeganeBG* self, PlayState* play); // chained elevator drop
void FUN_001e9a08(BgHakaMeganeBG* self, PlayState* play); // rotating platform spin
void FUN_002597d4(BgHakaMeganeBG* self, PlayState* play); // gate: do nothing
void FUN_002597d8(BgHakaMeganeBG* self, PlayState* play); // gate: wait for switch flag

// 11-arg form: four model slots (ptr, index) pairs plus a trailing arg
static InitChainEntry sInitChain[] = {
    { ICHAIN_CONTINUE, ICHAINTYPE_F32, 0x100, 1000 }, // cullingVolumeScale
    { ICHAIN_CONTINUE, ICHAINTYPE_F32, 0x104, 1000 }, // cullingVolumeDownward
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHakaMeganeBG_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgHakaMeganeBG* self = (BgHakaMeganeBG*)thisx;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    self->switchFlag = (u8)(thisx->params >> 8);
    thisx->params &= 0xFF;

    FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel[0], 0x11,
                                                 &self->skelAnimModel[1], 0x13, &self->skelAnimModel[2], 0x15,
                                                 &self->skelAnimModel[3], 0, 0);

    if (thisx->params == 2) {
        DynaPolyActor_Init(&self->dyna, DYNA_POS_ROT_Y);
        thisx->flags |= FLAG_CULLING_DISABLED;
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0xF), &colHeader);
        CULLING_VOLUME_DOWNWARD(thisx) += 500.0f;
        self->actionFunc = FUN_001e9a08;
    } else {
        DynaPolyActor_Init(&self->dyna, DYNA_POS);

        if (thisx->params == 0) {
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0xB), &colHeader);
            thisx->flags |= FLAG_REACT_TO_LENS;
            self->timer = 30;
            self->actionFunc = FUN_0019d0d4;
        } else if (thisx->params == 3) {
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
            thisx->home.pos.y += 100.0f;

            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->actionFunc = FUN_002597d4;
                thisx->world.pos.y = thisx->home.pos.y;
            } else {
                thisx->flags |= FLAG_CULLING_DISABLED;
                self->actionFunc = FUN_002597d8;
            }
        } else {
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0xD), &colHeader);
            self->timer = 120;
            self->actionFunc = FUN_001779b0;
            CULLING_VOLUME_SCALE(thisx) = 3000.0f;
            CULLING_VOLUME_DOWNWARD(thisx) = 3000.0f;
        }
    }

    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
}

void BgHakaMeganeBG_Draw(Actor* thisx, GameState* state) {
    BgHakaMeganeBG* self = (BgHakaMeganeBG*)thisx;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->dyna.actor.params == 0) {
        if (self->skelAnimModel[0] != NULL) {
            self->skelAnimModel[0]->SetUnkAC(1);
            self->skelAnimModel[0]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[0]->FUN_00372170(0);
        }
    } else if (self->skelAnimModel[self->dyna.actor.params] != NULL) {
        self->skelAnimModel[self->dyna.actor.params]->SetUnkAC(1);
        self->skelAnimModel[self->dyna.actor.params]->FUN_003721e0(&modelMtx);
        self->skelAnimModel[self->dyna.actor.params]->FUN_00372170(0);
    }
}
#endif
}
