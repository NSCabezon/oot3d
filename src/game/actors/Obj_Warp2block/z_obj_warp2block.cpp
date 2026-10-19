extern "C" {
#include "z_obj_warp2block.hpp"
#include "ichain.hpp"

#define FLAGS 0x0A000031

#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

typedef struct Warp2BlockSpawnData {
    /* 0x00 */ f32 scale;
    /* 0x04 */ f32 focus;
    /* 0x08 */ s16 params;
} Warp2BlockSpawnData; // size = 0x0C

void ObjWarp2block_Init(Actor* thisx, GameState* state);
void ObjWarp2block_Destroy(Actor* actor, GameState* state);
void ObjWarp2block_Update(Actor* thisx, GameState* state);
void ObjWarp2block_Draw(Actor* thisx, GameState* state);
void ObjWarp2block_DoNothing(ObjWarp2block* self, PlayState* play);

// Action functions and helpers in src/functions/*.cpp (GLOBAL_ASM), only used by this actor
s32 FUN_0025ac68(ObjWarp2block* self, PlayState* play); // N64: func_80BA2218
void FUN_00184184(ObjWarp2block* self, PlayState* play); // N64: func_80BA24F8
void FUN_00185468(ObjWarp2block* self, PlayState* play); // N64: func_80BA2610
s32 FUN_0032d8d8(DynaPolyActor* dyna); // DynaPolyActor_IsPlayerAbove
void FUN_0037073c(PlayState* play, s32 action); // Message_StartOcarina
// N64 func_80BA228C: unlabeled, sits inside the ObjTimeblock_WaitForSong cfg entry (needs a cfg split) -> raw address (debt)
#define OBJWARP2BLOCK_FUNC_80BA228C ((ObjWarp2blockFunc168)0x0020803C)

ActorInit Obj_Warp2block_InitVars = {
    ACTOR_OBJ_WARP2BLOCK,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_TIMEBLOCK,
    sizeof(ObjWarp2block),
    (ActorFunc)ObjWarp2block_Init,
    (ActorFunc)ObjWarp2block_Destroy,
    (ActorFunc)ObjWarp2block_Update,
    (ActorFunc)ObjWarp2block_Draw,
};

// Data block 0x535A08..0x535A58 (was GLOBAL_ASM data dump). sUnk535A08 is read by FUN_00185468 only.
__attribute__((used)) static f32 sUnk535A08[2] = { 65.0f, 6.0f };

__attribute__((used)) static Warp2BlockSpawnData sSpawnData[] = {
    { 1.0f, 60.0f, 0x0018 },
    { 0.6f, 40.0f, 0x0019 },
};

__attribute__((used)) static f32 sDistances[] = { 60.0f, 100.0f, 140.0f, 180.0f, 220.0f, 260.0f, 300.0f, 300.0f };

__attribute__((used)) static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1500, ICHAIN_STOP),
    { 0 },
};

static inline void func_80BA24E8(ObjWarp2block* self) { self->actionFunc = FUN_00184184; }
static inline void ObjWarp2block_SetInactive(ObjWarp2block* self) {
    self->actionFunc = ObjWarp2block_DoNothing;
    self->dyna.actor.draw = NULL;
}

// Matches only with params read via `thisx->` (self->dyna.actor.params swaps r0/r1 on the two literal loads).
void ObjWarp2block_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    ObjWarp2block* self = (ObjWarp2block*)thisx;
    CollisionHeader* colHeader;
    ZARInfo* zar;

    colHeader = NULL;
    self->dyna.actor.world.rot.z = self->dyna.actor.shape.rot.z = 0;
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);

    Actor_SetScale(&self->dyna.actor, sSpawnData[PARAMS_GET_U(thisx->params, 8, 1)].scale);
    self->func_168 = FUN_0025ac68;
    Actor_SetFocus(&self->dyna.actor, sSpawnData[PARAMS_GET_U(thisx->params, 8, 1)].focus);

    zar = FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel, 0, 0);
    if (!PARAMS_GET_U(thisx->params, 15, 1)) {
        ObjWarp2block_SetInactive(self);
    } else {
        func_80BA24E8(self);
        if (Flags_GetSwitch(play, PARAMS_GET_U(thisx->params, 0, 6))) {
            self->dyna.actor.draw = NULL;
        }
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(zar->FUN_003532c0(0), &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
    }
}

void ObjWarp2block_Destroy(Actor* thisx, GameState* state) {
    ObjWarp2block* self = (ObjWarp2block*)thisx;
    PlayState* play = (PlayState*)state;

    if (PARAMS_GET_U(self->dyna.actor.params, 15, 1)) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }
    if (self->skelAnimModel != NULL) {
        FUN_003508b8(&self->dyna.actor, self->skelAnimModel, 0);
    }
}

void ObjWarp2block_Update(Actor* thisx, GameState* state) {
    ObjWarp2block* self = (ObjWarp2block*)thisx;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
    if (self->unk_16C > 0) {
        self->unk_16C--;
    }
}

void ObjWarp2block_Draw(Actor* thisx, GameState* state) {
    ObjWarp2block* self = (ObjWarp2block*)thisx;

    // Inline state cast keeps the 0x20ac literal (a `play` local folds it into add #0x2000).
    FUN_00357fd0(GET_PLAYER((PlayState*)state), thisx->unk_178, &thisx->world.pos);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&thisx->modelMtx);
    self->skelAnimModel->FUN_00372170(0);
    self->unk_1CE = 1;
}

#if 0 // NON_MATCHING draft (Opus): 4 words differ, play/player land in r7/r8 swapped (ROM: play=r8, player=r7).
// Best form: (Actor*, GameState*) signature, `self` declared before `play`. Tried ~20 variants (decl order, raw state,
// helper signature/inline, early return, flags pointer, store order). Needs the prototype + an (ObjWarp2blockFunc168) cast in Init.
// N64: func_80BA1ECC (inlined into FUN_0025ac68)
static inline s32 ObjWarp2block_PlayerInRange(ObjWarp2block* self, PlayState* play) {
    if (FUN_0032d8d8(&self->dyna)) {
        return 0;
    } else {
        Actor* child = self->dyna.actor.child;
        Player* player = GET_PLAYER(play);
        Vec3f sp20;
        f32 temp_f2;

        if ((self->dyna.actor.xzDistToPlayer <= sDistances[PARAMS_GET_U(self->dyna.actor.params, 11, 3)]) ||
            (child->xzDistToPlayer <= sDistances[PARAMS_GET_U(child->params, 11, 3)])) {
            FUN_0036c5d8(&self->dyna.actor, &sp20, &player->actor.world.pos);
            temp_f2 = (self->dyna.actor.scale.x * 50.0f) + 6.0f;

            if (!(temp_f2 < __fabsf(sp20.x)) && !(temp_f2 < __fabsf(sp20.z))) {
                return 0;
            }

            FUN_0036c5d8(child, &sp20, &player->actor.world.pos);
            temp_f2 = (child->scale.x * 50.0f) + 6.0f;

            if (!(temp_f2 < __fabsf(sp20.x)) && !(temp_f2 < __fabsf(sp20.z))) {
                return 0;
            }
        } else {
            return 0;
        }
        return 1;
    }
}

// N64: func_80BA2218
s32 FUN_0025ac68(Actor* thisx, GameState* state) {
    ObjWarp2block* self = (ObjWarp2block*)thisx;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);

    if (ObjWarp2block_PlayerInRange(self, play)) {
        if (player->stateFlags2 & 0x1000000) {
            FUN_0037073c(play, 1);
            self->func_168 = OBJWARP2BLOCK_FUNC_80BA228C;
        } else {
            player->stateFlags2 |= 0x800000;
        }
    }
    return 0;
}
#endif

void ObjWarp2block_DoNothing(ObjWarp2block* self, PlayState* play) {
}

}
