extern "C" {
#include "z_bg_haka_ship.hpp"

#define FLAGS 0x80400030

#define SFX_BLOCKSINK 0x010001b3 // NA_SE_EV_BLOCKSINK - SFX_FLAG

void BgHakaShip_Init(Actor* actor, GameState* state);
void BgHakaShip_Destroy(Actor* actor, GameState* state);
void BgHakaShip_Update(Actor* actor, GameState* state);
void BgHakaShip_Draw(Actor* actor, GameState* state);
void BgHakaShip_ChildUpdatePosition(BgHakaShip* self, PlayState* play);
void BgHakaShip_WaitForSong(BgHakaShip* self, PlayState* play);
void BgHakaShip_CutsceneStationary(BgHakaShip* self, PlayState* play);
void BgHakaShip_Move(BgHakaShip* self, PlayState* play);
void BgHakaShip_SetupCrash(BgHakaShip* self, PlayState* play);
void BgHakaShip_CrashShake(BgHakaShip* self, PlayState* play);
void BgHakaShip_CrashFall(BgHakaShip* self, PlayState* play);

f32 __hardfp_sinf(f32 x);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void FUN_0049fa58(Vec3f* pos);                 // Audio_StopSfxByPos
void FUN_00373264(Actor* actor, s32 sfxId);    // Actor_PlaySfx_Flagged
s32 FUN_0036adf4(DynaPolyActor* dyna);         // DynaPolyActor_IsPlayerOnTop
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);
s32 Gameplay_InCsMode(PlayState* play);
void Matrix_RotateZ(MTX34* mtx, f32 angle, s32 mode);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);

ActorInit Bg_Haka_Ship_InitVars = {
    ACTOR_BG_HAKA_SHIP,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKA_OBJECTS,
    sizeof(BgHakaShip),
    (ActorFunc)BgHakaShip_Init,
    (ActorFunc)BgHakaShip_Destroy,
    (ActorFunc)BgHakaShip_Update,
    (ActorFunc)BgHakaShip_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHakaShip_Init(Actor* actor, GameState* state) {
    BgHakaShip* self = (BgHakaShip*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    FUN_00372f38(actor, play, &self->skelAnimModel0, 0x16, &self->skelAnimModel1, 0xb, &self->skelAnimModel2, 0xb,
                 &self->skelAnimModel3, 0x17, 0);
    self->switchFlag = actor->params >> 8;
    actor->params &= 0xFF;

    if (actor->params == 0) {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0x10), &colHeader);
        self->counter = 12;
        self->actionFunc = BgHakaShip_WaitForSong;
    } else {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0x11), &colHeader);
        self->actionFunc = BgHakaShip_ChildUpdatePosition;
    }
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->dyna.actor.world.rot.y = self->dyna.actor.shape.rot.y - 0x4000;
    self->yOffset = 0;
    if (actor->params == 0) {
        PlayState* p2 = play;
        if (Actor_SpawnAsChild(&p2->actorCtx, &self->dyna.actor, play, ACTOR_BG_HAKA_SHIP,
                               actor->world.pos.x + -10.0f, actor->world.pos.y + 82.0f, actor->world.pos.z, 0, 0,
                               0, 1) == NULL) {
            Actor_Kill(actor);
        }
    }
}

void BgHakaShip_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgHakaShip* self = (BgHakaShip*)actor;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_0049fa58(&self->bellSfxPos);
    FUN_00350f34(actor, &self->skelAnimModel0, &self->skelAnimModel1, &self->skelAnimModel2,
                 &self->skelAnimModel3, 0);
}

void BgHakaShip_Update(Actor* actor, GameState* state) {
    BgHakaShip* self = (BgHakaShip*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
    if (self->dyna.actor.params == 0) {
        Actor_MoveForward(&self->dyna.actor);
    }
}

void BgHakaShip_ChildUpdatePosition(BgHakaShip* self, PlayState* play) {
    Actor* parent = self->dyna.actor.parent;

    if (parent != NULL && parent->update != NULL) {
        self->dyna.actor.world.pos.x = parent->world.pos.x + -10.0f;
        self->dyna.actor.world.pos.y = parent->world.pos.y + 82.0f;
        self->dyna.actor.world.pos.z = parent->world.pos.z;
    } else {
        self->dyna.actor.parent = NULL;
    }
}

void BgHakaShip_WaitForSong(BgHakaShip* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->switchFlag)) {
        if (self->counter) {
            self->counter--;
        }
        if (self->counter == 0) {
            self->counter = 195;
            self->actionFunc = BgHakaShip_CutsceneStationary;
            OnePointCutscene_Init(play, 3390, 999, &self->dyna.actor, 0);
        }
    }
}

void BgHakaShip_CutsceneStationary(BgHakaShip* self, PlayState* play) {
    if (self->counter) {
        self->counter--;
    }
    self->yOffset = __hardfp_sinf(self->counter * (M_PI * 2.0f / 75.0f)) * 6144.0f;
    if (self->counter == 0) {
        self->counter = 75;
        self->actionFunc = BgHakaShip_Move;
    }
}

void BgHakaShip_Move(BgHakaShip* self, PlayState* play) {
    f32 distanceFromHome;
    Actor* child;

    if (self->counter) {
        self->counter--;
    }
    if (self->counter == 0) {
        self->counter = 75;
    }
    self->dyna.actor.world.pos.y =
        (__hardfp_sinf(self->counter * (M_PI * 2.0f / 75.0f)) * 50.0f) + self->dyna.actor.home.pos.y;

    distanceFromHome = self->dyna.actor.home.pos.x - self->dyna.actor.world.pos.x;
    if (distanceFromHome > 7650.0f) {
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x - 7650.0f;
        self->dyna.actor.speedXZ = 0.0f;
    }
    if (distanceFromHome > 7600.0f && !Gameplay_InCsMode(play)) {
        self->counter = 60;
        self->dyna.actor.speedXZ = 0.0f;
        if (GET_PLAYER(play)->actor.world.pos.y >= -1250.0f && GET_PLAYER(play)->actor.world.pos.z < -1745.0f) {
            FUN_00367c7c(play, 0x5071, NULL);
        }
        self->actionFunc = BgHakaShip_SetupCrash;
    } else {
        Math_StepToF(&self->dyna.actor.speedXZ, 4.0f, 0.2f);
    }
    child = self->dyna.actor.child;
    if (child != NULL && child->update != NULL) {
        child->shape.rot.z += ((655.0f / 13.0f) * self->dyna.actor.speedXZ);
    } else {
        self->dyna.actor.child = NULL;
    }
    self->yOffset = __hardfp_sinf(self->counter * (M_PI * 2.0f / 75.0f)) * 6144.0f;
}

void BgHakaShip_SetupCrash(BgHakaShip* self, PlayState* play) {
    if (self->counter) {
        self->counter--;
    }
    if (self->counter == 0) {
        self->counter = 60;
        self->actionFunc = BgHakaShip_CrashShake;
    }
    Math_ScaledStepToS(&self->yOffset, 0, 128);
}

void BgHakaShip_CrashShake(BgHakaShip* self, PlayState* play) {
    if (self->counter != 0) {
        self->counter--;
    }
    self->dyna.actor.world.pos.y = (self->counter % 4 - 2) * 3 + self->dyna.actor.home.pos.y;
    if (!self->counter) {
        self->dyna.actor.gravity = -1.0f;
        self->actionFunc = BgHakaShip_CrashFall;
    }
    FUN_00373264(&self->dyna.actor, SFX_BLOCKSINK);
}

void BgHakaShip_CrashFall(BgHakaShip* self, PlayState* play) {
    Actor* child;

    if (self->dyna.actor.home.pos.y - self->dyna.actor.world.pos.y > 2000.0f) {
        Actor_Kill(&self->dyna.actor);
        child = self->dyna.actor.child;
        if (child != NULL && child->update != NULL) {
            Actor_Kill(child);
        }
    } else {
        Audio_PlayActorSound2(&self->dyna.actor, SFX_BLOCKSINK);
        if ((self->dyna.actor.home.pos.y - self->dyna.actor.world.pos.y > 500.0f) &&
            FUN_0036adf4(&self->dyna)) {
            Gameplay_TriggerVoidOut(play);
        }
    }
}

static inline void HT(MTX34* m, f32 x, f32 y, f32 z) {
 Vec3f t; t.x=x; t.y=y; t.z=z;
 Matrix_UpdatePosition(m, m, &t);
}
void BgHakaShip_Draw(Actor* actor, GameState* state) {
    BgHakaShip* self = (BgHakaShip*)actor;
    MTX34 mtx;
    f32 angleTemp;

    MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);
    if (self->dyna.actor.params == 0) {
        if (((BgHakaShip*)actor)->skelAnimModel0 != NULL) {
            self->skelAnimModel0->SetUnkAC(1);
            self->skelAnimModel0->FUN_003721e0(&mtx);
            self->skelAnimModel0->FUN_00372170(0);
        }
        s16 yo = self->yOffset;
        f32 k = M_PI / 32768.0f;
        angleTemp = yo * k;
        HT(&mtx, -4858.0f, 860.0f, 926.0f);
        Matrix_RotateZ(&mtx, angleTemp, 1);
        if (self->skelAnimModel1 != NULL) {
            self->skelAnimModel1->SetUnkAC(1);
            self->skelAnimModel1->FUN_003721e0(&mtx);
            self->skelAnimModel1->FUN_00372170(0);
        }
        HT(&mtx, 0.0f, 0.0f, -1852.0f);
        Matrix_RotateZ(&mtx, angleTemp * -2.0f, 1);
        if (self->skelAnimModel2 != NULL) {
            self->skelAnimModel2->SetUnkAC(1);
            self->skelAnimModel2->FUN_003721e0(&mtx);
            self->skelAnimModel2->FUN_00372170(0);
        }
    } else {
        if (((BgHakaShip*)actor)->skelAnimModel3 != NULL) {
            self->skelAnimModel3->SetUnkAC(1);
            self->skelAnimModel3->FUN_003721e0(&mtx);
            self->skelAnimModel3->FUN_00372170(0);
        }
    }

    if (self->actionFunc == BgHakaShip_CutsceneStationary || self->actionFunc == BgHakaShip_Move) {
        f32 x = self->dyna.actor.world.pos.x + -485.80002f;
        f32 y = self->dyna.actor.world.pos.y + 86.0f;
        f32 z = self->dyna.actor.world.pos.z;
        self->bellSfxPos.z = z;
        self->bellSfxPos.y = y;
        self->bellSfxPos.x = x;
        Audio_PlaySoundGeneral(0x010001A6, &self->bellSfxPos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
}


}
