extern "C" {
#include "z_bg_mori_elevator.hpp"

#define FLAGS 0x80400010

void BgMoriElevator_Init(Actor* actor, GameState* state);
void BgMoriElevator_Destroy(Actor* actor, GameState* state);
void BgMoriElevator_Update(Actor* actor, GameState* state);

void FUN_001107b0(BgMoriElevator* self, PlayState* play); // N64: BgMoriElevator_WaitAfterInit
void FUN_003600d4(BgMoriElevator* self);
f32 FUN_003600e4(f32* pValue, f32 target, f32 scale, f32 maxStep, f32 minStep);
void FUN_0036ef10(PosRot* pos, s32 sfxId, f32 volume); // N64: func_800F436C
s32 FUN_0036e980(PlayState* play, Actor* actor, u8 arg2);
void FUN_003bf708(BgMoriElevator* self, PlayState* play); // N64: BgMoriElevator_SetPosition
void FUN_003f73b4(BgMoriElevator* self, PlayState* play); // N64: BgMoriElevator_StopMovement
void FUN_0012adb4(BgMoriElevator* self, PlayState* play); // N64: BgMoriElevator_MoveIntoGround
void FUN_0012b7e8(BgMoriElevator* self, PlayState* play); // N64: BgMoriElevator_MoveAboveGround
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);
void FUN_0037ab90(Actor* actor, GameState* state); // N64: BgMoriElevator_Draw

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);

s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);

static s16 sIsSpawned = false;

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 3000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

ActorInit Bg_Mori_Elevator_InitVars = {
    ACTOR_BG_MORI_ELEVATOR,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriElevator),
    (ActorFunc)BgMoriElevator_Init,
    (ActorFunc)BgMoriElevator_Destroy,
    (ActorFunc)BgMoriElevator_Update,
    NULL,
};

void BgMoriElevator_Init(Actor* actor, GameState* state) {
    BgMoriElevator* self = (BgMoriElevator*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    self->unk_172 = sIsSpawned;
    self->moriTexObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_TEX);

    switch (sIsSpawned) {
        case false:
            sIsSpawned = true;
            actor->room = -1;
            Actor_ProcessInitChain(actor, sInitChain);
            DynaPolyActor_Init(&self->dyna, 1);
            FUN_00372f38(actor, play, &self->skelAnimModel, 2, 0);
            ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 2), &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
            self->actionFunc = FUN_001107b0;
            break;
        case true:
            Actor_Kill(actor);
            break;
    }
}

void BgMoriElevator_Destroy(Actor* actor, GameState* state) {
    BgMoriElevator* self = (BgMoriElevator*)actor;
    PlayState* play = (PlayState*)state;

    if (self->unk_172 == 0) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
        sIsSpawned = false;
    }
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgMoriElevator_Update(Actor* actor, GameState* state) {
    BgMoriElevator* self = (BgMoriElevator*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
    self->unk_170 = self->dyna.unk_1B8;
    self->unk_16C = Flags_GetSwitch(play, (actor->params & 0x3F));
}

void FUN_001107b0(BgMoriElevator* self, PlayState* play) { // N64: BgMoriElevator_WaitAfterInit
    if (Object_IsLoaded(&play->objectCtx, self->moriTexObjectSlot)) {
        if (Flags_GetSwitch(play, self->dyna.actor.params & 0x3F) == 0) {
            self->actionFunc = FUN_003bf708;
        } else if (((u8*)play)[0x4C30] == 2) { // roomCtx.curRoom.num
            self->dyna.actor.world.pos.y = 73.0f;
            self->actionFunc = FUN_003bf708;
        }
        self->dyna.actor.draw = FUN_0037ab90;
    }
}

void FUN_0037ab90(Actor* actor, GameState* state) { // N64: BgMoriElevator_Draw
    BgMoriElevator* self = (BgMoriElevator*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

// Clamped step toward target (N64: func_808A1800). Returns the applied step.
f32 FUN_003600e4(f32* pValue, f32 target, f32 scale, f32 maxStep, f32 minStep) {
    f32 var = (target - *pValue) * scale;

    if (*pValue < target) {
        if (var > maxStep) {
            var = maxStep;
        } else {
            if (var < minStep) {
                var = minStep;
            }
        }
        *pValue = (*pValue + var);
        if (*pValue > target) {
            *pValue = target;
        }
    } else {
        if (target < *pValue) {
            if (var < (-maxStep)) {
                var = (-maxStep);
            } else {
                if ((-minStep) < var) {
                    var = (-minStep);
                }
            }
            *pValue = (*pValue + var);
            if (*pValue < target) {
                *pValue = target;
            }
        } else {
            var = 0.0f;
        }
    }
    return var;
}

void FUN_003600d4(BgMoriElevator* self) { // N64: BgMoriElevator_SetupSetPosition
    self->actionFunc = FUN_003bf708;
}

void FUN_001853ac(BgMoriElevator* self, PlayState* play) { // N64: none (3DS-only player-riding counter)
    u8* player = (u8*)GET_PLAYER(play);

    *(s16*)(player + 0x22A0) = self->dyna.actor.yawTowardsPlayer;
    *(f32*)(player + 0x229C) = -5.5f;
    self->unk_1D0++;
    if (self->dyna.unk_1B8 & 2) { // DYNA_INTERACT_PLAYER_ON_TOP
        self->unk_1D4++;
    }
    if (self->unk_1D0 >= 3) {
        if (self->unk_1D0 == self->unk_1D4) {
            self->dyna.actor.velocity.y = 0.0f;
            self->actionFunc = FUN_003f73b4;
            FUN_0036e980(play, &self->dyna.actor, 8);
            *(f32*)(player + 0x229C) = 0.0f;
        } else {
            self->unk_1D0 = 0;
            self->unk_1D4 = 0;
            self->actionFunc = FUN_003bf708;
        }
    }
}

// Cannot match anyway: the ROM region (576 B) also holds 28 B of an unrelated function at 0x003BF92C
// (rot.y += 100; sfx 0x010001D9) after the literal pool -> needs a functions.tsv/cfg split.
#define ROOMNUM(play) (((s8*)(play))[0x4C30])
static __inline s32 BgMoriElevator_IsPlayerRiding(BgMoriElevator* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;
    return (self->dyna.unk_1B8 & 2) && !(self->unk_170 & 2) &&
           ((GET_PLAYER(play)->actor.world.pos.y - thisx->world.pos.y) < 80.0f);
}

static __inline void BgMoriElevator_StopMovement(BgMoriElevator* self, PlayState* play) {
    u8* player = (u8*)GET_PLAYER(play);
    *(s16*)(player + 0x22A0) = self->dyna.actor.yawTowardsPlayer;
    *(f32*)(player + 0x229C) = -5.5f;
    self->unk_1D0 = 0;
    self->unk_1D4 = 0;
    self->actionFunc = FUN_001853ac;
}

static __inline void func_808A1C30(BgMoriElevator* self) {
    self->actionFunc = FUN_0012adb4;
}

static __inline void func_808A1CF4(BgMoriElevator* self, PlayState* play) {
    self->actionFunc = FUN_0012b7e8;
    OnePointCutscene_Init(play, 3230, 70, &self->dyna.actor, 0);
    OnePointCutscene_Init(play, 1020, 15, &self->dyna.actor, 0);
}

void FUN_003bf708(BgMoriElevator* self, PlayState* play) {
    if (BgMoriElevator_IsPlayerRiding(self, play)) {
        if (ROOMNUM(play) == 2) {
            self->targetY = -779.0f;
            BgMoriElevator_StopMovement(self, play);
        } else if (ROOMNUM(play) == 17) {
            self->targetY = 233.0f;
            BgMoriElevator_StopMovement(self, play);
        }
    } else if ((ROOMNUM(play) == 2) && (self->dyna.actor.world.pos.y < -275.0f)) {
        self->targetY = 233.0f;
        BgMoriElevator_StopMovement(self, play);
    } else if ((ROOMNUM(play) == 17) && (-275.0f < self->dyna.actor.world.pos.y)) {
        self->targetY = -779.0f;
        BgMoriElevator_StopMovement(self, play);
    } else if ((ROOMNUM(play) == 2) && Flags_GetSwitch(play, self->dyna.actor.params & 0x3F) &&
               (self->unk_16C == 0)) {
        self->targetY = 73.0f;
        func_808A1C30(self);
    } else if ((ROOMNUM(play) == 2) && !Flags_GetSwitch(play, self->dyna.actor.params & 0x3F) &&
               (self->unk_16C != 0)) {
        self->targetY = 233.0f;
        func_808A1CF4(self, play);
    }
}

// FUN_0012adb4 / FUN_0012b7e8 / FUN_003f73b4 (Move*/StopMovement actions) live in z_bg_mori_elevator_move.cpp:
// they must NOT share a TU with FUN_003600e4/FUN_003600d4, otherwise armcc inlines FUN_003600d4 and uses
// intra-TU register knowledge (keeps 0.0/1.0 in s6/s7 across the call instead of s16/s17 + vpush d8).
}
