extern "C" {
#include "z_bg_jya_lift.hpp"

extern __pure float _fabsf(float);
static __inline __pure float BgJyaLift_Fabsf(float f) {
    return _fabsf(f);
}

#define FLAGS 0x80400010

#define DYNA_INTERACT_PLAYER_ABOVE 4
#define CAM_SET_DIRECTED_YAW 0x3F
#define CAM_SET_DUNGEON0 3

// Callees not declared in include/ yet
s32 OnePointCutscene_Init(PlayState* play, s32 cutsceneId, s32 arg2, Actor* actor, s32 camId);
void Camera_ChangeSetting(Camera* camera, s32 setting);
void FUN_0036ae48(Camera* camera);
void FUN_00331284(Player* player, void* unk);

void BgJyaLift_Init(Actor* actor, GameState* state);
void BgJyaLift_Destroy(Actor* actor, GameState* state);
void BgJyaLift_Update(Actor* actor, GameState* state);
void BgJyaLift_Draw(Actor* actor, GameState* state);
void BgJyaLift_DelayMove(BgJyaLift* self, PlayState* play);
void BgJyaLift_Move(BgJyaLift* self, PlayState* play);

// Data at 0x00512A90 (see data_syms.s); the Actor init chain is in the data dump too
GLOBAL_ASM("data/z_Bg_Jya_Lift.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Jya_Lift/data_syms.s")
extern s16 BgJyaLift_sIsSpawned;
extern InitChainEntry BgJyaLift_sInitChain[];

ActorInit Bg_Jya_Lift_InitVars = {
    ACTOR_BG_JYA_LIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaLift),
    (ActorFunc)BgJyaLift_Init,
    (ActorFunc)BgJyaLift_Destroy,
    (ActorFunc)BgJyaLift_Update,
    (ActorFunc)BgJyaLift_Draw,
};

void BgJyaLift_Init(Actor* actor, GameState* state) {
    BgJyaLift* self = (BgJyaLift*)actor;
    PlayState* play = (PlayState*)state;
    void* colInfo;

    self->isSpawned = false;
    if (BgJyaLift_sIsSpawned) {
        Actor_Kill(actor);
        return;
    }

    FUN_00372f38(actor, play, &self->skelAnimModel, 0x23, 0);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 6);
    {
        CollisionHeader* colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(colInfo, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Actor_ProcessInitChain(actor, BgJyaLift_sInitChain);
    if (Flags_GetSwitch(play, (u16)actor->params & 0x3F)) {
        self->actionFunc = NULL;
        actor->world.pos.y = 973.0f;
    } else {
        self->actionFunc = BgJyaLift_DelayMove;
        actor->world.pos.y = 1613.0f;
        self->moveDelay = 0;
    }
    actor->room = -1;
    BgJyaLift_sIsSpawned = true;
    self->isSpawned = true;
}

void BgJyaLift_Destroy(Actor* actor, GameState* state) {
    BgJyaLift* self = (BgJyaLift*)actor;
    PlayState* play = (PlayState*)state;

    if (self->isSpawned) {
        BgJyaLift_sIsSpawned = false;
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    }
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgJyaLift_Update(Actor* actor, GameState* state) {
    BgJyaLift* self = (BgJyaLift*)actor;
    PlayState* play = (PlayState*)state;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, play);
    }
    if ((self->dyna.unk_1B8 & DYNA_INTERACT_PLAYER_ABOVE) && !(self->prevInteractFlags & DYNA_INTERACT_PLAYER_ABOVE)) {
        Camera_ChangeSetting(play->cameraPtrs[0], CAM_SET_DIRECTED_YAW);
    } else if (!(self->dyna.unk_1B8 & DYNA_INTERACT_PLAYER_ABOVE) &&
               (self->prevInteractFlags & DYNA_INTERACT_PLAYER_ABOVE) &&
               (*(u16*)((u8*)play->cameraPtrs[0] + 0x18A) == CAM_SET_DIRECTED_YAW)) {
        Camera_ChangeSetting(play->cameraPtrs[0], CAM_SET_DUNGEON0);
    }
    self->prevInteractFlags = self->dyna.unk_1B8;

    // Spirit Temple room 5 is the main room, the statue room 25 is directly above room 5
    if ((*(s8*)((u8*)play + 0x4C30) != 5) && (*(s8*)((u8*)play + 0x4C30) != 25)) {
        Actor_Kill(actor);
    }
}

void BgJyaLift_Draw(Actor* actor, GameState* state) {
    BgJyaLift* self = (BgJyaLift*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        FUN_00331284(GET_PLAYER(play), actor->unk_178);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

void BgJyaLift_DelayMove(BgJyaLift* self, PlayState* play) {
    if (Flags_GetSwitch(play, (u16)self->dyna.actor.params & 0x3F) || (self->moveDelay > 0)) {
        self->moveDelay++;
        if (self->moveDelay >= 20) {
            OnePointCutscene_Init(play, 3430, -99, &self->dyna.actor, 0);
            self->actionFunc = BgJyaLift_Move;
        }
    }
}

void BgJyaLift_Move(BgJyaLift* self, PlayState* play) {
    f32 distFromBottom;
    f32 tempVelocity;

    Math_SmoothStepToF(&self->dyna.actor.velocity.y, 4.0f, 0.1f, 1.0f, 0.0f);
    tempVelocity = (self->dyna.actor.velocity.y < 0.2f) ? 0.2f : self->dyna.actor.velocity.y;
    distFromBottom = Math_SmoothStepToF(&self->dyna.actor.world.pos.y, 973.0f, 0.1f, tempVelocity, 0.2f);
    if ((self->dyna.actor.world.pos.y < 1440.0f) && (1440.0f <= self->dyna.actor.prevPos.y)) {
        FUN_0036ae48(play->cameraPtrs[*(s16*)((u8*)play + 0xA64)]);
    }
    if (BgJyaLift_Fabsf(distFromBottom) < 0.001f) {
        self->actionFunc = NULL;
        self->dyna.actor.world.pos.y = 973.0f;
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001D1);
    } else {
        FUN_00373264(&self->dyna.actor, 0x01000163);
    }
}

}
