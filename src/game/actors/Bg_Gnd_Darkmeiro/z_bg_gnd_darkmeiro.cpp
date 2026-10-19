extern "C" {
#include "z_bg_gnd_darkmeiro.hpp"

#define FLAGS 0x80400030

#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

void BgGndDarkmeiro_Init(Actor* thisx, PlayState* play2);
void BgGndDarkmeiro_Destroy(Actor* thisx, PlayState* play2);
void BgGndDarkmeiro_Update(Actor* thisx, PlayState* play);
void FUN_0036d940(Actor* actor, s32 timer);

ActorInit Bg_Gnd_Darkmeiro_InitVars = {
    ACTOR_BG_GND_DARKMEIRO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DEMO_KEKKAI,
    sizeof(BgGndDarkmeiro),
    (ActorFunc)BgGndDarkmeiro_Init,
    (ActorFunc)BgGndDarkmeiro_Destroy,
    (ActorFunc)BgGndDarkmeiro_Update,
    NULL,
};

// Init: not matched yet (see STATUS.md). Draft kept under #if 0.
#if 0
void BgGndDarkmeiro_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    CollisionHeader* colHeader = NULL;
    BgGndDarkmeiro* self = (BgGndDarkmeiro*)thisx;

    self->updateFunc = (BgGndDarkmeiroActionFunc)FUN_003f5450;
    Actor_SetScale(&self->dyna.actor, 0.1f);
    switch (PARAMS_GET_U(self->dyna.actor.params, 0, 8)) {
        case 0:
            self->dyna.actor.draw = (ActorFunc)BgGndDarkmeiro_DrawInvisiblePath;
            self->dyna.actor.flags |= 0x80;
            FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel1, 0xA, 0);
            break;
        case 1:
            FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel1, 0xE, 0);
            if (play->sceneNum == 7 && *((u8*)play + 0x4C30) == 0xB && ((u8*)&gSaveContext)[0xE] != 0) {
                FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel2, 0xE, 0);
            } else {
                self->skelAnimModel2 = NULL;
            }
            colHeader = (CollisionHeader*)DynaPolyInfo_Alloc((DynaPolyActor*)thisx, (GameState*)play,
                                                             ((~self->dyna.actor.params & 0xFF00) && (self->dyna.actor.params & 0x4000)) ? 4 : 2);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
            if (PARAMS_GET_U(self->dyna.actor.params, 8, 6) == 0x3F) {
                self->updateFunc = (BgGndDarkmeiroActionFunc)FUN_0012aacc;
                self->dyna.actor.draw = (ActorFunc)BgGndDarkmeiro_DrawStaticBlock;
            } else {
                self->actionFlags = self->timer1 = self->timer2 = self->timer3 = 0;
                self->dyna.actor.draw = (ActorFunc)BgGndDarkmeiro_DrawSwitchBlock;
                self->updateFunc = BgGndDarkmeiro_UpdateSwitchBlock;
                if (!Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6))) {
                    FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
                } else {
                    self->timer1 = 64;
                    self->actionFlags |= 2;
                }
            }
            break;
        case 2:
            self->actionFlags = self->timer1 = self->timer2 = 0;
            self->updateFunc = BgGndDarkmeiro_UpdateBlockTimer;
            self->dyna.actor.draw = NULL;
            if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6) + 1)) {
                self->timer1 = 64;
                self->actionFlags |= 4;
            }
            if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6) + 2)) {
                self->timer2 = 64;
                self->actionFlags |= 8;
            }
            if ((self->timer1 != 0) || (self->timer2 != 0)) {
                Flags_SetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6));
            } else {
                Flags_UnsetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6));
            }
            break;
    }
}
#endif

GLOBAL_ASM("asm/BgGndDarkmeiro_Init.s")

// Matched (Stage 1)
void BgGndDarkmeiro_Destroy(Actor* thisx, PlayState* play2) {
    BgGndDarkmeiro* self = (BgGndDarkmeiro*)thisx;

    if (PARAMS_GET_U(self->dyna.actor.params, 0, 8) == 1) {
        DynaPoly_DeleteBgActor(play2, &play2->colCtx.dyna, self->dyna.bgId);
    }
    FUN_00350f34(thisx, &self->skelAnimModel1, 0);
    if (self->skelAnimModel2 != NULL) {
        FUN_00350f34(thisx, &self->skelAnimModel2, 0);
    }
}

// Matched (Stage 1)
void BgGndDarkmeiro_Update(Actor* thisx, PlayState* play) {
    BgGndDarkmeiro* self = (BgGndDarkmeiro*)thisx;

    self->updateFunc(self, play);
}

// Drafts for the functions still in functions_*.cpp (not matched yet; not compiled):
// FUN_001292d4 (UpdateBlockTimer), FUN_0012aad0 (UpdateSwitchBlock),
// FUN_00382314 (DrawInvisiblePath), FUN_00382520 (DrawStaticBlock). FUN_00382360 (DrawSwitchBlock) has no draft.
#if 0
void BgGndDarkmeiro_UpdateBlockTimer(BgGndDarkmeiro* self, PlayState* play) {
    s16 timeLeft;

    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6) + 1)) {
        if (self->actionFlags & 4) {
            if (self->timer1 > 0) {
                self->timer1--;
            } else {
                Flags_UnsetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6) + 1);
                self->actionFlags &= ~4;
            }
        } else {
            self->actionFlags |= 4;
            self->timer1 = 304;
            Audio_PlaySoundGeneral(0x010001D8, NULL, 4, (f32*)0x0054AC20, (f32*)0x0054AC20, (s8*)0x0054AC24);
        }
    }

    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6) + 2)) {
        if (self->actionFlags & 8) {
            if (self->timer2 > 0) {
                self->timer2--;
            } else {
                Flags_UnsetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6) + 2);
                self->actionFlags &= ~8;
            }
        } else {
            self->actionFlags |= 8;
            self->timer2 = 304;
            Audio_PlaySoundGeneral(0x010001D8, NULL, 4, (f32*)0x0054AC20, (f32*)0x0054AC20, (s8*)0x0054AC24);
        }
    }

    timeLeft = CLAMP_MIN(self->timer1, self->timer2);
    if (timeLeft > 0) {
        FUN_0036d940(&self->dyna.actor, timeLeft);
    }
    if ((self->timer1 >= 64) || (self->timer2 >= 64)) {
        Flags_SetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6));
    } else {
        Flags_UnsetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6));
    }
}

void BgGndDarkmeiro_UpdateSwitchBlock(BgGndDarkmeiro* self, PlayState* play) {
    if (self->timer1 > 0) {
        self->timer1--;
    }
    if (self->timer3 > 0) {
        self->timer3--;
    }
    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6))) {
        self->timer1 = 64;
    }
    if (self->actionFlags & 2) {
        if (self->timer1 == 0) {
            FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
            self->actionFlags &= ~2;
        }
    } else if (self->timer1 != 0) {
        FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);
        self->actionFlags |= 2;
    }
}

void BgGndDarkmeiro_DrawInvisiblePath(Actor* thisx, PlayState* play) {
    BgGndDarkmeiro* self = (BgGndDarkmeiro*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    if (self->skelAnimModel1 != NULL) {
        ((u8*)self->skelAnimModel1)[0xAC] = 1;
        self->skelAnimModel1->FUN_003721e0(&mtx);
        self->skelAnimModel1->FUN_00372170(0);
    }
}

void BgGndDarkmeiro_DrawStaticBlock(Actor* thisx, PlayState* play) {
    BgGndDarkmeiro* self = (BgGndDarkmeiro*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    if (self->skelAnimModel1 != NULL) {
        ((u8*)self->skelAnimModel1)[0xAC] = 1;
        self->skelAnimModel1->FUN_003721e0(&mtx);
        self->skelAnimModel1->FUN_00372170(1);
    }
}
#endif
}
