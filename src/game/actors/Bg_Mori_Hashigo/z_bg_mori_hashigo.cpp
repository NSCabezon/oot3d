extern "C" {
#include "z_bg_mori_hashigo.hpp"

#define FLAGS 0x80400000

#define HASHIGO_CLASP -1
#define HASHIGO_LADDER 0

// Helpers not declared in include/ yet
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
void Collider_InitJntSph(PlayState* play, BgMoriHashigoJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgMoriHashigoJntSph* collider, Actor* actor, void* src, void* elements);
void Collider_DestroyJntSph(PlayState* play, BgMoriHashigoJntSph* collider);
void FUN_002ac4f8(BgMoriHashigo* self, PlayState* play); // N64: BgMoriHashigo_Draw (asm, src/functions/functions_2A0000s.cpp)

void FUN_0011070c(BgMoriHashigo* self, PlayState* play); // N64: BgMoriHashigo_WaitForMoriTex

void BgMoriHashigo_Init(Actor* actor, GameState* state);
void BgMoriHashigo_Destroy(Actor* actor, GameState* state);
void BgMoriHashigo_Update(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Mori_Hashigo.data.s")

// Fixed-address statics in the data dump (no symbol in asm)
#define sInitChainClasp ((InitChainEntry*)0x005132C0)
#define sJntSphInit ((void*)0x005132B0)
#define sInitChainLadder ((InitChainEntry*)0x005132D8)

ActorInit Bg_Mori_Hashigo_InitVars = {
    ACTOR_BG_MORI_HASHIGO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriHashigo),
    (ActorFunc)BgMoriHashigo_Init,
    (ActorFunc)BgMoriHashigo_Destroy,
    (ActorFunc)BgMoriHashigo_Update,
    NULL,
};

#if 0 // draft (Opus pass): 572 B, sp frame 0x14 as ROM, 55/143 words differ (push r4-r7 vs ROM r4-r9). ROM keeps a
       // second copy of play (r7) in the ladder DynaPoly helper and the NULL constant in r8 across both
       // branches (push {r4-r9}); here the zsi pointer reuses r7. Helper signature mix below is what got closest:
       // Actor*/GameState* params with local casts + passing &play->state make armcc keep thisx (helpers, Kill)
       // and self (slot/actionFunc) in separate regs as in ROM. Still GLOBAL_ASM.
static void BgMoriHashigo_InitDynapoly(Actor* thisx, PlayState* play, s32 idx, s32 moveFlag) {
    BgMoriHashigo* self = (BgMoriHashigo*)thisx;
    CollisionHeader* colHeader = NULL;
    void* zsi = DynaPolyInfo_Alloc(&self->dyna, &play->state, idx);
    DynaPolyActor_Init(&self->dyna, moveFlag);
    ReadCollisionHeaderFromZsi(zsi, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
}

static void BgMoriHashigo_InitCollider(BgMoriHashigo* self, PlayState* play) {
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->dyna.actor, sJntSphInit, self->colliderElements);
    ((BgMoriHashigoJntSphElement*)self->collider.elements)->centerX = self->dyna.actor.world.pos.x;
    ((BgMoriHashigoJntSphElement*)self->collider.elements)->centerY = self->dyna.actor.world.pos.y + 21.0f;
    ((BgMoriHashigoJntSphElement*)self->collider.elements)->centerZ = self->dyna.actor.world.pos.z;
    ((BgMoriHashigoJntSphElement*)self->collider.elements)->radius = 19.0f;
}

static s32 BgMoriHashigo_SpawnLadder(BgMoriHashigo* self, GameState* state) {
    PlayState* play = (PlayState*)state;
    f32 sn;
    f32 cs;
    Vec3f pos;
    Actor* ladder;

    cs = Math_CosS(self->dyna.actor.shape.rot.y);
    sn = Math_SinS(self->dyna.actor.shape.rot.y);
    pos.x = 6.0f * sn + self->dyna.actor.world.pos.x;
    pos.y = -210.0f + self->dyna.actor.world.pos.y;
    pos.z = 6.0f * cs + self->dyna.actor.world.pos.z;
    ladder = Actor_SpawnAsChild(&play->actorCtx, &self->dyna.actor, play, ACTOR_BG_MORI_HASHIGO, pos.x, pos.y, pos.z,
                                self->dyna.actor.world.rot.x, self->dyna.actor.world.rot.y,
                                self->dyna.actor.world.rot.z, 0);
    if (ladder != NULL) {
        return true;
    } else {
        return false;
    }
}

static s32 BgMoriHashigo_InitClasp(Actor* thisx, GameState* state) {
    BgMoriHashigo* self = (BgMoriHashigo*)thisx;
    PlayState* play = (PlayState*)state;
    Actor_ProcessInitChain(&self->dyna.actor, sInitChainClasp);
    self->dyna.actor.flags |= 1;
    Actor_SetFocus(&self->dyna.actor, 55.0f);
    FUN_00372f38(&self->dyna.actor, play, &self->claspSkelAnimModel, 4, 0);
    BgMoriHashigo_InitCollider(self, play);
    if ((self->dyna.actor.params == HASHIGO_CLASP) && !BgMoriHashigo_SpawnLadder(self, &play->state)) {
        return false;
    } else {
        return true;
    }
}

static s32 BgMoriHashigo_InitLadder(Actor* thisx, GameState* state) {
    BgMoriHashigo* self = (BgMoriHashigo*)thisx;
    PlayState* play = (PlayState*)state;
    FUN_00372f38(&self->dyna.actor, play, &self->ladderSkelAnimModel, 3, 0);
    BgMoriHashigo_InitDynapoly(&self->dyna.actor, play, 3, 0);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChainLadder);
    return true;
}

void BgMoriHashigo_Init(Actor* thisx, GameState* state) {
    BgMoriHashigo* self = (BgMoriHashigo*)thisx;
    PlayState* play = (PlayState*)state;

    self->claspSkelAnimModel = NULL;
    self->ladderSkelAnimModel = NULL;
    if (self->dyna.actor.params == HASHIGO_CLASP) {
        if (!BgMoriHashigo_InitClasp(thisx, &play->state)) {
            Actor_Kill(thisx);
            return;
        }
    } else if (self->dyna.actor.params == HASHIGO_LADDER) {
        if (!BgMoriHashigo_InitLadder(thisx, &play->state)) {
            Actor_Kill(thisx);
            return;
        }
    }
    self->moriTexObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjectSlot < 0) {
        Actor_Kill(thisx);
    } else {
        self->actionFunc = (BgMoriHashigoActionFunc)FUN_0011070c;
    }
}
#endif

GLOBAL_ASM("asm/BgMoriHashigo_Init.s")

void BgMoriHashigo_Destroy(Actor* actor, GameState* state) {
    BgMoriHashigo* self = (BgMoriHashigo*)actor;
    PlayState* play = (PlayState*)state;

    if (self->dyna.actor.params == HASHIGO_LADDER) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }
    if (self->dyna.actor.params == HASHIGO_CLASP) {
        Collider_DestroyJntSph(play, &self->collider);
    }
    FUN_00350f34(
        actor, &self->claspSkelAnimModel, &self->ladderSkelAnimModel, 0);
}

void FUN_0011070c(BgMoriHashigo* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->moriTexObjectSlot)) {
        if (self->dyna.actor.params == HASHIGO_CLASP) {
            self->actionFunc = (BgMoriHashigoActionFunc)0x00376114; // N64: BgMoriHashigo_Clasp (inside FUN_00375fd0, cfg-merged)
        } else if (self->dyna.actor.params == HASHIGO_LADDER) {
            self->actionFunc = (BgMoriHashigoActionFunc)0x001A250C; // N64: BgMoriHashigo_LadderWait (inside FUN_001a24d4, cfg-merged)
        }
        self->dyna.actor.draw = (ActorFunc)FUN_002ac4f8;
    }
}

void BgMoriHashigo_Update(Actor* actor, GameState* state) {
    BgMoriHashigo* self = (BgMoriHashigo*)actor;
    PlayState* play = (PlayState*)state;

    if (self->hitTimer > 0) {
        self->hitTimer--;
    }
    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
}

}
