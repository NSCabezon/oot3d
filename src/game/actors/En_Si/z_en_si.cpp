extern "C" {
#include "z_en_si.hpp"

#define FLAGS 0x00000201
#define R_UPDATE_RATE SREG(30)

void EnSi_Init(Actor* actor, GameState* state);
void EnSi_Destroy(Actor* actor, GameState* state);
void EnSi_Update(Actor* actor, GameState* state);
void EnSi_Draw(Actor* actor, GameState* state);

s32 Player_InCsMode(PlayState* play);
s32 Item_Give(PlayState* play, s32 item);
void FUN_0035c528(u32 seqId);

void FUN_003ad218(EnSi* self, PlayState* play); // hookshot-attached action (func_80AFB89C)
void FUN_003adc80(EnSi* self, PlayState* play); // idle action (func_80AFB768)
void FUN_003d0544(EnSi* self, PlayState* play); // collected action (func_80AFB950)

GLOBAL_ASM("data/z_En_Si.data.s")

ActorInit En_Si_InitVars = {
    ACTOR_EN_SI,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_ST,
    sizeof(EnSi),
    (ActorFunc)EnSi_Init,
    (ActorFunc)EnSi_Destroy,
    (ActorFunc)EnSi_Update,
    (ActorFunc)EnSi_Draw,
};

#include "model_manager.hpp"

void* ZAR_GetCMBByIndex(void* zar, s32 idx);

void EnSi_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnSi* self = (EnSi*)actor;
    ZARInfo* zar;
    ModelAllocator* alloc;

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, (ColliderCylinderInit*)0x0052EB60);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, (CollisionCheckInfoInit2*)0x0052EB98);
    Actor_SetScale(&self->actor, 0.025f);
    self->actor.shape.yOffset = 42.0f;
    self->unk_200 = 0;
    self->actionFunc = FUN_003adc80;
    self->actor.unk_19A = 1;

    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    alloc = GetModelManager().allocator;
    alloc->owner = self->actor.unk_178;
    self->skelAnimModels[0] = alloc->Create(ZAR_GetCMBByIndex(zar, 2), 1);
    self->skelAnimModels[1] = alloc->Create(ZAR_GetCMBByIndex(zar, 3), 1);
    alloc->owner = NULL;
    self->unk_20C = self->skelAnimModels[1]->unk_0C;
    self->unk_20C->FUN_00372d94(zar->GetCMABByIndex(0));
    self->unk_20C->animSpeed = 2.0f;
    self->unk_20C->animType = 1;
}

void EnSi_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnSi* self = (EnSi*)actor;
    s32 i;

    Collider_DestroyCylinder(play, &self->collider);
    for (i = 0; i < 2; i++) {
        if (self->skelAnimModels[i] != NULL) {
            SkeletonAnimationModel* model = self->skelAnimModels[i];

            ((void (*)(SkeletonAnimationModel*))((void**)model->vTable)[1])(model);
            self->skelAnimModels[i] = NULL;
        }
    }
}

void EnSi_Update(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnSi* self = (EnSi*)actor;

    Actor_MoveForward(actor);
    Actor_UpdateBgCheckInfo(play, actor, 0.0f, 0.0f, 0.0f, 4 /* UPDBGCHECKINFO_FLAG_2 */);
    self->actionFunc(self, play);
    Actor_SetFocus(actor, 16.0f);
}

void EnSi_Draw(Actor* actor, GameState* state) {
    EnSi* self = (EnSi*)actor;
    s32 i;

    if (self->actionFunc != FUN_003d0544) {
        self->unk_20C->animSpeed = PauseContext_GetState() == 0 ? 2.0f : 0.0f;
        for (i = 0; i < 2; i++) {
            self->skelAnimModels[i]->unk_AC = 1;
            self->skelAnimModels[i]->FUN_003721e0(&self->actor.modelMtx);
            self->skelAnimModels[i]->FUN_00372170(0);
        }
    }
}

void FUN_003d0544(EnSi* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (*(u16*)((u8*)&gSaveContext + 0x44) != 0 && *(u16*)((u8*)play + 0x318C) == 0) {
        if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
            Actor_Kill(&self->actor);
        } else {
            player->actor.freezeTimer = (s32)(30.0f / R_UPDATE_RATE + 0.5f);
        }
    }
}

static void EnSi_GiveSkullToken(EnSi* self, PlayState* play, Player* player) {
    Item_Give(play, ITEM_SKULL_TOKEN);
    {
        s16 params = self->actor.params;

        *(u32*)((u8*)&gSaveContext + 0xEB4 + ((params >> 8) & 0x1C)) |= (params & 0xFF) << gGsFlagsShift[(params >> 8) & 3];
    }
    player->actor.freezeTimer = (s32)(30.0f / R_UPDATE_RATE + 0.5f);
    FUN_00367c7c(play, 0xB4, NULL); // Message_StartTextbox(play, TEXT_ID_SKULLTULA_TOKEN, NULL)
    if (*(u16*)((u8*)&gSaveContext + 0x44) != 0) {
        if (*(u16*)((u8*)play + 0x318C) == 0) {
            FUN_0035c528(0x010005A6);
        }
    }
}

void FUN_003adc80(EnSi* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->actor.flags & 0x2000) {
        self->actionFunc = FUN_003ad218;
        return;
    }
    Math_SmoothStepToF(&self->actor.scale.x, 0.25f, (R_UPDATE_RATE / 3.0f) * 0.4f, 1.0f, 0.0f);
    Actor_SetScale(&self->actor, self->actor.scale.x);
    self->actor.shape.rot.y = (s16)(self->actor.shape.rot.y + (R_UPDATE_RATE / 3.0f) * 1024.0f);

    if (!Player_InCsMode(play)) {
        if (self->collider.base.acFlags & AC_HIT) {
            self->collider.base.acFlags &= ~AC_HIT;
        }
        if (self->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
            self->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
            EnSi_GiveSkullToken(self, play, player);
            self->actionFunc = FUN_003d0544;
        } else {
            Collider_UpdateCylinder(&self->actor, &self->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        }
    }
}

void FUN_003ad218(EnSi* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&self->actor.scale.x, 0.25f, 0.4f, 1.0f, 0.0f);
    Actor_SetScale(&self->actor, self->actor.scale.x);
    self->actor.shape.rot.y += 0x400;

    if (!(self->actor.flags & 0x2000)) {
        EnSi_GiveSkullToken(self, play, player);
        self->actionFunc = FUN_003d0544;
    }
}
}
