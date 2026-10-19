extern "C" {
#include "z_bg_spot08_iceblock.hpp"

#define FLAGS 0x80400000

void BgSpot08Iceblock_Init(Actor* actor, GameState* state);
void BgSpot08Iceblock_Destroy(Actor* actor, GameState* state);
void BgSpot08Iceblock_Update(Actor* actor, GameState* state);
void BgSpot08Iceblock_Draw(Actor* actor, GameState* state);

s16 Rand_S16Offset(s16 base, s16 range);

GLOBAL_ASM("data/z_Bg_Spot08_Iceblock.data.s")

ActorInit Bg_Spot08_Iceblock_InitVars = {
    ACTOR_BG_SPOT08_ICEBLOCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT08_OBJ,
    sizeof(BgSpot08Iceblock),
    (ActorFunc)BgSpot08Iceblock_Init,
    (ActorFunc)BgSpot08Iceblock_Destroy,
    (ActorFunc)BgSpot08Iceblock_Update,
    (ActorFunc)BgSpot08Iceblock_Draw,
};

void FUN_003bcd9c(BgSpot08Iceblock* self, PlayState* play); // FloatNonrotating
void FUN_003b35c8(BgSpot08Iceblock* self, PlayState* play); // FloatRotating
void FUN_003c0460(BgSpot08Iceblock* self, PlayState* play); // FloatOrbitingTwins
extern InitChainEntry BgSpot08Iceblock_sInitChain[];
#define sInitChain BgSpot08Iceblock_sInitChain

GLOBAL_ASM("../src/game/actors/Bg_Spot08_Iceblock/data_syms.s")

static void BgSpot08Iceblock_InitDynaPoly(BgSpot08Iceblock* self, PlayState* play, void* collision, s32 flags) {
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, flags);
    ReadCollisionHeaderFromZsi(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
}

static void BgSpot08Iceblock_SpawnTwinFloe(BgSpot08Iceblock* self, PlayState* play) {
    Actor* actor = &self->dyna.actor;
    f32 sin = Math_SinS(self->dyna.actor.home.rot.y) * 100.0f;
    f32 cos = Math_CosS(self->dyna.actor.home.rot.y) * 100.0f;

    if (!(self->dyna.actor.params & 0x100)) {
        {
            PlayState* play2 = play; // block-scoped alias: fixes r1/r2 arg setup order
            Actor_SpawnAsChild(&play2->actorCtx, &self->dyna.actor, play2, ACTOR_BG_SPOT08_ICEBLOCK,
                               self->dyna.actor.home.pos.x, self->dyna.actor.home.pos.y, self->dyna.actor.home.pos.z,
                               self->dyna.actor.home.rot.x, self->dyna.actor.home.rot.y, self->dyna.actor.home.rot.z,
                               0x123);
        }
        self->dyna.actor.world.pos.x += sin;
        actor->world.pos.z += cos;
    } else {
        self->dyna.actor.world.pos.x -= sin;
        self->dyna.actor.world.pos.z -= cos;
    }
    self->actionFunc = FUN_003c0460;
}

void BgSpot08Iceblock_Init(Actor* actor, GameState* state) {
    BgSpot08Iceblock* self = (BgSpot08Iceblock*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    switch (self->dyna.actor.params & 0xFF) {
        case 0xFF:
            self->dyna.actor.params = 0x10;
            break;
        default:
            self->dyna.actor.params = 0x10;
            break;
        case 1:
        case 4:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x14:
        case 0x20:
        case 0x23:
        case 0x24:
            break;
    }

    FUN_00372f38(actor, play, &self->skelAnimModel1, 1, &self->skelAnimModel2, 0, 0);

    switch (self->dyna.actor.params & 0x200) {
        case 0:
            colHeader = (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, state, 1);
            break;
        case 0x200:
            colHeader = (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, state, 0);
            break;
    }

    u16 t = self->dyna.actor.params;
    switch (t & 0xF) {
        case 2:
        case 3:
            BgSpot08Iceblock_InitDynaPoly(self, play, colHeader, 3);
            break;
        default:
            BgSpot08Iceblock_InitDynaPoly(self, play, colHeader, 0);
            break;
    }

    if (gSaveContext.linkAge != 0) {
        Actor_Kill(actor);
        return;
    }

    Actor_ProcessInitChain(actor, sInitChain);

    switch (self->dyna.actor.params & 0xF0) {
        case 0:
            Actor_SetScale(actor, 0.2f);
            break;
        case 0x10:
            Actor_SetScale(actor, 0.1f);
            break;
        case 0x20:
            Actor_SetScale(actor, 0.05f);
            break;
    }

    self->bobPhaseSlow = (s32)(Rand_ZeroOne() * (0xFFFF + 0.5f));
    self->bobPhaseFast = (s32)(Rand_ZeroOne() * (0xFFFF + 0.5f));
    self->unk_1C4 = 1.0f;
    self->unk_1D8 = 1.0f;

    switch (self->dyna.actor.params & 0xF) {
        case 0:
        case 1:
            self->actionFunc = FUN_003bcd9c;
            break;
        case 2:
            self->actionFunc = FUN_003b35c8;
            break;
        case 3:
            BgSpot08Iceblock_SpawnTwinFloe(self, play);
            break;
        case 4:
            self->actionFunc = NULL;
            break;
    }
}


void BgSpot08Iceblock_Destroy(Actor* actor, GameState* state) {
    BgSpot08Iceblock* self = (BgSpot08Iceblock*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel1, &self->skelAnimModel2, 0);
}

void BgSpot08Iceblock_Update(Actor* actor, GameState* state) {
    BgSpot08Iceblock* self = (BgSpot08Iceblock*)actor;
    PlayState* play = (PlayState*)state;

    if (Rand_ZeroOne() < 0.05f) {
        self->bobIncrSlow = Rand_S16Offset(300, 100);
        self->bobIncrFast = Rand_S16Offset(800, 400);
    }

    self->bobPhaseSlow += (self->bobIncrSlow > 0) ? (s32)(0.5f + (self->bobIncrSlow * 2.0f / 3.0f))
                                                  : (s32)((self->bobIncrSlow * 2.0f / 3.0f) - 0.5f);
    self->bobPhaseFast += (self->bobIncrFast > 0) ? (s32)(0.5f + (self->bobIncrFast * 2.0f / 3.0f))
                                                  : (s32)((self->bobIncrFast * 2.0f / 3.0f) - 0.5f);
    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}

void BgSpot08Iceblock_Draw(Actor* actor, GameState* state) {
    BgSpot08Iceblock* self = (BgSpot08Iceblock*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    switch (self->dyna.actor.params & 0x200) {
        case 0:
            self->skelAnimModel1->SetUnkAC(1);
            self->skelAnimModel1->FUN_003721e0(&modelMtx);
            self->skelAnimModel1->FUN_00372170(0);
            break;
        case 0x200:
            self->skelAnimModel2->SetUnkAC(1);
            self->skelAnimModel2->FUN_003721e0(&modelMtx);
            self->skelAnimModel2->FUN_00372170(0);
            break;
    }
}
}
