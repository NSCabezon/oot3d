extern "C" {
#include "z_bg_haka.hpp"

#define FLAGS 0x00000000

#define SCENE_GRAVEYARD 0x53
#define SCENE_LAKE_HYLIA 0x57
#define PLAYER_STATE2_4 0x10
#define PLAYER_STATE2_FORCE_SAND_FLOOR_SOUND 0x200

void BgHaka_Init(Actor* actor, GameState* state);
void BgHaka_Destroy(BgHaka* self, PlayState* play);
void BgHaka_Update(BgHaka* self, PlayState* play);
void BgHaka_Draw(BgHaka* self, PlayState* play);

s32 Gameplay_InCsMode(PlayState* play);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);

void FUN_003e4064(BgHaka* self, PlayState* play); // N64: BgHaka_IdleClosed
void FUN_003de340(BgHaka* self, PlayState* play); // N64: BgHaka_Pull
void FUN_001e923c(BgHaka* self, PlayState* play); // N64: BgHaka_IdleLockedClosed
void FUN_003e1370(BgHaka* self, PlayState* play); // N64: BgHaka_IdleOpened

// 7-arg form of the model loader (loads a second model into skelAnimModel2)

ActorInit Bg_Haka_InitVars = {
    ACTOR_BG_HAKA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKA,
    sizeof(BgHaka),
    (ActorFunc)BgHaka_Init,
    (ActorFunc)BgHaka_Destroy,
    (ActorFunc)BgHaka_Update,
    (ActorFunc)BgHaka_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(minVelocityY, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHaka_Init(Actor* actor, GameState* state) {
    BgHaka* self = (BgHaka*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    self->unk_1C8 = 0;
    if ((u16)self->dyna.actor.params & 0x8000) {
        self->unk_1C8 = 1;
        self->dyna.actor.params = (u16)self->dyna.actor.params & 0x7FFF;
    }
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, &self->skelAnimModel2, 1, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->actionFunc = FUN_003e4064;
}

void BgHaka_Destroy(BgHaka* self, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(&self->dyna.actor, &self->skelAnimModel, &self->skelAnimModel2, 0);
}

void BgHaka_Update(BgHaka* self, PlayState* play) {
    self->actionFunc(self, play);
}

// The translate floats go through s0/s1/s2 (x/y/z), so the vector is built from float args.
static inline void BgHaka_Translate(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f translate;

    translate.x = x;
    translate.y = y;
    translate.z = z;
    Matrix_UpdatePosition(mtx, mtx, &translate);
}

void BgHaka_Draw(BgHaka* self, PlayState* play) {
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&mtx);
    self->skelAnimModel->FUN_00372170(0);
    if (play->sceneNum != SCENE_GRAVEYARD && self->unk_1C8 == 0) {
        BgHaka_Translate(&mtx, 0.0f, 0.0f, self->dyna.actor.minVelocityY * 10.0f);
        self->skelAnimModel2->SetUnkAC(1);
        self->skelAnimModel2->FUN_003721e0(&mtx);
        self->skelAnimModel2->FUN_00372170(0);
    }
}

// N64: BgHaka_CheckPlayerOnDirtPatch
static void BgHaka_CheckPlayerOnDirtPatch(BgHaka* self, Player* player) {
    Vec3f playerRelativePos;

    FUN_0036c5d8(&self->dyna.actor, &playerRelativePos, &player->actor.world.pos);
    if (__fabsf(playerRelativePos.x) < 34.6f && playerRelativePos.z > -112.8f && playerRelativePos.z < -36.0f) {
        player->stateFlags2 |= PLAYER_STATE2_FORCE_SAND_FLOOR_SOUND;
    }
}

void FUN_003e4064(BgHaka* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->dyna.unk_1A8 != 0.0f) {
        if (play->sceneNum == SCENE_GRAVEYARD && !LINK_IS_ADULT && IS_DAY) {
            self->dyna.unk_1A8 = 0.0f;
            player->stateFlags2 &= ~PLAYER_STATE2_4;
            if (!Gameplay_InCsMode(play)) {
                FUN_00367c7c(play, 0x5073, NULL);
                self->dyna.actor.params = 150;
                self->actionFunc = FUN_001e923c;
            }
        } else if (0.0f < self->dyna.unk_1A8 ||
                   (play->sceneNum == SCENE_LAKE_HYLIA && !LINK_IS_ADULT && !Flags_GetSwitch(play, 0x23))) {
            self->dyna.unk_1A8 = 0.0f;
            player->stateFlags2 &= ~PLAYER_STATE2_4;
        } else {
            self->dyna.actor.world.rot.y = self->dyna.actor.shape.rot.y + 0x8000;
            self->actionFunc = FUN_003de340;
        }
    }
    BgHaka_CheckPlayerOnDirtPatch(self, player);
}

void FUN_003de340(BgHaka* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 reachedMaxPullDist;

    self->dyna.actor.speedXZ += 0.05f;
    self->dyna.actor.speedXZ = CLAMP_MAX(self->dyna.actor.speedXZ, 1.5f);
    // minVelocityY is used to keep track of the distance pulled from home
    reachedMaxPullDist = Math_StepToF(&self->dyna.actor.minVelocityY, 60.0f, self->dyna.actor.speedXZ);
    self->dyna.actor.world.pos.x =
        self->dyna.actor.minVelocityY * Math_SinS(self->dyna.actor.world.rot.y) + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z =
        self->dyna.actor.minVelocityY * Math_CosS(self->dyna.actor.world.rot.y) + self->dyna.actor.home.pos.z;
    if (reachedMaxPullDist) {
        self->dyna.unk_1A8 = 0.0f;
        player->stateFlags2 &= ~PLAYER_STATE2_4;
        if (self->dyna.actor.params == 1) {
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x1000487); // NA_SE_SY_CORRECT_CHIME
        } else if (!IS_DAY && play->sceneNum == SCENE_GRAVEYARD) {
            Actor_Spawn(&play->actorCtx, play, 0x0D, self->dyna.actor.home.pos.x, self->dyna.actor.home.pos.y,
                        self->dyna.actor.home.pos.z, 0, self->dyna.actor.shape.rot.y, 0, 1, 1);
        } else if (play->sceneNum == SCENE_LAKE_HYLIA && !FUN_0033caf8(GET_PLAYER(play))) { // 3DS-only branch
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x1000487); // NA_SE_SY_CORRECT_CHIME
        }
        self->actionFunc = FUN_003e1370;
    }
    FUN_00373264(&self->dyna.actor, 0x01000161); // NA_SE_EV_ROCK_SLIDE - SFX_FLAG
}

void FUN_003e1370(BgHaka* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->dyna.unk_1A8 != 0.0f) {
        self->dyna.unk_1A8 = 0.0f;
        player->stateFlags2 &= ~PLAYER_STATE2_4;
    }
}

void FUN_001e923c(BgHaka* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->dyna.actor.params != 0) {
        self->dyna.actor.params--;
    }
    if (self->dyna.unk_1A8 != 0.0f) {
        self->dyna.unk_1A8 = 0.0f;
        player->stateFlags2 &= ~PLAYER_STATE2_4;
    }
    if (self->dyna.actor.params == 0) {
        self->actionFunc = FUN_003e4064;
    }
    BgHaka_CheckPlayerOnDirtPatch(self, player);
}
}
