extern "C" {
#include "z_en_siofuki.hpp"

#define FLAGS 0x80400030

void EnSiofuki_Init(Actor* actor, GameState* state);
void EnSiofuki_Destroy(Actor* actor, GameState* state);
void EnSiofuki_Update(Actor* actor, GameState* state);
void EnSiofuki_Draw(Actor* actor, GameState* state);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_001cd768(EnSiofuki*, PlayState*);
void FUN_0021de14(EnSiofuki*, PlayState*);
void FUN_0025a514(EnSiofuki*, PlayState*);
void FUN_0036ef10(PosRot* pos, s32 sfxId, f32 volume);

GLOBAL_ASM("data/z_En_Siofuki.data.s")

ActorInit En_Siofuki_InitVars = {
    ACTOR_EN_SIOFUKI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SIOFUKI,
    sizeof(EnSiofuki),
    (ActorFunc)EnSiofuki_Init,
    (ActorFunc)EnSiofuki_Destroy,
    (ActorFunc)EnSiofuki_Update,
    (ActorFunc)EnSiofuki_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void EnSiofuki_Init(Actor* actor, GameState* state) {
    EnSiofuki* self = (EnSiofuki*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    if ((actor->room == 10) && Flags_GetSwitch(play, 0x1E)) {
        Actor_Kill(actor);
        return;
    }

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    {
        ZARInfo* zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
        self->skelAnimModel->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0));
    }
    self->skelAnimModel->unk_0C->SetAnimType(1);
    self->skelAnimModel->unk_0C->SetAnimSpeed(2.0f);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->sfxFlags |= 1;

    if (!(((u16)actor->params >> 12 == 0) || ((u16)actor->params >> 12 == 1))) {
        Actor_Kill(actor);
        return;
    }

    self->unk_200.z = 1.0f;
    self->unk_200.y = 1.0f;
    self->unk_200.x = 1.0f;
    self->unk_1FC = 0;
    if ((gSaveContext.masterQuestFlag == 1) && (play->sceneNum == 2) && (actor->room == 0xE) &&
        ((s32)actor->home.pos.y == -1073)) {
        actor->world.pos.x = 699.0f;
        actor->world.pos.y = -1150.0f;
        actor->world.pos.z = -2376.0f;
        self->unk_1FC = 1;
        self->unk_200.z = 0.9f;
        self->unk_200.x = 0.9f;
    }

    self->initPosY = actor->world.pos.y;
    self->unk_174 = 35.0f;
    self->unk_170 = -6058.0f + self->unk_174;

    if (actor->shape.rot.x != 0) {
        self->maxHeight = actor->shape.rot.x * 40.0f;
        self->currentHeight = self->maxHeight;
    }
    self->activeTime = 0;
    if (actor->shape.rot.y != 0) {
        self->activeTime = actor->shape.rot.y;
    }
    if (actor->shape.rot.z != 0) {
        actor->scale.x = actor->shape.rot.z * (1.0f / 1.73f) * 0.1f;
        actor->scale.z = actor->shape.rot.z * 0.5f * 0.1f;
    }

    actor->world.rot.x = 0;
    actor->world.rot.y = 0;
    actor->world.rot.z = 0;
    actor->shape.rot.x = 0;
    actor->shape.rot.y = 0;
    actor->shape.rot.z = 0;

    if ((u16)actor->params >> 12 == EN_SIOFUKI_RAISING) {
        self->currentHeight = 10.0f;
        self->targetHeight = 10.0f;
        self->actionFunc = (EnSiofukiActionFunc)FUN_001cd768;
    } else if ((u16)actor->params >> 12 == EN_SIOFUKI_LOWERING) {
        if (Flags_GetTreasure(play, actor->params & 0x3F)) {
            self->currentHeight = -45.0f;
            self->targetHeight = -45.0f;
            self->actionFunc = (EnSiofukiActionFunc)FUN_0021de14;
        } else {
            self->targetHeight = self->currentHeight;
            self->actionFunc = (EnSiofukiActionFunc)FUN_0025a514;
        }
    }

    if (self->unk_1FC != 0) {
        actor->scale.x = 0.147f;
        actor->scale.z = 0.147f;
    }
}

void EnSiofuki_Destroy(Actor* actor, GameState* state) {
    EnSiofuki* self = (EnSiofuki*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void EnSiofuki_Update(Actor* actor, GameState* state) {
    EnSiofuki* self = (EnSiofuki*)actor;

    self->actionFunc(self, (PlayState*)state);
}

struct SiofukiVec {
    f32& operator[](s32 i) {
        return (&x)[i];
    }
    f32 x, y, z;
};
static inline void SiofukiTranslate(MTX34* mtx, f32 x, f32 y) {
    SiofukiVec pos;
    pos[0] = x;
    pos[1] = y;
    pos[2] = 0.0f;
    Matrix_UpdatePosition(mtx, mtx, (Vec3f*)&pos);
}
void EnSiofuki_Draw(Actor* actor, GameState* state) {
    EnSiofuki* self = (EnSiofuki*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    f32 animSpeed;
    f32 heightRatio;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    animSpeed = 2.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }
    SiofukiTranslate(&mtx, 0.0f, self->unk_170);
    {
        f32 sx = self->unk_200.x;
        f32 sy = self->unk_200.y;
        f32 sz = self->unk_200.z;

        mtx.data[0][0] *= sx;
        mtx.data[1][0] *= sx;
        mtx.data[2][0] *= sx;
        mtx.data[0][1] *= sy;
        mtx.data[1][1] *= sy;
        mtx.data[2][1] *= sy;
        mtx.data[0][2] *= sz;
        mtx.data[1][2] *= sz;
        mtx.data[2][2] *= sz;
    }

    self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&mtx);
    self->skelAnimModel->FUN_00372170(0);

    if (self->sfxFlags & 1) {
        u32 sfxId = 0x01000198;
        f32 vol = 1.0f;
        switch ((u16)actor->params >> 12) {
            case EN_SIOFUKI_LOWERING:
                if (self->currentHeight > -35.0f) {
                    heightRatio = (self->currentHeight - -35.0f) / (self->maxHeight - -35.0f);
                    break;
                }
                return;
            case EN_SIOFUKI_RAISING:
                heightRatio = (self->currentHeight - 10.0f) / (400.0f - 10.0f);
                if ((play->sceneNum == 2) && (heightRatio < 0.35f)) {
                    return;
                }
                break;
            default:
                return;
        }
        FUN_0036ef10(&actor->world, sfxId, heightRatio + vol);
    }
}
}
