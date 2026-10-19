extern "C" {
#include "z_bg_bdan_objects.hpp"

#define FLAGS 0x00400010

void BgBdanObjects_Init(Actor* thisx, PlayState* play);
void BgBdanObjects_Destroy(Actor* actor, PlayState* state);
void BgBdanObjects_Update(Actor* actor, PlayState* state);
void BgBdanObjects_Draw(Actor* actor, GameState* state);

s32 FUN_0036adf4(DynaPolyActor* dyna);
f32 cosf(f32 x);
f32 sinf(f32 x);
s32 Gameplay_InCsMode(PlayState* play);
void Camera_ChangeSetting(Camera* camera, s32 setting);
void FUN_0036762c(Camera* camera, s32 flag);
void FUN_00367618(Camera* camera, s32 flag);
s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s32 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s32 a, s32 b, s32 c, s32 d);
void Quake_SetCountdown(s32 quakeIndex, s32 countdown);
void FUN_0036ae48(Camera* camera);
void FUN_001c832c(BgBdanObjects* self, PlayState* play);
void FUN_00155374(BgBdanObjects* self, PlayState* play);
void FUN_0036d940(Actor* actor, s32 timer);
void FUN_00338654(PlayState* play, s32 camId, s32 camIdNone);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);

void FUN_0019fde4(BgBdanObjects* self, PlayState* play);
void FUN_0019fba8(BgBdanObjects* self, PlayState* play);
void FUN_0019fc94(BgBdanObjects* self, PlayState* play);
void FUN_00177914(BgBdanObjects* self, PlayState* play);
void FUN_00196978(BgBdanObjects* self, PlayState* play);
void FUN_0019975c(BgBdanObjects* self, PlayState* play);
void FUN_003e82ec(BgBdanObjects* self, PlayState* play);
void FUN_00177bb0(BgBdanObjects* self, PlayState* play);
void BgBdanObjects_DoNothing(BgBdanObjects* self, PlayState* play);
void FUN_0019a1b0(BgBdanObjects* self, PlayState* play);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);

typedef struct {
    /* 0x00 */ char unk_00[0x2];
    /* 0x02 */ s16 ySurface;
    /* 0x04 */ char unk_04[0xC];
} BgBdanObjectsWaterBox; // size = 0x10

GLOBAL_ASM("data/z_Bg_Bdan_Objects.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Bdan_Objects/data_syms.s")

// Init chain and cylinder init live in the data dump above; absolute EQU symbols in data_syms.s.
extern InitChainEntry BgBdanObjects_sInitChain[];
extern ColliderCylinderInit BgBdanObjects_sCylinderInit;

ActorInit Bg_Bdan_Objects_InitVars = {
    ACTOR_BG_BDAN_OBJECTS,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_BDAN_OBJECTS,
    sizeof(BgBdanObjects),
    (ActorFunc)BgBdanObjects_Init,
    (ActorFunc)BgBdanObjects_Destroy,
    (ActorFunc)BgBdanObjects_Update,
    (ActorFunc)BgBdanObjects_Draw,
};

void BgBdanObjects_Init(Actor* thisx, PlayState* play) {
    BgBdanObjects* self = (BgBdanObjects*)thisx;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;

    Actor_ProcessInitChain(thisx, BgBdanObjects_sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    self->switchFlag = (thisx->params >> 8) & 0x3F;
    thisx->params &= 0xFF;
    thisx->unk_19A = 1;
    self->unk_230 = 0;
    zar = FUN_00372f38(thisx, play, &self->skelAnimModel[0], 12,
                                                      &self->skelAnimModel[1], 7, &self->skelAnimModel[2], 6,
                                                      &self->skelAnimModel[3], 8, 0);
    self->skelAnimModel[2]->unk_0C->FUN_00372d94(zar->GetCMABByIndex(4));
    self->skelAnimModel[2]->unk_0C->SetAnimType(1);
    if (thisx->params == 2) {
        thisx->flags |= 0x30;
        ((BgBdanObjectsWaterBox*)play->colCtx.stat.colHeader->waterBoxes)[7].ySurface = thisx->world.pos.y;
        self->actionFunc = FUN_0019fde4;
        return;
    }
    if (thisx->params == 0) {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 3), &colHeader);
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, thisx, &BgBdanObjects_sCylinderInit);
        thisx->world.pos.y += -79.0f;
        if (Flags_GetClear(play, thisx->room)) {
            Flags_SetSwitch(play, self->switchFlag);
            self->actionFunc = FUN_00196978;
        } else {
            if (gSaveContext.infTable[20] & 0x40) {
                if (Actor_SpawnAsChild(&play->actorCtx, thisx, play, ACTOR_EN_BIGOKUTA,
                                       thisx->home.pos.x, thisx->home.pos.y,
                                       thisx->home.pos.z, 0, thisx->shape.rot.y + 0x8000, 0,
                                       3) != NULL) {
                    thisx->child->world.pos.z = thisx->child->home.pos.z + 263.0f;
                }
                thisx->world.pos.y = thisx->home.pos.y + -70.0f;
                thisx->world.rot.y = 0;
                self->actionFunc = FUN_003e82ec;
            } else {
                Flags_SetSwitch(play, self->switchFlag);
                self->timer = 0;
                self->actionFunc = FUN_0019a1b0;
            }
        }
    } else if (thisx->params == 1) {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
        self->timer = 768;
        self->switchFlag = 0;
        self->actionFunc = FUN_0019975c;
    } else {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 1), &colHeader);
        if (Flags_GetSwitch(play, self->switchFlag)) {
            thisx->world.pos.y = thisx->home.pos.y - 400.0f;
            self->actionFunc = BgBdanObjects_DoNothing;
        } else {
            self->actionFunc = FUN_00177bb0;
        }
    }
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
}

void BgBdanObjects_Destroy(Actor* thisx, PlayState* play) {
    BgBdanObjects* self = (BgBdanObjects*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    if (thisx->params == 0) {
        Collider_DestroyCylinder(play, &self->collider);
    }
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1],
                                               &self->skelAnimModel[2], &self->skelAnimModel[3], 0);
}

static inline void BgBdanObjects_Translate(MTX34* mtx, f32 y, f32 x, f32 z) {
    Vec3f translate;

    translate.x = x;
    translate.y = y;
    translate.z = z;
    Matrix_UpdatePosition(mtx, mtx, &translate);
}

void BgBdanObjects_Draw(Actor* thisx, GameState* state) {
    BgBdanObjects* self = (BgBdanObjects*)thisx;
    MTX34 modelMtx;
    f32 animSpeed;

    MTX34CopyAsm(&modelMtx, &thisx->modelMtx);
    animSpeed = 1.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }

    if (thisx->params == 0) {
        if (self->actionFunc == FUN_0019a1b0) {
            f32 posY;
            f32 margin;
            f32 homeY;
            f32 offset;
            f32 limit;

            posY = thisx->world.pos.y;
            offset = -79.0f;
            margin = 5.0f;
            homeY = thisx->home.pos.y;
            limit = (homeY + offset) - margin;
            if (posY > limit) {
                BgBdanObjects_Translate(&modelMtx, -50.0f, 0.0f, 0.0f);
            }
        }
    }

    if (thisx->params == 2) {
        if (self->skelAnimModel[2] != NULL) {
            self->skelAnimModel[2]->unk_0C->SetAnimSpeed(animSpeed);
            self->skelAnimModel[2]->SetUnkAC(1);
            self->skelAnimModel[2]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[2]->FUN_00372170(0);
        }
        return;
    }

    {
        s16 idx = thisx->params;
        if (self->skelAnimModel[idx] != NULL) {
            self->skelAnimModel[idx]->SetUnkAC(1);
            self->skelAnimModel[idx]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[idx]->FUN_00372170(0);
        }
    }
}

void BgBdanObjects_DoNothing(BgBdanObjects* self, PlayState* play) {
}

void BgBdanObjects_Update(Actor* thisx, PlayState* play) {
    Actor_SetFocus(thisx, 50.0f);
    ((BgBdanObjects*)thisx)->actionFunc((BgBdanObjects*)thisx, play);
}

void FUN_0019fde4(BgBdanObjects* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->switchFlag)) {
        self->timer = 150;
        self->actionFunc = FUN_0019fba8;
        self->unk_230 = 1;
    }
}

void FUN_0019fba8(BgBdanObjects* self, PlayState* play) {
    if (self->timer == 0) {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 0.5f)) {
            Flags_UnsetSwitch(play, self->switchFlag);
            self->actionFunc = FUN_0019fde4;
        }
        FUN_0035ae08(&self->dyna.actor, 0x010001b5);
    } else {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 75.0f, 0.5f)) {
            self->actionFunc = FUN_0019fc94;
        }
        FUN_0035ae08(&self->dyna.actor, 0x010001b5);
    }
    ((BgBdanObjectsWaterBox*)play->colCtx.stat.colHeader->waterBoxes)[7].ySurface = self->dyna.actor.world.pos.y;
}

void FUN_0019fc94(BgBdanObjects* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    FUN_0036d940(&self->dyna.actor, self->timer);
    if (self->timer == 0) {
        self->actionFunc = FUN_0019fba8;
        self->unk_230 = 0;
    }
}

static inline void BgBdan_SetTimer(BgBdanObjects* self, s16 v) {
    s16* p = &self->timer;
    *p = v;
}

void FUN_00177bb0(BgBdanObjects* self, PlayState* play) {
    if (FUN_0036adf4(&self->dyna)) {
        Flags_SetSwitch(play, self->switchFlag);
        BgBdan_SetTimer(self, 75);
        self->dyna.actor.home.pos.y -= 200.0f;
        {
            PlayState* play2 = play;

            self->actionFunc = FUN_00177914;
            OnePointCutscene_Init(play2, 3100, 76, &self->dyna.actor, 0);
        }
    }
}

void FUN_0019975c(BgBdanObjects* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->switchFlag == 0) {
        if (FUN_0036adf4(&self->dyna)) {
            self->unk_21C = *(s16*)((u8*)play->cameraPtrs[0] + 0x18A);
            Camera_ChangeSetting(play->cameraPtrs[0], 0x3A);
            FUN_0036762c(play->cameraPtrs[0], 4);
            self->switchFlag = 10;
        }
    } else {
        Camera_ChangeSetting(play->cameraPtrs[0], 0x3A);
        if (!FUN_0036adf4(&self->dyna)) {
            if (self->switchFlag != 0) {
                self->switchFlag--;
            }
        }
        if (self->switchFlag == 0) {
            Camera_ChangeSetting(play->cameraPtrs[0], (s16)self->unk_21C);
            FUN_00367618(play->cameraPtrs[0], 4);
        }
    }

    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - (sinf(self->timer * (M_PI / 384.0f)) * 471.24f);
    if (self->timer == 0) {
        self->timer = 768;
    }
}

void FUN_00155374(BgBdanObjects* self, PlayState* play) {
    f32 smoothed = Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 500.0f, 0.5f, 7.5f, 1.0f);

    if (smoothed < 0.1f) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001f5);
        self->actionFunc = FUN_001c832c;
        self->timer = 45;
        self->unk_21C = 3;
        return;
    }

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->timer = 17;
    }
    FUN_00373264(&self->dyna.actor, 0x010001e6);
}

void FUN_0019a1b0(BgBdanObjects* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    s32 isNormal0 = self->unk_21C == 1;
    if (isNormal0) {
        if (self->dyna.actor.xzDistToPlayer < 250.0f) {
            self->unk_21C = 2;
            self->timer = 30;
            OnePointCutscene_Init(play, 3070, -99, &self->dyna.actor, 0);
            player->actor.world.pos.x = player->actor.home.pos.x = player->actor.prevPos.x = -1130.0f;
            player->actor.world.pos.y = player->actor.home.pos.y = player->actor.prevPos.y = -1025.0f;
            player->actor.world.pos.z = player->actor.home.pos.z = player->actor.prevPos.z = -3300.0f;
            Audio_PlayActorSound2(&self->dyna.actor, 0x010001ea);
        }
    } else if (self->timer != 0) {
        self->timer--;
        if (self->timer == 0) {
            self->actionFunc = FUN_00155374;
        }
    }

    bool ok = true;
    if (!Gameplay_InCsMode(play)) {
        ok = self->unk_21C == 1;
    }
    if (ok) {
        self->dyna.actor.world.pos.y = (self->dyna.actor.home.pos.y + -79.0f) - 5.0f;
    } else {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + -79.0f;
    }
}

void FUN_001998f4(BgBdanObjects* self, PlayState* play);

// FUN_001c832c NOT MATCHED: 13/77 words differ, only register permutation (ROM: quakeIndex r6, timer base this+0x100 r7, play+0xa00 r8; ours r8/r6/r7). ~20 variants tried.
// FUN_001998f4 is an unsplit function (0x001998f4, tail of EnWallmas_JumpToCeiling in functions.tsv) -> (cfg); EQU in data_syms.s.
#if 0
void FUN_001c832c(BgBdanObjects* self, PlayState* play) {
    s32 quakeIndex;

    if (self->timer != 0) {
        self->timer--;
        if (self->timer == 0) {
            quakeIndex = Quake_Add(play->cameraPtrs[*(s16*)((u8*)play + 0xA64)], 1);
            Quake_SetSpeed(quakeIndex, 0x3A98);
            Quake_SetQuakeValues(quakeIndex, 0, 1, 250, 1);
            Quake_SetCountdown(quakeIndex, 10);
        }
    }

    s32 isDungeon1 = self->unk_21C == 4;
    if (isDungeon1) {
        Actor_SpawnAsChild(&play->actorCtx, &self->dyna.actor, play, ACTOR_EN_BIGOKUTA, self->dyna.actor.world.pos.x,
                           self->dyna.actor.world.pos.y + 140.0f, self->dyna.actor.world.pos.z, 0,
                           self->dyna.actor.shape.rot.y + 0x8000, 0, 0);
        gSaveContext.infTable[20] |= 0x40;
        self->timer = 15;
        self->actionFunc = FUN_001998f4;
        FUN_0036ae48(play->cameraPtrs[*(s16*)((u8*)play + 0xA64)]);
    }
}
#endif

void FUN_00177914(BgBdanObjects* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - (cosf(self->timer * (M_PI / 75.0f)) * 200.0f);
    if (self->timer == 0) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001f6);
        self->actionFunc = BgBdanObjects_DoNothing;
        FUN_00338654(play, 0, -1);
    } else {
        FUN_00373264(&self->dyna.actor, 0x010001e7);
    }
}

}
