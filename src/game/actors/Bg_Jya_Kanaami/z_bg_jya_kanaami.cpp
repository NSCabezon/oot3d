extern "C" {
#include "z_bg_jya_kanaami.hpp"

#define FLAGS 0x80400000
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

void BgJyaKanaami_Init(Actor* actor, GameState* state);
void BgJyaKanaami_Destroy(Actor* actor, GameState* state);
void BgJyaKanaami_Update(Actor* actor, GameState* state);
void BgJyaKanaami_Draw(Actor* actor, GameState* state);

s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
s32 Quake_Add(Camera* cam, u32 type);
void Quake_SetSpeed(s32 idx, s16 speed);
void Quake_SetQuakeValues(s32 idx, s16 y, s16 x, s16 zoom, s16 rollOffset);
void Quake_SetCountdown(s32 idx, s16 countdown);

void FUN_003bf458(BgJyaKanaami* self, PlayState* play);
void FUN_003bc4b8(BgJyaKanaami* self, PlayState* play);

static inline void BgJyaKanaami_SetWait(BgJyaKanaami* self) {
    self->actionFunc = FUN_003bf458;
    self->unk_16A = 0;
}

static inline void BgJyaKanaami_SetDone(BgJyaKanaami* self) {
    self->actionFunc = NULL;
    self->dyna.actor.world.rot.x = 0x4000;
}

ActorInit Bg_Jya_Kanaami_InitVars = {
    ACTOR_BG_JYA_KANAAMI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaKanaami),
    (ActorFunc)BgJyaKanaami_Init,
    (ActorFunc)BgJyaKanaami_Destroy,
    (ActorFunc)BgJyaKanaami_Update,
    (ActorFunc)BgJyaKanaami_Draw,
};

InitChainEntry sBgJyaKanaamiInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 700, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgJyaKanaami_Init(Actor* actor, GameState* state) {
    BgJyaKanaami* self = (BgJyaKanaami*)actor;
    PlayState* play = (PlayState*)state;
    void* colInfo;

    FUN_00372f38(actor, play, &self->skelAnimModel, 0xD, 0);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 2);
    {
        CollisionHeader* colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(colInfo, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Actor_ProcessInitChain(actor, sBgJyaKanaamiInitChain);
    if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 0, 6))) {
        BgJyaKanaami_SetDone(self);
    } else {
        BgJyaKanaami_SetWait(self);
    }
}

void BgJyaKanaami_Destroy(Actor* actor, GameState* state) {
    BgJyaKanaami* self = (BgJyaKanaami*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void FUN_003bf458(BgJyaKanaami* self, PlayState* play) {
    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 0, 6)) || self->unk_16A > 0) {
        if (self->dyna.actor.world.pos.x > -1000.0f && self->unk_16A == 0) {
            OnePointCutscene_Init(play, 3450, -99, &self->dyna.actor, 0);
        }
        self->unk_16A++;
        if (self->unk_16A >= 10) {
            self->actionFunc = FUN_003bc4b8;
            self->unk_168 = 0;
        }
    }
}

// N64: func_80899950
void FUN_003bc4b8(BgJyaKanaami* self, PlayState* play) {
    s32 quakeIndex;
    s16 step;

    step = self->unk_168 + 0x20;
    self->unk_168 = step;
    if (Math_ScaledStepToS(&self->dyna.actor.world.rot.x, 0x4000, step)) {
        Actor* thisx = &self->dyna.actor;

        self->actionFunc = NULL;
        thisx->world.rot.x = 0x4000;
        Audio_PlayActorSound2(thisx, 0x010001A8);

        quakeIndex = Quake_Add(play->cameraPtrs[play->activeCamera], 3);
        Quake_SetSpeed(quakeIndex, 25000);
        Quake_SetQuakeValues(quakeIndex, 2, 0, 0, 0);
        Quake_SetCountdown(quakeIndex, 16);
    }
}

void BgJyaKanaami_Update(Actor* actor, GameState* state) {
    BgJyaKanaami* self = (BgJyaKanaami*)actor;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, (PlayState*)state);
    }
    actor->shape.rot.x = actor->world.rot.x;
}

static inline void BgJyaKanaami_ScaleCol(MTX34* mtx, s32 col, f32 scale) {
    mtx->data[0][col] *= scale;
    mtx->data[1][col] *= scale;
    mtx->data[2][col] *= scale;
}

void BgJyaKanaami_Draw(Actor* actor, GameState* state) {
    BgJyaKanaami* self = (BgJyaKanaami*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    Vec3f scale;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    scale.x = 10.0f;
    scale.y = 10.0f;
    scale.z = 10.0f;
    BgJyaKanaami_ScaleCol(&mtx, 0, scale.x);
    BgJyaKanaami_ScaleCol(&mtx, 1, scale.y);
    BgJyaKanaami_ScaleCol(&mtx, 2, scale.z);
    if (self->skelAnimModel != NULL) {
        FUN_00357fd0(GET_PLAYER(play), actor->unk_178, &actor->world.pos);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

}
