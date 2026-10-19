extern "C" {
#include "z_bg_spot15_rrbox.hpp"

#define FLAGS 0x00000000

// Data blob at 0x00513AB8 (see data/z_Bg_Spot15_Rrbox.data.s): s16 spawn counter (N64 D_808B4590),
// sBoxGroundCheckPoints (0x513ABC), sInitChain (0x513AF8), D_808B45C4 spawn positions (0x513B08).
// No symbols in the scatter file -> absolute symbols (EQU) in data_syms.s. Init reaches the positions
// as counter + 0x50 (one literal), so the blob is described as one struct.
typedef struct {
    /* 0x00 */ s16 counter;
    /* 0x04 */ Vec3f boxGroundCheckPoints[5];
    /* 0x40 */ InitChainEntry initChain[4];
    /* 0x50 */ Vec3f positions[1];
} BgSpot15RrboxData;

extern BgSpot15RrboxData BgSpot15Rrbox_sData;
extern InitChainEntry BgSpot15Rrbox_sInitChain[];
#define D_808B4590 (BgSpot15Rrbox_sData.counter)
#define D_808B45C4 (BgSpot15Rrbox_sData.positions)
#define sInitChain BgSpot15Rrbox_sInitChain

// Helpers not declared in include/ yet
void FUN_00184ff8(BgSpot15Rrbox* self, PlayState* play);
void FUN_003b3080(BgSpot15Rrbox* self, PlayState* play);
void FUN_003bc294(BgSpot15Rrbox* self, PlayState* play);
void FUN_003bd528(BgSpot15Rrbox* self, PlayState* play);
void FUN_00315fd8(BgSpot15Rrbox* self, PlayState* play);
extern __pure float _fabsf(float);

void BgSpot15Rrbox_Init(Actor* actor, GameState* state);
void BgSpot15Rrbox_Destroy(Actor* actor, GameState* state);
void BgSpot15Rrbox_Update(Actor* actor, GameState* state);
void BgSpot15Rrbox_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Spot15_Rrbox.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Spot15_Rrbox/data_syms.s")

ActorInit Bg_Spot15_Rrbox_InitVars = {
    ACTOR_BG_SPOT15_RRBOX,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT15_OBJ,
    sizeof(BgSpot15Rrbox),
    (ActorFunc)BgSpot15Rrbox_Init,
    (ActorFunc)BgSpot15Rrbox_Destroy,
    (ActorFunc)BgSpot15Rrbox_Update,
    (ActorFunc)BgSpot15Rrbox_Draw,
};

// Draft, not matching yet: 7 words differ, register allocation only (ROM keeps idx in r2, idx + 1 in r0 and
// the positions base (counter + 0x50) in ip). Variants tried: struct copy, idx/next/pointer locals in every order,
// actionFunc store before/after, SetupWait inline helper, self->dyna.actor vs actor. Kept as asm.
#if 0
void BgSpot15Rrbox_Init(Actor* actor, GameState* state) {
    BgSpot15Rrbox* self = (BgSpot15Rrbox*)actor;
    PlayState* play = (PlayState*)state;
    void* colInfo;

    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 0);
    {
        CollisionHeader* colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(colInfo, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Actor_ProcessInitChain(actor, sInitChain);
    self->bgId = BG_ACTOR_MAX;
    if (Flags_GetSwitch(play, actor->params & 0x3F)) {
        self->actionFunc = FUN_003b3080;
        {
            BgSpot15RrboxData* d = &BgSpot15Rrbox_sData;
            Vec3f* positions = d->positions;
            s16 idx = d->counter;

            actor->world.pos.x = positions[idx].x;
            actor->world.pos.y = positions[idx].y;
            actor->world.pos.z = positions[idx].z;
            d->counter = idx + 1;
        }
    } else {
        actor->gravity = 0.0f;
        actor->velocity.x = actor->velocity.y = actor->velocity.z = 0.0f;
        self->actionFunc = FUN_00184ff8;
    }
}
#endif
GLOBAL_ASM("asm/BgSpot15Rrbox_Init.s")

void BgSpot15Rrbox_Destroy(Actor* actor, GameState* state) {
    BgSpot15Rrbox* self = (BgSpot15Rrbox*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    D_808B4590 = 0;
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot15Rrbox_Update(Actor* actor, GameState* state) {
    BgSpot15Rrbox* self = (BgSpot15Rrbox*)actor;
    PlayState* play = (PlayState*)state;

    if (self->unk_168 > 0) {
        self->unk_168--;
    }
    self->dyna.actor.world.rot.y = self->dyna.unk_1B0;
    self->unk_16C = Math_SinS(self->dyna.actor.world.rot.y);
    self->unk_170 = Math_CosS(self->dyna.actor.world.rot.y);
    self->actionFunc(self, play);
}

void BgSpot15Rrbox_Draw(Actor* actor, GameState* state) {
    BgSpot15Rrbox* self = (BgSpot15Rrbox*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

// Action (matched). Moved from functions_3A0000s.cpp (ld name kept).
void FUN_003b3080(BgSpot15Rrbox* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    player->stateFlags2 &= ~0x10; // PLAYER_STATE2_4 (not defined in repo headers)
    self->dyna.unk_1A8 = 0.0f;
}

static __inline __pure float Rrbox_Fabsf(float f) {
    return _fabsf(f);
}

void FUN_003bc294(BgSpot15Rrbox* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* actor = &self->dyna.actor;

    if (Rrbox_Fabsf(self->dyna.unk_1A8) > 0.001f) {
        self->dyna.unk_1A8 = 0.0f;
        player->stateFlags2 &= ~0x10; // PLAYER_STATE2_4
    }
    Actor_MoveForward(&self->dyna.actor);
    if (actor->world.pos.y <= -31990.0f) {
        Actor_Kill(actor);
        return;
    }
    if ((actor->floorHeight - actor->world.pos.y) >= -0.001f) {
        actor->world.pos.y = actor->floorHeight;
        FUN_00315fd8(self, play);
        Audio_PlayActorSound2(&self->dyna.actor, 0x01000220);
    }
}

// Draft, not matching yet: 4 words differ (float load scheduling in the IsOnFloorBox distance check: ROM loads
// dyna.z into s3 before this.z). Still GLOBAL_ASM in functions_180000s.cpp (N64 func_808B40AC + inlined 3AAC/3A40/4010).
#if 0
DynaPolyActor* DynaPoly_GetActor(CollisionContext* colCtx, s32 bgId);
s32 FUN_0036d288(PlayState* play, DynaPolyActor* dyna, s16 arg2, s16 arg3, s32 arg4); // N64 func_800435D8

static inline f32 Rrbox_DistXZSq(Vec3f* a, Vec3f* b) {
    f32 dx = a->x - b->x;
    f32 dz = a->z - b->z;

    return dx * dx + dz * dz;
}

static s32 BgSpot15Rrbox_IsOnFloorBox(BgSpot15Rrbox* self, PlayState* play) {
    DynaPolyActor* dynaPolyActor = DynaPoly_GetActor(&play->colCtx, self->bgId);

    if ((dynaPolyActor != NULL) &&
        Rrbox_DistXZSq(&dynaPolyActor->actor.world.pos, &self->dyna.actor.world.pos) < 0.01f) {
        return true;
    }
    return false;
}

static s32 BgSpot15Rrbox_CanMove(BgSpot15Rrbox* self, PlayState* play) {
    s16 rotY;

    if (play->sceneNum == 0x4C) { // SCENE_LON_LON_BUILDINGS
        return true;
    } else if (BgSpot15Rrbox_IsOnFloorBox(self, play)) {
        return false;
    }
    if (self->dyna.actor.world.pos.x <= 930.0f && self->dyna.actor.world.pos.z >= -360.0f) {
        if (self->dyna.unk_1A8 >= 0.0f) {
            rotY = self->dyna.actor.world.rot.y;
        } else {
            rotY = self->dyna.actor.world.rot.y + 0x8000;
        }
        if (rotY < 0x2000 && rotY > -0x6000) {
            return gSaveContext.eventChkInf[1] & 0x10; // EVENTCHKINF_TALON_RETURNED_FROM_CASTLE
        }
        return true;
    }
    return true;
}

static s32 BgSpot15Rrbox_IsBlocked(BgSpot15Rrbox* self, PlayState* play) {
    return !FUN_0036d288(play, &self->dyna, self->dyna.actor.scale.x * 290.0f,
                         self->dyna.actor.scale.x * 290.0f + 20.0f, 1);
}

void FUN_00184ff8(BgSpot15Rrbox* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->unk_168 <= 0 && Rrbox_Fabsf(self->dyna.unk_1A8) > 0.001f) {
        if (BgSpot15Rrbox_CanMove(self, play) && !BgSpot15Rrbox_IsBlocked(self, play)) {
            self->unk_17C = self->dyna.unk_1A8;
            self->dyna.actor.gravity = 0.0f;
            self->actionFunc = FUN_003bd528;
            return;
        }
    }
    player->stateFlags2 &= ~0x10; // PLAYER_STATE2_4
    self->dyna.unk_1A8 = 0.0f;
}
#endif

// Draft only (rough): 3DS version differs from N64 (inlined 4380, timer 15 vs 10). Still GLOBAL_ASM in functions_3A0000s.cpp.
#if 0
void FUN_003bd528(BgSpot15Rrbox* self, PlayState* play) {
    f32 sign;
    Player* player = GET_PLAYER(play);
    Actor* actor = &self->dyna.actor;
    s32 approxFResult;

    self->unk_174 += 0.5f;
    if (self->unk_174 > 2.0f) {
        self->unk_174 = 2.0f;
    }
    approxFResult = Math_StepToF(&self->unk_178, 20.0f, self->unk_174);
    sign = self->unk_17C >= 0.0f ? 1.0f : -1.0f;
    actor->world.pos.x = actor->home.pos.x + (sign * self->unk_178 * self->unk_16C);
    actor->world.pos.z = actor->home.pos.z + (sign * self->unk_178 * self->unk_170);

    if (!BgSpot15Rrbox_TrySnapToFloor(self, play)) {
        actor->home.pos.x = actor->world.pos.x;
        actor->home.pos.z = actor->world.pos.z;
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_1A8 = 0.0f;
        self->unk_178 = 0.0f;
        self->unk_174 = 0.0f;
        actor->velocity.x = actor->velocity.y = actor->velocity.z = 0.0f;
        actor->gravity = -1.0f;
        actor->floorHeight = BgSpot15Rrbox_GetFloorHeight(self, play);
        self->actionFunc = FUN_003bc294;
    } else if (approxFResult) {
        if (BgSpot15Rrbox_IsBlocked(self, play)) {
            Audio_PlayActorSound2(actor, 0x01000220);
        }
        if (BgSpot15Rrbox_IsOnFloorBox(self, play)) {
            Sfx_PlaySfxCentered(0x01000487);
        }
        actor->home.pos.x = actor->world.pos.x;
        actor->home.pos.z = actor->world.pos.z;
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_1A8 = 0.0f;
        self->unk_178 = 0.0f;
        self->unk_174 = 0.0f;
        self->unk_168 = 10;
        actor->gravity = 0.0f;
        actor->velocity.x = actor->velocity.y = actor->velocity.z = 0.0f;
        self->actionFunc = FUN_00184ff8;
    }
    Audio_PlayActorSound2(actor, 0x01000161);
}
#endif

}
