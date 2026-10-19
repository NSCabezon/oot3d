extern "C" {
#include "z_bg_spot16_doughnut.hpp"

#define FLAGS 0x40400000

void BgSpot16Doughnut_Init(Actor* actor, GameState* state);
void BgSpot16Doughnut_Destroy(Actor* actor, GameState* state);
void BgSpot16Doughnut_Update(Actor* actor, GameState* state);
void BgSpot16Doughnut_Draw(Actor* actor, GameState* state);

void FUN_003a7bf0(Actor* actor, GameState* state);
void FUN_003a7b68(Actor* actor, GameState* state);

ActorInit Bg_Spot16_Doughnut_InitVars = {
    ACTOR_BG_SPOT16_DOUGHNUT,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_EFC_DOUGHNUT,
    sizeof(BgSpot16Doughnut),
    (ActorFunc)BgSpot16Doughnut_Init,
    (ActorFunc)BgSpot16Doughnut_Destroy,
    (ActorFunc)BgSpot16Doughnut_Update,
    (ActorFunc)BgSpot16Doughnut_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 11000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 5000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 5000, ICHAIN_STOP),
};

static s16 sScales[] = { 0, 0, 70, 210, 300, -1 };

void BgSpot16Doughnut_Init(Actor* actor, GameState* state) {
    BgSpot16Doughnut* self = (BgSpot16Doughnut*)actor;
    PlayState* play = (PlayState*)state;
    ZARInfo* zar;
    s32 params;

    Actor_ProcessInitChain(actor, sInitChain);
    zar = FUN_00372f38(actor, play, &self->skelAnimModel, 0, &self->skelAnimModel2, 1, 0);
    self->skelAnimModel->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0));
    self->skelAnimModel->unk_0C->SetAnimType(1);
    Actor_SetScale(actor, 0.1f);
    self->fireFlag = 0;
    self->envColorAlpha = 255;
    params = actor->params;
    if (params == 1 || params == 2 || params == 3 || params == 4) {
        Actor_SetScale(actor, sScales[params] * 1.0e-4f);
        actor->draw = FUN_003a7b68;
        actor->update = FUN_003a7bf0;
    } else {
        {
            s16 sceneNum = play->sceneNum;

            // ROM: cmp #0x51; beq; nop; ble default; then linear 0x52/0x63/0x6B tests.
            // Reproduced by an explicit ==/<= split before the switch.
            if (sceneNum == 0x51) {
                goto bigUncull;
            } else if (sceneNum <= 0x51) {
                goto defaultScale;
            }
            switch (sceneNum) {
                case 0x6B:
                bigUncull:
                    actor->uncullZoneForward = 25000.0f;
                    actor->uncullZoneScale = 20000.0f;
                    /* fallthrough */
                default:
                defaultScale:
                    Actor_SetScale(actor, 0.1f);
                    break;
                case 0x52:
                    Actor_SetScale(actor, 0.12f);
                    break;
                case 0x63:
                    Actor_SetScale(actor, 0.15f);
                    actor->uncullZoneForward = 20000.0f;
                    actor->uncullZoneScale = 20000.0f;
                    break;
            }
        }
        if (!LINK_IS_ADULT || (gSaveContext.eventChkInf[2] & 0x8000)) {
            self->fireFlag &= ~1;
        } else if (*(u8*)((u8*)play + 0x107) != 0) {
            self->fireFlag &= ~1;
        } else {
            self->fireFlag |= 1;
        }
    }
}

void BgSpot16Doughnut_Destroy(Actor* actor, GameState* state) {
    BgSpot16Doughnut* self = (BgSpot16Doughnut*)actor;

    FUN_00350f34(actor, &self->skelAnimModel, &self->skelAnimModel2, 0);
}

void BgSpot16Doughnut_Update(Actor* actor, GameState* state) {
    BgSpot16Doughnut* self = (BgSpot16Doughnut*)actor;
    PlayState* play = (PlayState*)state;

    if (!(self->fireFlag & 1)) {
        actor->shape.rot.y -= 0x20;
        if (self->envColorAlpha < 255) {
            self->envColorAlpha += 5;
        } else {
            self->envColorAlpha = 255;
        }
    } else if (play->GetCutsceneState() != 0 && *(void**)((u8*)play + 0x22E4) != NULL &&
               **(u16**)((u8*)play + 0x22E4) == 2) {
        if (self->envColorAlpha > 5) {
            self->envColorAlpha -= 5;
        } else {
            self->envColorAlpha = 0;
            self->fireFlag &= ~1;
        }
    }
}

void BgSpot16Doughnut_Draw(Actor* actor, GameState* state) {
    BgSpot16Doughnut* self = (BgSpot16Doughnut*)actor;
    MTX34 modelMtx;
    f32 animSpeed;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->fireFlag & 1) {
        animSpeed = 1.0f;
        if (PauseContext_GetState() != 0) {
            animSpeed = 0.0f;
        }
        FUN_003695cc(self->skelAnimModel, 0, 4, 0, 1.0f, 0.0f, 0.0f, (f32)self->envColorAlpha * (1.0f / 255.0f));
        if (self->skelAnimModel != NULL) {
            self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
            self->skelAnimModel->SetUnkAC(1);
            self->skelAnimModel->FUN_003721e0(&modelMtx);
            self->skelAnimModel->FUN_00372170(1);
        }
    } else if (self->skelAnimModel2 != NULL) {
        FUN_003695cc(self->skelAnimModel2, 0, 4, 0, 1.0f, 1.0f, 1.0f, (f32)self->envColorAlpha * (1.0f / 255.0f));
        self->skelAnimModel2->SetUnkAC(1);
        self->skelAnimModel2->FUN_003721e0(&modelMtx);
        self->skelAnimModel2->FUN_00372170(1);
    }
}

// Update function for outwardly expanding and dissipating
void FUN_003a7bf0(Actor* actor, GameState* state) {
    BgSpot16Doughnut* self = (BgSpot16Doughnut*)actor;

    if (self->envColorAlpha > 5) {
        self->envColorAlpha -= 5;
    } else {
        Actor_Kill(actor);
    }
    actor->shape.rot.y -= 0x20;
    Actor_SetScale(actor, actor->scale.x + 0.0019999998f);
}

// Draw function for outwardly expanding and dissipating
void FUN_003a7b68(Actor* actor, GameState* state) {
    BgSpot16Doughnut* self = (BgSpot16Doughnut*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel2 != NULL) {
        FUN_003695cc(self->skelAnimModel2, 0, 4, 2, 1.0f, 1.0f, 1.0f, (f32)self->envColorAlpha * (1.0f / 255.0f));
        self->skelAnimModel2->SetUnkAC(1);
        self->skelAnimModel2->FUN_003721e0(&modelMtx);
        self->skelAnimModel2->FUN_00372170(1);
    }
}

}
