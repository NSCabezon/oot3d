extern "C" {
#include "z_en_sth.hpp"

#define FLAGS 0x00000019
#define EN_STH_WALLET CUR_UPG_VALUE(4) // UPG_WALLET
#define EN_STH_GS_TOKENS (gSaveContext.inventory.gsTokens)
#define EN_STH_EVENTCHKINF_SKULLTULA_REWARD (gSaveContext.eventChkInf[13])

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);

void EnSth_Init(EnSth* self, PlayState* play);
void EnSth_Destroy(EnSth* self, PlayState* play);
void EnSth_Update(EnSth* self, PlayState* play);

void FUN_0010f9b4(EnSth* self, PlayState* play); // N64: EnSth_WaitForObject
void FUN_00111e7c(EnSth* self, PlayState* play); // N64: EnSth_ParentRewardObtainedWait
void FUN_00111ee8(EnSth* self, PlayState* play); // N64: EnSth_ChildRewardObtainedWait
void FUN_00111f68(EnSth* self, PlayState* play); // N64: EnSth_RewardObtainedTalk
void FUN_0012cb98(EnSth* self, PlayState* play); // N64: EnSth_RewardUnobtainedWait
void FUN_0012cc10(EnSth* self, PlayState* play); // N64: EnSth_RewardUnobtainedTalk
void FUN_0012ccdc(EnSth* self, PlayState* play); // N64: EnSth_GiveReward
s32 FUN_00369a48(Actor* actor, PlayState* play); // N64: Actor_TextboxIsClosing
void FUN_00369960(Actor* actor, PlayState* play); // N64: EnSth_FacePlayer (shared)
void FUN_00369a68(Actor* actor, PlayState* play); // N64: EnSth_LookAtPlayer (shared)
void FUN_003725e0(PlayState* play);              // N64: Message_CloseTextbox
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void FUN_001df8c0(EnSth* self, PlayState* play); // N64: EnSth_Draw
void FUN_003cd920(EnSth* self, PlayState* play); // N64: EnSth_SetupAfterObjectLoaded
s32 Collider_DestroyCylinder(PlayState* play, ColliderCylinder* collider);
void SkelAnime_Free2(SkelAnime* skelAnime);

void FUN_001dfa30(Actor* thisx, GameState* state); // N64: EnSth_Update2
void SkelAnime_SetCurrentFrame2(SkelAnime* skelAnime, f32 frame);

void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_00357a50(SkelAnime* skelAnime, s32 idx, s32 count, void* colors, s32 arg4);
void FUN_0015074c();
void FUN_0016dc1c();

typedef struct {
    f32 r, g, b, a;
} EnSthColor;

// Lives in .bss (runtime-filled model index table), no ROM data: defined as an EQU in syms.s.
extern s32 EnSth_sModelIdx[];

GLOBAL_ASM("../src/game/actors/En_Sth/syms.s")

ActorInit En_Sth_InitVars = {
    ACTOR_EN_STH,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnSth),
    (ActorFunc)EnSth_Init,
    (ActorFunc)EnSth_Destroy,
    (ActorFunc)EnSth_Update,
    NULL,
};

u32 EnSth_sUnused = 0;

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 40, 0, { 0, 0, 0 } },
};

static s16 sObjectIds[] = { 0x0107, 0x0111, 0x0111, 0x0111, 0x0111, 0x0111 };

static s32 sAnimIdx[] = { 3, 8, 8, 8, 8, 8 };

static EnSthActionFunc sRewardObtainedWaitActions[] = {
    FUN_00111e7c, FUN_00111ee8, FUN_00111ee8, FUN_00111ee8, FUN_00111ee8, FUN_00111ee8,
};

static u16 sEventFlags[] = { 0x0000, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000 };

static u16 sGetItemIds[] = { 0x56, 0x45, 0x39, 0x46, 0x03, 0x3E };

#define EN_STH_COLOR(r, g, b) { (r) / 255.0f, (g) / 255.0f, (b) / 255.0f, 1.0f }

static EnSthColor sParentShirtColor = EN_STH_COLOR(190, 110, 0);
static EnSthColor sParentPantsColor = EN_STH_COLOR(190, 110, 0);
// [0] = children's pants, [params] = children's shirts
static EnSthColor sShirtColors[] = {
    EN_STH_COLOR(90, 110, 130), EN_STH_COLOR(0, 180, 110),   EN_STH_COLOR(0, 255, 80),
    EN_STH_COLOR(255, 160, 60), EN_STH_COLOR(190, 230, 250), EN_STH_COLOR(240, 230, 120),
};

void EnSth_Init(EnSth* self, PlayState* play) {
    s16 objectId;
    s32 params = self->actor.params;
    s32 objectSlot;

    if (self->actor.params == 0) {
        if (EN_STH_GS_TOKENS < 100) {
            Actor_Kill(&self->actor);
            return;
        }
    } else if (EN_STH_GS_TOKENS < (self->actor.params * 10)) {
        Actor_Kill(&self->actor);
        return;
    }

    {
        s16* objectIds = sObjectIds;
        objectId = objectIds[params];
    }
    if (objectId != OBJECT_GAMEPLAY_KEEP) {
        objectSlot = Object_GetIndex(&play->objectCtx, objectId);
    } else {
        objectSlot = 0;
    }

    ActorFunc draw = (ActorFunc)FUN_001df8c0;
    self->requiredObjectSlot = objectSlot;
    self->drawFunc = draw;
    Actor_SetScale(&self->actor, 0.01f);
    self->actionFunc = FUN_0010f9b4;
    self->actor.draw = NULL;
    self->unk_296 = 0;
    self->actor.targetMode = 6;
}

void EnSth_Destroy(EnSth* self, PlayState* play) {
    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(&self->actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void FUN_0010f9b4(EnSth* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        self->actor.objBankIndex = self->requiredObjectSlot;
        self->actionFunc = FUN_003cd920;
    }
}

void EnSth_Update(EnSth* self, PlayState* play) {
    self->actionFunc(self, play);
}

// N64: EnSth_Update2
void FUN_001dfa30(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnSth* self = (EnSth*)thisx;

    if (self->skelAnime.animationType == 8) {
        self->skelAnime.unk_80[1] = 1;
    } else {
        self->skelAnime.unk_80[1] = 0;
    }

    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(play, thisx, 0.0f, 0.0f, 0.0f, 4);
    if (FUN_00370734(&self->skelAnime)) {
        SkelAnime_SetCurrentFrame2(&self->skelAnime, 0.0f);
    }
    self->actionFunc(self, play);

    if (DECR(self->unk_29A) == 0) {
        self->unk_29A = Rand_S16Offset_003702c8(0x3C, 0x3C);
    }
    self->unk_298 = self->unk_29A;
    if (self->unk_298 >= 3) {
        self->unk_298 = 0;
    }
}

void FUN_001df8c0(EnSth* self, PlayState* play) {
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    if (*(u16*)&self->actor.params == 0) {
        FUN_0036932c(self->skelAnime.unk_28, 1);
        FUN_0036932c(self->skelAnime.unk_28, 2);
        FUN_0036932c(self->skelAnime.unk_28, 3);
        FUN_0036932c(self->skelAnime.unk_28, 4);
        FUN_0037266c(self->skelAnime.unk_28, 4);
        FUN_00357a50(&self->skelAnime, 0, 4, &sParentShirtColor, 1);
        FUN_00357a50(&self->skelAnime, 1, 3, &sParentPantsColor, 1);
    } else {
        FUN_0036932c(self->skelAnime.unk_28, 8);
        FUN_0036932c(self->skelAnime.unk_28, 2);
        FUN_0036932c(self->skelAnime.unk_28, 3);
        FUN_0036932c(self->skelAnime.unk_28, 4);
        FUN_0036932c(self->skelAnime.unk_28, 5);
        FUN_0036932c(self->skelAnime.unk_28, 6);
        FUN_0036932c(self->skelAnime.unk_28, 7);
        FUN_0037266c(self->skelAnime.unk_28, 7);
        FUN_00357a50(&self->skelAnime, 0, 4, &sShirtColors[self->actor.params], 1);
        FUN_00357a50(&self->skelAnime, 1, 3, sShirtColors, 1);
    }
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_0016dc1c,
                      (PostLimbDrawOpa)FUN_0015074c, &self->actor, 0);
}

static void EnSth_SetupShapeColliderUpdate2AndDraw(Actor* thisx, PlayState* play) {
    EnSth* self = (EnSth*)thisx;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = 0xFF;
    self->actor.update = (ActorFunc)FUN_001dfa30;
    self->actor.draw = self->drawFunc;
}

// N64: EnSth_SetupAfterObjectLoaded
void FUN_003cd920(EnSth* self, PlayState* play) {
    Actor* thisx = &self->actor;
    s16* params;

    EnSth_SetupShapeColliderUpdate2AndDraw(&self->actor, play);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel, EnSth_sModelIdx[thisx->params], 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, EnSth_sModelIdx[thisx->params], sAnimIdx[thisx->params],
                 NULL, NULL, thisx->params != 0 ? 0x13 : 0x10);
    if (thisx->params != 0) {
        Animation_Change_00375c08(&self->skelAnime, sAnimIdx[thisx->params], 1.5f, 0.0f,
                                  (f32)Animation_GetLastFrame(&self->skelAnime, sAnimIdx[thisx->params]), 0,
                                  0.0f);
    } else {
        Animation_PlayLoop(&self->skelAnime, sAnimIdx[0]);
    }

    params = &thisx->params;
    self->eventFlag = sEventFlags[*params];
    if (EN_STH_EVENTCHKINF_SKULLTULA_REWARD & self->eventFlag) {
        self->actionFunc = sRewardObtainedWaitActions[*params];
    } else {
        self->actionFunc = FUN_0012cb98;
    }
}

void FUN_00111e7c(EnSth* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = FUN_00111f68;
    } else {
        self->actor.textId = 0x23;
        if (self->actor.xzDistToPlayer < 100.0f) {
            FUN_0036bb28(&self->actor, play, 100.0f);
        }
    }
    FUN_00369a68(&self->actor, play);
}

void FUN_00111ee8(EnSth* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = FUN_00111f68;
    } else {
        if (EN_STH_GS_TOKENS < 50) {
            self->actor.textId = 0x20;
        } else {
            self->actor.textId = 0x1F;
        }
        if (self->actor.xzDistToPlayer < 100.0f) {
            FUN_0036bb28(&self->actor, play, 100.0f);
        }
    }
    FUN_00369a68(&self->actor, play);
}

void FUN_00111f68(EnSth* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        if (self->actor.params == 0) {
            self->actionFunc = FUN_00111e7c;
        } else {
            self->actionFunc = FUN_00111ee8;
        }
    }
    FUN_00369960(&self->actor, play);
}

void FUN_0012cb98(EnSth* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = FUN_0012cc10;
    } else {
        if (self->actor.params == 0) {
            self->actor.textId = 0x28;
        } else {
            self->actor.textId = 0x21;
        }
        if (self->actor.xzDistToPlayer < 100.0f) {
            FUN_0036bb28(&self->actor, play, 100.0f);
        }
    }
    FUN_00369a68(&self->actor, play);
}

void FUN_0012cc10(EnSth* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        u16 getItemId;
        FUN_003725e0(play);
        self->actionFunc = FUN_0012ccdc;
        getItemId = sGetItemIds[self->actor.params];
        switch (self->actor.params) {
            case 1:
            case 3:
                switch (EN_STH_WALLET) {
                    case 0:
                        getItemId = 0x45;
                        break;
                    case 1:
                        getItemId = 0x46;
                        break;
                }
                break;
        }
        GiveItem(&self->actor, play, getItemId, 10000.0f, 50.0f);
    }
    FUN_00369960(&self->actor, play);
}

void FUN_0012ccdc(EnSth* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = FUN_00111f68;
        EN_STH_EVENTCHKINF_SKULLTULA_REWARD |= self->eventFlag;
    } else {
        u16 getItemId = sGetItemIds[self->actor.params];
        switch (self->actor.params) {
            case 1:
            case 3:
                switch (EN_STH_WALLET) {
                    case 0:
                        getItemId = 0x45;
                        break;
                    case 1:
                        getItemId = 0x46;
                        break;
                }
                break;
        }
        GiveItem(&self->actor, play, getItemId, 10000.0f, 50.0f);
    }
    FUN_00369960(&self->actor, play);
}
}
