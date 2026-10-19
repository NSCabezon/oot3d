extern "C" {
#include "z_en_m_thunder.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000000
#define R_UPDATE_RATE SREG(30)
#define PLAYER_UNK_858(p) (*(f32*)((u8*)(p) + 0x2240))

void EnMThunder_Init(Actor* thisx, GameState* state);
void EnMThunder_Destroy(Actor* thisx, PlayState* play);
void EnMThunder_Update(EnMThunder* self, PlayState* play);
void EnMThunder_Draw(Actor* actor, GameState* state);

void FUN_003f4d9c(EnMThunder* self, PlayState* play); // N64 EnMThunder_ChargingSpinAttack
void FUN_0010fa78(EnMThunder* self, PlayState* play); // N64 EnMThunder_EmptySpinAttack
void FUN_00314988(Vec3f* pos, s32 level);
void FUN_00128c20(EnMThunder* self, PlayState* play); // N64 EnMThunder_SpinAttacking
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* light);
s32 FUN_003318bc(PlayState* play, s32 amount, s32 type); // Magic_RequestChange
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_0047d548(void* model, s32 arg);
void Matrix_Scale(MTX34* m, s32 mode, f32 x, f32 y, f32 z);
void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_003720a4(Player* player, MTX34* out);
void FUN_00333abc(void* mat, s32 idx, f32* out);
void FUN_00333a38(void* mat, s32 idx, f32* in);
u32 PauseContext_GetState(void);
void FUN_0034708c(PlayState* play); // Magic_Reset
void FUN_0036bee0(PlayState* play, f32 intensity, f32 a, f32 b, f32 c); // Environment_AdjustLights
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* light);
s32 Gameplay_InCsMode(PlayState* play);
void Lights_PointNoGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);

// Freer at +0x238: real virtual call, slot 1. _ZTI15EnMThunderFreer is an EQU in src/game/shared_syms.s (VFE key).
extern "C++" {
struct EnMThunderFreer {
    virtual void unk_vf0();
    virtual void Destroy();
};
}

GLOBAL_ASM("data/z_En_M_Thunder.data.s")
GLOBAL_ASM("../src/game/actors/En_M_Thunder/data_syms.s")

extern ColliderCylinderInit EnMThunder_sCylinderInit;
typedef struct {
    u32 spin[3];
    u32 jump[3];
} EnMThunderDmgTable;
extern EnMThunderDmgTable EnMThunder_sDmgTable;
extern u32 EnMThunder_sSfxIds[];
extern f32 EnMThunder_sSpinChargeScale[];

ActorInit En_M_Thunder_InitVars = {
    ACTOR_EN_M_THUNDER,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnMThunder),
    (ActorFunc)EnMThunder_Init,
    (ActorFunc)EnMThunder_Destroy,
    (ActorFunc)EnMThunder_Update,
    (ActorFunc)EnMThunder_Draw,
};

static inline void EnMThunder_SetupAction(EnMThunder* self, EnMThunderActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

// EnMThunder_Init: draft, 7 words off (r0/r1/r2 numbering around the stateFlags2 clear + dmgFlags table load; see STATUS.md)
void EnMThunder_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnMThunder* self = (EnMThunder*)thisx;
    Player* player = GET_PLAYER(play);

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnMThunder_sCylinderInit);
    self->swordType = thisx->params - 1;
    Lights_PointNoGlowSetInfo(self->unk_200, (f32)(s16)self->actor.world.pos.x, (f32)(s16)self->actor.world.pos.y,
                              (f32)(s16)self->actor.world.pos.z, 255, 255, 255, 0, 0);
    self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, self->unk_200);
    self->collider.dim.radius = 0.0f;
    self->collider.dim.height = 40.0f;
    self->collider.dim.yShift = -20.0f;
    self->spinTrailTexScroll = 0.0f;
    self->followPlayerTimer = 8;
    self->actor.world.pos = *(Vec3f*)((u8*)player + 0x2340);
    self->spinAttackTimer = 0.0f;
    self->dimmingIntensity = 0.0f;
    { s16 ry = player->actor.shape.rot.y + 0x8000; self->actor.shape.rot.y = ry; }
    self->actor.room = -1;
    Actor_SetScale(&self->actor, 0.1f);
    self->isUsingMagic = false;

    if (player->stateFlags2 & 0x20000) {
        EnMThunder* s2 = self;
        if (!gSaveContext.magicAcquired || (*(u16*)((u8*)&gSaveContext + 0x1580) != 0) ||
            ((self->actor.params & 0xFF00) && !FUN_003318bc(play, (self->actor.params & 0xFF00) >> 8, 0))) {
            Audio_PlaySoundGeneral(0x01000115, &player->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
            Audio_PlaySoundGeneral(0x0100010A, &player->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
            Actor_Kill(&self->actor);
            return;
        }

        player->stateFlags2 &= ~0x20000;
        self->isUsingMagic = true;
        self->collider.info.toucher.dmgFlags = EnMThunder_sDmgTable.spin[self->swordType];
        self->attackStrength = 1;
        {
            if (self->swordType == 1) {
                s2->targetScale = 2;
            } else {
                s2->targetScale = 4;
            }
        }
        self->actionFunc = FUN_00128c20;
        self->followPlayerTimer = 8;
        Audio_PlaySoundGeneral(0x01000120, &player->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        self->spinAttackTimer = 1.0f;
    } else {
        self->actionFunc = FUN_003f4d9c;
    }
    self->actor.child = NULL;
    self->actor.unk_19A = 1;

    {
        ZARInfo* zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
        ModelAllocator* alloc = GetModelManager().allocator;
        s32 i;

        alloc->owner = self->actor.unk_178;
        for (i = 0; i < 3; i++) {
            SkeletonAnimationModel* model = alloc->Create(ZAR_GetCMBByIndex(zar, 0xC + i), 1);

            self->freers[i] = (EnMThunderFreer*)model;
            FUN_0047d548(model, 2);
        }
        alloc->owner = NULL;
        self->unk_244 = 0;
        self->unk_248 = zar->GetCMABByIndex(0x13);
        self->unk_24C = zar->GetCMABByIndex(0x14);
        ((SkeletonAnimationModel*)self->freers[0])->unk_0C->SetAnimType(1);
        ((SkeletonAnimationModel*)self->freers[0])->unk_0C->FUN_00372d94(self->unk_248);
        ((SkeletonAnimationModel*)self->freers[0])->unk_0C->SetAnimSpeed(2.0f);
        self->unk_250 = 0;
        ((SkeletonAnimationModel*)self->freers[1])->unk_0C->SetAnimType(1);
        ((SkeletonAnimationModel*)self->freers[2])->unk_0C->SetAnimType(1);
        ((SkeletonAnimationModel*)self->freers[1])->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0x15));
        ((SkeletonAnimationModel*)self->freers[2])->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0x16));
    }
}


static inline void EnMThunder_AdjustEnvLights(PlayState* play, f32 intensity) {
    FUN_0036bee0(play, intensity, 800.0f, 0.0f, 0.0f);
}

void EnMThunder_Destroy(Actor* thisx, PlayState* play) {
    EnMThunder* self = (EnMThunder*)thisx;
    s32 i;

    if (self->isUsingMagic) {
        if (*(u16*)((u8*)&gSaveContext + 0x1580) != 7) {
            FUN_0034708c(play);
        }
    }

    Collider_DestroyCylinder(play, &self->collider);
    EnMThunder_AdjustEnvLights(play, 0.0f);
    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);

    for (i = 0; i < 3; i++) {
        if (self->freers[i] != NULL) {
            self->freers[i]->Destroy();
            self->freers[i] = NULL;
        }
    }
}

void EnMThunder_Update(EnMThunder* self, PlayState* play) {
    f32 blueRadius;
    s16 radius;
    f32 k800;

    self->actionFunc(self, play);
    EnMThunder_AdjustEnvLights(play, self->dimmingIntensity);
    blueRadius = self->spinAttackTimer;
    k800 = 800.0f;
    radius = (s32)(blueRadius * k800);
    Lights_PointNoGlowSetInfo(self->unk_200, (f32)(s16)self->actor.world.pos.x, (f32)(s16)self->actor.world.pos.y,
                              (f32)(s16)self->actor.world.pos.z, (u32)(blueRadius * 255.0f) & 0xFF,
                              (u32)(blueRadius * 255.0f) & 0xFF, (u32)(blueRadius * 100.0f), radius, 0);
}

static inline void EnMThunder_UpdateSpinAttack(EnMThunder* self, PlayState* play) {
    if (self->followPlayerTimer < (s32)(6.0f / R_UPDATE_RATE + 0.5f)) {
        if (self->chargeAlpha < 40) {
            self->chargeAlpha = 0;
        } else {
            self->chargeAlpha -= 40;
        }
    }
    self->spinTrailTexScroll += self->spinAttackAlpha * ((f32)R_UPDATE_RATE * (1.0f / 3.0f));
    if (self->dimmingIntensity < self->spinAttackTimer) {
        self->dimmingIntensity += ((self->spinAttackTimer - self->dimmingIntensity) * 0.1f * (f32)R_UPDATE_RATE * (1.0f / 3.0f));
    } else {
        self->dimmingIntensity = self->spinAttackTimer;
    }
}

// N64 EnMThunder_SpinAttacking (moved from functions_120000s.cpp).
void FUN_00128c20(EnMThunder* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Math_StepToF(&self->spinAttackTimer, 0.0f, 1 / 16.0f)) {
        Actor_Kill(&self->actor);
    } else {
        Math_SmoothStepToF(&self->actor.scale.x, (s32)self->targetScale, 0.6f, 0.8f, 0.0f);
        Actor_SetScale(&self->actor, self->actor.scale.x);
        self->collider.dim.radius = (self->actor.scale.x * 25.0f);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    }

    if (self->followPlayerTimer > 0) {
        self->actor.world.pos.x = ((Vec3f*)((u8*)player + 0x2340))->x;
        self->actor.world.pos.z = ((Vec3f*)((u8*)player + 0x2340))->z;
        self->followPlayerTimer--;
    }

    if (self->spinAttackTimer > 0.6f) {
        self->spinAttackAlpha = 1.0f;
    } else {
        self->spinAttackAlpha = self->spinAttackTimer * (5.0f / 3.0f);
    }

    EnMThunder_UpdateSpinAttack(self, play);

    if (Gameplay_InCsMode(play)) {
        Actor_Kill(&self->actor);
    }
    self->unk_244 = -(u32)(self->spinTrailTexScroll * 12288.0f);
    self->actor.shape.rot.y = player->actor.shape.rot.y + self->unk_244 + 0x8000;
}

// N64 EnMThunder_EmptySpinAttack (moved from functions_100000s.cpp).
void FUN_0010fa78(EnMThunder* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (player->stateFlags2 & 0x20000) {
        if (*((s8*)player + 0x2226) >= 0x18) {
            Audio_PlaySoundGeneral(0x01000115, &player->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
            Audio_PlaySoundGeneral(0x0100010A, &player->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
        Actor_Kill(&self->actor);
        return;
    }

    if (!(player->stateFlags1 & 0x1000)) {
        Actor_Kill(&self->actor);
    }
}

// N64 EnMThunder_ChargingSpinAttack.
void FUN_003f4d9c(EnMThunder* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* child = self->actor.child;

    self->spinChargePercent = PLAYER_UNK_858(player);
    self->actor.world.pos = *(Vec3f*)((u8*)player + 0x2340);
    self->actor.shape.rot.y = player->actor.shape.rot.y + 0x8000;

    if (!self->isUsingMagic) {
        if (PLAYER_UNK_858(player) >= 0.1f) {
            if ((*(u16*)((u8*)&gSaveContext + 0x1580) != 0) ||
                ((self->actor.params & 0xFF00) && !FUN_003318bc(play, (self->actor.params & 0xFF00) >> 8, 4))) {
                FUN_0010fa78(self, play);
                self->actionFunc = FUN_0010fa78;
                self->dimmingIntensity = 0.0f;
                self->chargeAlpha = 0;
                self->spinAttackTimer = 0.0f;
                return;
            }

            self->isUsingMagic = true;
        }
    }

    if (player->stateFlags2 & 0x20000) {
        if ((child != NULL) && (child->update != NULL)) {
            child->parent = NULL;
        }

        if (PLAYER_UNK_858(player) <= 0.15f) {
            if ((PLAYER_UNK_858(player) >= 0.1f) && (*((s8*)player + 0x2226) >= 0x18)) {
                Audio_PlaySoundGeneral(0x01000115, &player->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                       &DAT_0054ac24.unk_0);
                Audio_PlaySoundGeneral(0x0100010A, &player->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                       &DAT_0054ac24.unk_0);
            }
            Actor_Kill(&self->actor);
            return;
        } else {
            player->stateFlags2 &= ~0x20000;
            if (self->actor.params & 0xFF00) {
                *(u16*)((u8*)&gSaveContext + 0x1580) = 1;
            }
            if (PLAYER_UNK_858(player) < 0.85f) {
                self->collider.info.toucher.dmgFlags = EnMThunder_sDmgTable.spin[self->swordType];
                self->attackStrength = 1;
                if (self->swordType != 1) { self->targetScale = 4; } else { self->targetScale = 2; }
            } else {
                self->collider.info.toucher.dmgFlags = EnMThunder_sDmgTable.jump[self->swordType];
                self->attackStrength = 0;
                if (self->swordType == 1) { self->targetScale = 4; } else { self->targetScale = 8; }
            }

            EnMThunder_SetupAction(self, FUN_00128c20);
            self->followPlayerTimer = 8;
            Audio_PlaySoundGeneral(EnMThunder_sSfxIds[self->attackStrength], &player->actor.world.pos, 4,
                                   &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            self->spinAttackTimer = 1.0f;
            return;
        }
    }

    if (!(player->stateFlags1 & 0x1000)) {
        if (self->actor.child != NULL) {
            self->actor.child->parent = NULL;
        }
        Actor_Kill(&self->actor);
        return;
    }

    if (PLAYER_UNK_858(player) > 0.15f) {
        self->chargeAlpha = 255;
        if (self->actor.child == NULL) {
            Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0x101, self->actor.world.pos.x,
                               self->actor.world.pos.y, self->actor.world.pos.z, 0, self->actor.shape.rot.y, 0,
                               self->swordType + 2);
        }
        self->dimmingIntensity += ((((PLAYER_UNK_858(player) - 0.15f) * 1.5f) - self->dimmingIntensity) * 0.5f *
                                   (f32)R_UPDATE_RATE * (1.0f / 3.0f));
    } else if (PLAYER_UNK_858(player) > 0.1f) {
        s32 alpha = (s32)((PLAYER_UNK_858(player) - 0.1f) * 255.0f * 20.0f);
        self->chargeAlpha = alpha;
        self->spinAttackTimer = (PLAYER_UNK_858(player) - 0.1f) * 10.0f;
    } else {
        self->chargeAlpha = 0;
    }

    {
        f32 charge = PLAYER_UNK_858(player);

        if (charge > 0.85f) {
            FUN_00314988(&player->actor.world.pos, 2);
        } else if (charge > 0.15f) {
            FUN_00314988(&player->actor.world.pos, 1);
        } else if (charge > 0.1f) {
            FUN_00314988(&player->actor.world.pos, 0);
        }
    }

    if (Gameplay_InCsMode(play)) {
        Actor_Kill(&self->actor);
    }
}

// EnMThunder_Draw: draft, far (~190 words; prologue reg allocation r4/r5/r6/r7 + Vec3f temps differ; see STATUS.md)
#if 0
#define EnMThunder_FREER(self, i) ((SkeletonAnimationModel*)(self)->freers[i])

void EnMThunder_Draw(Actor* thisx, GameState* state) {
    EnMThunder* self = (EnMThunder*)thisx;
    Player* player = GET_PLAYER((PlayState*)state);
    MTX34 mtx;
    MTX34 mtx2;
    f32 spinChargeScale;
    s32 idx;
    s32 i;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    Matrix_Scale(&mtx, 1, 0.02f, 0.02f, 0.02f);
    idx = 1 & ~self->attackStrength;
    EnMThunder_FREER(self, idx + 1)->SetUnkAC(1);
    EnMThunder_FREER(self, idx + 1)->FUN_003721e0(&mtx);
    FUN_003720a4(player, &mtx2);

    switch (self->swordType) {
        case 0: {
            Vector3f vec(0.0f, 300.0f, -100.0f);
            Matrix_UpdatePosition(&mtx2, &mtx2, (Vec3f*)&vec);
            Matrix_Scale(&mtx2, 1, -1.2f, -1.0f, -0.7f);
            Matrix_RotateX(&mtx2, 1, 16384.0f);
            break;
        }
        case 1: {
            Vector3f vec(0.0f, 220.0f, 0.0f);
            Matrix_UpdatePosition(&mtx2, &mtx2, (Vec3f*)&vec);
            Matrix_Scale(&mtx2, 1, -0.7f, -0.6f, -0.4f);
            Matrix_RotateX(&mtx2, 1, 16384.0f);
            break;
        }
        case 2: {
            Vector3f vec(200.0f, 350.0f, 0.0f);
            Matrix_UpdatePosition(&mtx2, &mtx2, (Vec3f*)&vec);
            Matrix_Scale(&mtx2, 1, -1.8f, -1.4f, -0.7f);
            Matrix_RotateX(&mtx2, 1, 16384.0f);
            break;
        }
    }

    if (self->spinChargePercent >= 0.85f) {
        spinChargeScale = (EnMThunder_sSpinChargeScale[(*(u32*)((u8*)state + 0x5BF4)) & 7] * 6.0f) + 1.0f;
        if (self->unk_250 == 0) {
            EnMThunder_FREER(self, 0)->unk_0C->FUN_00372d94(self->unk_24C);
        }
        self->unk_250 = 1;
    } else {
        spinChargeScale = (EnMThunder_sSpinChargeScale[(*(u32*)((u8*)state + 0x5BF4)) & 7] * 2.0f) + 1.0f;
        if (self->unk_250 != 0) {
            EnMThunder_FREER(self, 0)->unk_0C->FUN_00372d94(self->unk_248);
        }
        self->unk_250 = 0;
    }
    Matrix_Scale(&mtx2, 1, 1.0f, spinChargeScale, spinChargeScale);
    EnMThunder_FREER(self, 0)->SetUnkAC(1);
    EnMThunder_FREER(self, 0)->FUN_003721e0(&mtx2);

    {
        f32 buf[4];
        void* mat = EnMThunder_FREER(self, idx + 1)->unk_10;

        for (i = 0; i < 2; i++) {
            FUN_00333abc(mat, i, buf);
            buf[3] = self->spinAttackAlpha;
            FUN_00333a38(mat, i, buf);
            *((u8*)(*(void**)((u8*)mat + 4)) + i * 292) = 1;
        }
    }
    {
        f32 buf[4];
        void* mat = EnMThunder_FREER(self, 0)->unk_10;

        FUN_00333abc(mat, 0, buf);
        buf[3] = (f32)self->chargeAlpha * (1.0f / 255.0f);
        FUN_00333a38(mat, 0, buf);
        *(u8*)(*(void**)((u8*)mat + 4)) = 1;
    }

    if (PauseContext_GetState() == 0) {
        EnMThunder_FREER(self, 0)->unk_0C->animSpeed = 2.0f;
        EnMThunder_FREER(self, 2)->unk_0C->animSpeed = EnMThunder_FREER(self, 1)->unk_0C->animSpeed = 1.0f;
    } else {
        EnMThunder_FREER(self, 2)->unk_0C->animSpeed = EnMThunder_FREER(self, 1)->unk_0C->animSpeed =
            EnMThunder_FREER(self, 0)->unk_0C->animSpeed = 0.0f;
    }
    EnMThunder_FREER(self, idx + 1)->FUN_00372170(0);
    EnMThunder_FREER(self, 0)->FUN_00372170(0);
}
#endif

GLOBAL_ASM("asm/EnMThunder_Draw.s")
}
