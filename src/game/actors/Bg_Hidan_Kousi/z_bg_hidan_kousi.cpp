extern "C" {
#include "z_bg_hidan_kousi.hpp"

#define FLAGS 0x80400010
#define PARAMS_LO(p) ((u16)(p) & 0xFF)
#define PARAMS_HI(p) (((u32)(p) << 16) >> 24)

void BgHidanKousi_Init(Actor* actor, GameState* state);
void BgHidanKousi_Destroy(Actor* actor, GameState* state);
void BgHidanKousi_Update(Actor* actor, GameState* state);
void BgHidanKousi_Draw(Actor* actor, GameState* state);

void FUN_003e2880(BgHidanKousi* self, PlayState* play); // N64: func_80889B5C
void FUN_001980c8(BgHidanKousi* self, PlayState* play); // N64: func_80889BC0
void FUN_0019bdf8(BgHidanKousi* self, PlayState* play); // N64: func_80889C18
void FUN_00176fb8(BgHidanKousi* self, PlayState* play); // N64: func_80889C90
void FUN_0013ebdc(BgHidanKousi* self, PlayState* play); // N64: func_80889D28 (empty)
s32 FUN_0036ae04(void);                                 // func_8005B198 (player category)
void FUN_0036b96c(Actor* actor);                        // Actor_UpdatePos
void OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 arg2, s32 arg3);

// ROM data order: these tables precede InitVars (0x00511A98).
static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static s16 D_80889E7C[] = { 0x4000, -0x4000, -0x4000, 0x0000 };

static f32 D_80889E40[] = { 120.0f, 150.0f, 150.0f };

static s32 sMetalFencesDLs[] = { 0x19, 0x1A, 0x1B };

static s32 sMetalFencesCollisions[] = { 9, 10, 11 };

ActorInit Bg_Hidan_Kousi_InitVars = {
    ACTOR_BG_HIDAN_KOUSI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanKousi),
    (ActorFunc)BgHidanKousi_Init,
    (ActorFunc)BgHidanKousi_Destroy,
    (ActorFunc)BgHidanKousi_Update,
    (ActorFunc)BgHidanKousi_Draw,
};

// N64: func_80889ACC. Going through a local Actor* (not self->dyna.actor / a Vec3s* rot) gives the
// extra self copy register seen in FUN_00176fb8.
static inline void BgHidanKousi_SetOpenPos(BgHidanKousi* self) {
    Actor* thisx = &self->dyna.actor;
    f32 temp1 = D_80889E40[PARAMS_LO(thisx->params)] * Math_SinS(thisx->world.rot.y);
    f32 temp2 = D_80889E40[PARAMS_LO(thisx->params)] * Math_CosS(thisx->world.rot.y);

    thisx->world.pos.x = thisx->home.pos.x + temp1;
    thisx->world.pos.z = thisx->home.pos.z + temp2;
}

// ReadCollisionHeaderFromZsi is a real call (armlink inlines it into `str r0, [r1]`).
void BgHidanKousi_Init(Actor* thisx, GameState* state) {
    BgHidanKousi* self = (BgHidanKousi*)thisx;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    Actor_SetFocus(thisx, 50.0f);
    Actor_ProcessInitChain(thisx, sInitChain);
    FUN_00372f38(thisx, play, &self->skelAnimModel, sMetalFencesDLs[PARAMS_LO(thisx->params)], 0);
    ReadCollisionHeaderFromZsi(
        DynaPolyInfo_Alloc(&self->dyna, state, sMetalFencesCollisions[PARAMS_LO(thisx->params)]),
        &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    thisx->world.rot.y = thisx->shape.rot.y + D_80889E7C[PARAMS_LO(thisx->params)];
    if (Flags_GetSwitch(play, PARAMS_HI(thisx->params))) {
        BgHidanKousi_SetOpenPos(self);
        self->actionFunc = FUN_0013ebdc;
    } else {
        self->actionFunc = FUN_003e2880;
    }
}

void FUN_0013ebdc(BgHidanKousi* self, PlayState* play) {
}

void FUN_003e2880(BgHidanKousi* self, PlayState* play) {
    if (Flags_GetSwitch(play, PARAMS_HI(self->dyna.actor.params))) {
        self->actionFunc = FUN_001980c8;
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        self->unk_168 = 300;
    }
}

void FUN_001980c8(BgHidanKousi* self, PlayState* play) {
    self->unk_168--;
    if (self->dyna.actor.type == FUN_0036ae04() || self->unk_168 <= 0) {
        self->actionFunc = FUN_0019bdf8;
    }
}

void FUN_0019bdf8(BgHidanKousi* self, PlayState* play) {
    Actor* actor = &self->dyna.actor;

    actor->speedXZ += 0.2f;
    if (actor->speedXZ > 2.0f) {
        actor->speedXZ = 2.0f;
        self->actionFunc = FUN_00176fb8;
    }
    Actor_MoveForward(actor);
    FUN_00373264(actor, 0x0100018D);
}

void FUN_00176fb8(BgHidanKousi* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;
    Vec3f* home = &thisx->home.pos;
    Vec3f* world = &thisx->world.pos;

    FUN_0036b96c(thisx);
    if (D_80889E40[PARAMS_LO(thisx->params)] < Math_Vec3f_DistXYZ(home, world)) {
        BgHidanKousi_SetOpenPos(self);
        self->actionFunc = FUN_0013ebdc;
        Audio_PlayActorSound2(thisx, 0x0100018E);
    } else {
        FUN_00373264(thisx, 0x0100018D);
    }
}

void BgHidanKousi_Destroy(Actor* actor, GameState* state) {
    BgHidanKousi* self = (BgHidanKousi*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgHidanKousi_Update(Actor* actor, GameState* state) {
    BgHidanKousi* self = (BgHidanKousi*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgHidanKousi_Draw(Actor* actor, GameState* state) {
    BgHidanKousi* self = (BgHidanKousi*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
