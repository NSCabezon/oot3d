extern "C" {
#include "z_en_a_obj.hpp"

#define FLAGS 0x00000010

void EnAObj_Init(Actor* thisx, PlayState* play);
void EnAObj_Destroy(Actor* actor, GameState* state);
void EnAObj_Update(Actor* actor, GameState* state);
void EnAObj_Draw(Actor* actor, GameState* state);

ActorInit En_A_Obj_InitVars = {
    ACTOR_EN_A_OBJ,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnAObj),
    (ActorFunc)EnAObj_Init,
    (ActorFunc)EnAObj_Destroy,
    (ActorFunc)EnAObj_Update,
    (ActorFunc)EnAObj_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    { 0x0A, 0x00, 0x39, 0x39, 0x20, 0x01 },
    { 2, { 0x00000000, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 0, 1, 1 },
    { 25.0f, 60.0f, 0.0f, { 0.0f, 0.0f, 0.0f } },
};

void FUN_00393d44(EnAObj* self, PlayState* play);
void FUN_003e2548(EnAObj* self, PlayState* play);
s32 FUN_00369a48(Actor* actor, PlayState* play);
void FUN_00363cb8(Actor* actor, PlayState* play);
__attribute__((noinline)) void FUN_003a5768(EnAObj* self, s32 unused);

void FUN_003a5768(EnAObj* self, s32 unused) {
    self->actionFunc = (ActorFunc)FUN_00393d44;
}

void FUN_003e2548(EnAObj* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        FUN_003a5768(self, self->actor.params);
    }
}

void FUN_00393d44(EnAObj* self, PlayState* play) {
    s16 yaw;

    if (self->actor.textId != 0) {
        yaw = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        if ((u32)(yaw + 0x2700 + 0xFF) > 0x4FFE) {
            if (self->actor.params != 0xA) {
                return;
            }
            if ((u32)(yaw + 0x5800) <= 0xB000) {
                return;
            }
        }
        if (Actor_ProcessTalkRequest(&self->actor, play)) {
            self->actionFunc = (ActorFunc)FUN_003e2548;
            return;
        }
        FUN_00363cb8(&self->actor, play);
    }
}

void FUN_0038112c(EnAObj* self, PlayState* play);
/* Block-rotate action: unlabeled tail inside caseD_3_00393ae4 (cfg split pending); EQU in data_syms.s. */
void EnAObj_BlockRot(EnAObj* self, PlayState* play);
void FUN_0039db30(EnAObj* self, PlayState* play);

static inline void EnAObj_SetupAction(EnAObj* self, ActorFunc actionFunc) {
    self->actionFunc = actionFunc;
}

static inline void EnAObj_SetupBlockRot(EnAObj* self) {
    self->rotateState = 0;
    self->rotateWaitTimer = 15;
    self->actor.world.rot.y = 0;
    self->actor.shape.rot = self->actor.world.rot;
    EnAObj_SetupAction(self, (ActorFunc)EnAObj_BlockRot);
}

void EnAObj_Init(Actor* thisx, PlayState* play) {
    CollisionHeader* colHeader = NULL;
    EnAObj* self = (EnAObj*)thisx;
    f32 shadowScale = 6.0f;
    s32 type;

    self->textIdParam = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    thisx->unk_19A = 1;

    u8 modelIdx[12] = { 0x18, 0x18, 0x18, 0x1A, 0x1A, 0x1B, 0x16, 0x1D, 0x1C, 0x1E, 0x1F, 0x04 };
    type = thisx->params;
    if (type > 0xB) {
        type = 0xB;
    }
    FUN_00372f38(thisx, play, &self->skelAnimModel, modelIdx[type], 0);

    switch (thisx->params) {
        case 0:
            Actor_SetScale(thisx, 0.025f);
            break;
        case 1:
            Actor_SetScale(thisx, 0.05f);
            break;
        case 2:
        case 5:
        case 6:
            Actor_SetScale(thisx, 0.1f);
            break;
        case 3:
            Actor_SetScale(thisx, 0.005f);
            break;
        case 4:
        default:
            Actor_SetScale(thisx, 0.01f);
            break;
    }

    if (thisx->params >= 9) {
        shadowScale = 12.0f;
    }

    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, shadowScale);

    thisx->uncullZoneDownward = 1200.0f;
    thisx->uncullZoneScale = 200.0f;
    thisx->focus.pos = thisx->world.pos;
    self->bgId = -1;
    self->interactFlags = 0;
    self->transformFlags = 0;

    switch (thisx->params) {
        case 1:
        case 2:
            self->bgId = 1;
            Actor_ChangeType(play, &play->actorCtx, thisx, ACTORCAT_BG);
            thisx->uncullZoneDownward = 1200.0f;
            thisx->uncullZoneScale = 720.0f;
            EnAObj_SetupAction(self, (ActorFunc)FUN_0038112c);
            break;
        case 3:
        case 4:
            self->bgId = 3;
            Actor_ChangeType(play, &play->actorCtx, thisx, ACTORCAT_BG);
            EnAObj_SetupBlockRot(self);
            break;
        case 6:
            self->unk_208 = 10.0f;
            self->actor.flags |= 1;
            thisx->gravity = -2.0f;
            EnAObj_SetupAction(self, (ActorFunc)FUN_00393d44);
            self->bgId = 5;
            break;
        case 7:
        case 8:
            EnAObj_SetupAction(self, (ActorFunc)FUN_00393d44);
            self->bgId = 0;
            break;
        case 9:
        case 10:
            thisx->textId = (self->textIdParam & 0xFF) | 0x300;
            thisx->targetArrowOffset = 500.0f;
            self->unk_208 = 45.0f;
            thisx->flags |= 9;
            EnAObj_SetupAction(self, (ActorFunc)FUN_00393d44);
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, thisx, &sCylinderInit);
            thisx->colChkInfo.mass = 0xFF;
            thisx->targetMode = 0;
            break;
        case 11:
            thisx->gravity = -1.5f;
            EnAObj_SetupAction(self, (ActorFunc)FUN_0039db30);
            break;
        default:
            thisx->gravity = -2.0f;
            EnAObj_SetupAction(self, (ActorFunc)FUN_00393d44);
            break;
    }

    if (thisx->params < 5) {
        thisx->colChkInfo.mass = 0xFF;
    }

    if (self->bgId != -1) {
        u8 colIdx[8] = { 0, 0, 3, 4, 5, 5, 0, 0 };
        void* zsi = play->GetObjectStatus(thisx->objBankIndex)->zarInfo.FUN_003532c0(colIdx[self->bgId]);
        ReadCollisionHeaderFromZsi(zsi, &colHeader);
        self->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    }
}

void EnAObj_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnAObj* self = (EnAObj*)actor;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->bgId);
    if (self->actor.params == 9 || self->actor.params == 10) {
        Collider_DestroyCylinder(play, &self->collider);
    }
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void EnAObj_Update(Actor* actor, GameState* state) {
    EnAObj* self = (EnAObj*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(actor, state);
    Actor_MoveForward(actor);
    if (actor->gravity != 0.0f) {
        if (actor->params != 0xB) {
            Actor_UpdateBgCheckInfo(play, actor, 5.0f, 40.0f, 0.0f, 0x1D);
        } else {
            Actor_UpdateBgCheckInfo(play, actor, 5.0f, 20.0f, 0.0f, 0x1D);
        }
    }

    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += self->unk_208;

    if (actor->params == 9 || actor->params == 10) {
        Collider_UpdateCylinder(actor, &self->collider);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}

GLOBAL_ASM("../src/game/actors/En_A_Obj/data_syms.s")

typedef struct EnAObjDrawColor {
    f32 c[4];
} __attribute__((aligned(4))) EnAObjDrawColor;

extern EnAObjDrawColor EnAObj_sDrawColor;

void* FUN_003687a8(SkeletonAnimationModel* model);
void FUN_003589cc(void* p, s32 n);
void FUN_00358964(void* p, s32 n, void* color);

void EnAObj_Draw(Actor* actor, GameState* state) {
    EnAObj* self = (EnAObj*)actor;

    if (self->skelAnimModel != NULL) {
        Actor* thisx = actor;

        if (thisx->params == 0xB) {
            EnAObjDrawColor color = EnAObj_sDrawColor;
            void* p = FUN_003687a8(self->skelAnimModel);

            FUN_003589cc(p, 4);
            FUN_00358964(p, 4, &color);
        }
        self->skelAnimModel->FUN_003721e0(&self->actor.modelMtx);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
