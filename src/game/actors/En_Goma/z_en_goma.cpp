extern "C" {
#include "z_en_goma.hpp"

#define FLAGS 0x00000035

void EnGoma_Init(Actor* actor, GameState* state);
void EnGoma_Destroy(Actor* thisx, PlayState* play);
void EnGoma_Update(Actor* actor, GameState* state);
void EnGoma_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Goma.data.s")

GLOBAL_ASM("../src/game/actors/En_Goma/data_syms.s")

extern EnGomaData gEnGomaData; // 0x00525E58 (EQU in data_syms.s)

void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 startFrame, f32 endFrame, s32 mode,
                               f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
s16 Actor_WorldYawTowardActor(Actor* actor, Actor* target);
void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);

void FUN_003e44e0(EnGoma* self, PlayState* play); // Stand
void FUN_003e9a84(EnGoma* self, PlayState* play); // ChasePlayer
void FUN_003d364c(EnGoma* self, PlayState* play); // Flee
void FUN_003d4b00(EnGoma* self, PlayState* play); // Hatch
void FUN_003c6ad4(EnGoma* self, PlayState* play); // EggFallToGround
void EffectSsHahen_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 flags, s16 scale, s16 objectSlot,
                         s16 params, void* dList);
void FUN_001c60e0(EnGoma* self, PlayState* play); // Land
void FUN_00196804(EnGoma* self, PlayState* play); // Dead
void FUN_001796b8(EnGoma* self, PlayState* play); // PrepareJump
void FUN_003d6e2c(EnGoma* self, PlayState* play); // Die
void FUN_003d364c(EnGoma* self, PlayState* play); // Flee
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void FUN_001807e4(EnGoma* self);                  // SetupStand

ActorInit En_Goma_InitVars = {
    ACTOR_BOSS_GOMA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GOL,
    sizeof(EnGoma),
    (ActorFunc)EnGoma_Init,
    (ActorFunc)EnGoma_Destroy,
    (ActorFunc)EnGoma_Update,
    (ActorFunc)EnGoma_Draw,
};

// EnGoma_Init: ~25 words off (register allocation: ROM keeps play in r7 / zero in r8; ours r6 / sl). Fixed: actor->speedXZ single-site.
// Needs EQUs gEnGoma_ColInit1/2, gEnGoma_sInitChain (data_syms.s) and model_manager.hpp.
#if 0
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void FUN_00358ea8(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, void* anim, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void Animation_PlayLoop(SkelAnime* skelAnime, void* anim);
void FUN_00175674(EnGoma* self, PlayState* play); // Debris
extern InitChainEntry gEnGoma_sInitChain[];
extern ColliderCylinderInit gEnGoma_ColInit1;
extern ColliderCylinderInit gEnGoma_ColInit2;

void EnGoma_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnGoma* self = (EnGoma*)actor;
    ZARInfo* zar;
    void* cmb;

    self->bossLimbDL = NULL;
    self->model = NULL;
    self->eggTimer = Rand_ZeroOne() * 200.0f;
    Actor_ProcessInitChain(&self->actor, gEnGoma_sInitChain);
    Actor_SetScale(&self->actor, 0.01f);

    if (self->actor.params < 100) {
        if (self->actor.params >= 10) {
            self->actor.gravity = -1.3f;
            self->actor.flags &= ~1;
            self->eggScale = 1.0f;
            self->actionTimer = 75;
            self->gomaType = 2;
            self->actor.velocity.y = Rand_ZeroOne() * 5.0f + 5.0f;
            self->actionFunc = FUN_00175674;
            self->actor.speedXZ = Rand_ZeroOne() * 2.3f + 1.5f;
            self->actionTimer = 45;
            self->actor.scale.x = Rand_ZeroOne() * 0.005f + 0.01f;
            self->actor.scale.y = Rand_ZeroOne() * 0.005f + 0.01f;
            self->actor.scale.z = Rand_ZeroOne() * 0.005f + 0.01f;
            ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
            self->actor.objBankIndex = Object_GetIndex(&play->objectCtx, 3);
            zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
            cmb = ZAR_GetCMBByIndex(zar, 0xE);
            {
                ModelAllocator* alloc = GetModelManager().allocator;

                alloc->owner = self->actor.unk_178;
                self->model = (EnGomaModel*)alloc->Create(cmb, 1);
                alloc->owner = NULL;
            }
        } else {
            ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);
            self->actor.objBankIndex = Object_GetIndex(&play->objectCtx, 0x1E);
            zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
            cmb = ZAR_GetCMBByIndex(zar, 0);
            *(void**)&self->unk_814 = cmb;
            FUN_00358ea8(zar, play, &self->skelanime, cmb, self->actor.unk_178, gEnGomaData.anim[0], self->unk_228,
                         (char*)self + 0x498, 12);
            Animation_PlayLoop(&self->skelanime, gEnGomaData.anim[0]);
            self->actor.colChkInfo.health = 2;
            if (self->actor.params < 3) {
                self->actionFunc = FUN_003c6ad4;
                actor->speedXZ = 1.5f;
                self->invincibilityTimer = 10;
            } else if (self->actor.params == 8 || self->actor.params == 6) {
                self->actionFunc = FUN_001551ac;
                self->spawnNum = gEnGomaData.spawnNum++;
            } else if (self->actor.params == 9 || self->actor.params == 7) {
                self->actionFunc = FUN_001551ac;
            }
            self->eggYOffset = (self->actor.params >= 8) ? -1500.0f : 1500.0f;
            self->gomaType = 1;
            self->eggScale = 1.0f;
            self->eggSquishAngle = Rand_ZeroOne() * 1000.0f;
            self->actionTimer = 75;
            Collider_InitCylinder(play, &self->colliderCylinder1);
            Collider_SetCylinder(play, &self->colliderCylinder1, &self->actor, &gEnGoma_ColInit1);
            Collider_InitCylinder(play, &self->colliderCylinder2);
            Collider_SetCylinder(play, &self->colliderCylinder2, &self->actor, &gEnGoma_ColInit2);
        }
    }
    self->actor.unk_19B = 4;
}
#endif

GLOBAL_ASM("asm/EnGoma_Init.s")

void EnGoma_Destroy(Actor* thisx, PlayState* play) {
    EnGoma* self = (EnGoma*)thisx;

    if (self->model != NULL) {
        self->model->Destroy();
        self->model = NULL;
    }
    SkelAnime_Free2(&self->skelanime);
    if (self->actor.params < 10) {
        Collider_DestroyCylinder(play, &self->colliderCylinder1);
        Collider_DestroyCylinder(play, &self->colliderCylinder2);
    }
}

void FUN_001c60e0(EnGoma* self, PlayState* play) {
    FUN_003731e0(&self->skelanime);
    if (self->actor.bgCheckFlags & 1) {
        Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 2.0f);
    }
    if (self->actionTimer == 0) {
        FUN_001807e4(self);
    }
}

s32 FUN_00271db4(PlayState* play, s32 limbIndex, MTX34* mtx, EnGoma* self) {
    if (limbIndex == 9) {
        f32 scale = 0.000095873802f;

        Matrix_RotateX(mtx, 1, self->eyePitch * scale);
        Matrix_RotateY(mtx, 1, self->eyeYaw * scale);
    }
    return 0;
}

void FUN_003e44e0(EnGoma* self, PlayState* play) {
    FUN_003731e0(&self->skelanime);
    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);
    FUN_00370084(&self->actor.shape.rot.y, Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor), 2, 3000);

    if (self->actionTimer == 0) {
        f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[1]);

        Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[1], 1.0f, 0.0f, lastFrame, 0, -5.0f);
        self->actionFunc = FUN_003e9a84;
        self->actionTimer = Rand_S16Offset_003702c8(70, 110);
    }
}

void FUN_0010623c(EnGoma* self, PlayState* play) {
    self->actor.flags |= 0x1000000;
    FUN_003731e0(&self->skelanime);
    Math_ApproachF(&self->actor.speedXZ, 10.0f, 0.5f, 5.0f);

    if (self->actor.velocity.y <= 0.0f && (self->actor.bgCheckFlags & 1)) {
        f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[4]);

        Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[4], 1.0f, 0.0f, lastFrame, 2, 0.0f);
        self->actionFunc = FUN_001c60e0;
        self->actionTimer = 15;
        if (self->actor.params < 6) {
            Audio_PlayActorSound2(&self->actor, 0x01000454);
        } else {
            Audio_PlayActorSound2(&self->actor, 0x01000456);
        }
    }
    self->visualState = 0;
}

void FUN_003d4b00(EnGoma* self, PlayState* play) {
    FUN_003731e0(&self->skelanime);
    if (self->actionTimer == 0) {
        f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[0]);

        self->actionTimer = Rand_S16Offset_003702c8(10, 30);
        Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[0], 1.0f, 0.0f, lastFrame, 0, -5.0f);
        self->actionFunc = FUN_003e44e0;
        self->gomaType = 0;
    }
}

void FUN_003d6e2c(EnGoma* self, PlayState* play) {
    FUN_003731e0(&self->skelanime);
    if (self->actor.bgCheckFlags & 1) {
        Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 2.0f);
    }
    if (self->actionTimer == 26) {
        if (self->actor.params < 6) {
            Audio_PlayActorSound2(&self->actor, 0x01000453);
        } else {
            Audio_PlayActorSound2(&self->actor, 0x01000455);
        }
    }
    if (self->actionTimer == 0) {
        f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[7]);

        Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[7], 1.0f, 0.0f, lastFrame, 0, -2.0f);
        self->actionFunc = FUN_00196804;
        self->actionTimer = 5;
    }
}

static inline void EnGoma_SetupPrepareJump(EnGoma* self) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[2]);

    Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[2], 1.0f, 0.0f, lastFrame, 2, -5.0f);
    self->actionFunc = FUN_001796b8;
    self->actionTimer = 45;
}

void FUN_003e9a84(EnGoma* self, PlayState* play) {
    FUN_003731e0(&self->skelanime);
    if (Animation_OnFrameImpl(&self->skelanime, 1.0f, 1.0f) || Animation_OnFrameImpl(&self->skelanime, 5.0f, 1.0f)) {
        if (self->actor.params < 6) {
            Audio_PlayActorSound2(&self->actor, 0x01000286);
        } else {
            Audio_PlayActorSound2(&self->actor, 0x010003CC);
        }
    }
    Math_ApproachF(&self->actor.speedXZ, 10.0f / 3.0f, 0.5f, 2.0f);
    FUN_00370084(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 3, 2000);
    FUN_00370084(&self->actor.shape.rot.y, self->actor.world.rot.y, 2, 3000);
    if (self->actor.bgCheckFlags & 1) {
        self->actor.velocity.y = 0.0f;
    }
    if (self->actor.xzDistToPlayer <= 150.0f) {
        EnGoma_SetupPrepareJump(self);
    }
}

void FUN_003d364c(EnGoma* self, PlayState* play) {
    FUN_003731e0(&self->skelanime);
    Math_ApproachF(&self->actor.speedXZ, 20.0f / 3.0f, 0.5f, 2.0f);
    FUN_00370084(&self->actor.world.rot.y, Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor) + 0x8000, 3,
                 2000);
    FUN_00370084(&self->actor.shape.rot.y, self->actor.world.rot.y, 2, 3000);

    if (self->actionTimer == 0) {
        f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[0]);

        self->actionTimer = Rand_S16Offset_003702c8(10, 30);
        Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[0], 1.0f, 0.0f, lastFrame, 0, -5.0f);
        self->actionFunc = FUN_003e44e0;
        self->gomaType = 0;
    }
}

void FUN_001796b8(EnGoma* self, PlayState* play) {
    s16 targetAngle;

    FUN_003731e0(&self->skelanime);
    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);
    targetAngle = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor);
    FUN_00370084(&self->actor.world.rot.y, targetAngle, 2, 4000);
    FUN_00370084(&self->actor.shape.rot.y, targetAngle, 2, 3000);

    if (self->actionTimer == 0) {
        f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[3]);

        Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[3], 1.0f, 0.0f, lastFrame, 2, 0.0f);
        self->actionFunc = FUN_0010623c;
        self->actor.velocity.y = 8.0f;
        if (self->actor.params < 6) {
            Audio_PlayActorSound2(&self->actor, 0x01000287);
        } else {
            Audio_PlayActorSound2(&self->actor, 0x010003CD);
        }
    }
    self->visualState = 0;
}

static Vec3f sDeadEffectVel = { 0.0f, 0.0f, 0.0f }; // 0x0059AF6C (z_en_goma.bss pin)

void FUN_00196804(EnGoma* self, PlayState* play) {
    FUN_003731e0(&self->skelanime);
    Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 2.0f);

    if (self->actionTimer == 3) {
        Vec3f accel;
        Vec3f pos;

        pos.x = self->actor.world.pos.x;
        pos.y = (self->actor.world.pos.y + 5.0f) - 10.0f;
        pos.z = self->actor.world.pos.z;
        accel = sDeadEffectVel;
        accel.y = 0.03f;
        EffectSsKFire_Spawn(play, &pos, &sDeadEffectVel, &accel, 40, 0);
    }

    if (self->actionTimer == 0 && Math_SmoothStepToF(&self->actor.scale.y, 0.0f, 0.5f, 0.00225f, 0.00001f) <= 0.005f) {
        if (self->actor.params < 6) {
            s16* children = (s16*)self->actor.parent;

            children[self->actor.params + 0x120] = -1;
        }
        Audio_PlaySoundGeneral(0x010002E8, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
        Actor_Kill(&self->actor);
        Item_DropCollectibleRandom(play, NULL, &self->actor.world.pos, 0x30);
    }
    self->visualState = 2;
}

// FUN_003ddad8 (EnGoma_Stunned): 5 words off, only VFP numbering of the 0.0f / lastFrame pair (s16/s17 swapped at 0x48-0x9c); ~17 variants tried.
#if 0
void FUN_003ddad8(EnGoma* self, PlayState* play) {
    Actor_SetColorFilter(&self->actor, 0, 180, 0, 2);
    self->visualState = 2;

    if (self->actionTimer != 0) {
        FUN_003731e0(&self->skelanime);
    }

    if (self->actor.bgCheckFlags & 1) {
        self->actor.velocity.y = 0.0f;
        Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);
    }

    if (self->stunTimer == 0) {
        f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[0]);

        self->actionTimer = Rand_S16Offset_003702c8(10, 30);
        Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[0], 1.0f, 0.0f, lastFrame, 0, -5.0f);
        self->actionFunc = FUN_003e44e0;
        self->gomaType = 0;
    } else if (--self->stunTimer < 45) {
        if (self->stunTimer & 1) {
            self->actor.world.pos.x += 1.5f;
            self->actor.world.pos.z += 1.5f;
        } else {
            self->actor.world.pos.x -= 1.5f;
            self->actor.world.pos.z -= 1.5f;
        }
    }
}
#endif

void FUN_00120948(EnGoma* self, PlayState* play) {
    FUN_003731e0(&self->skelanime);

    if (self->actor.bgCheckFlags & 1) {
        Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 2.0f);
    }

    if (self->actionTimer == 0) {
        if ((s8)self->actor.colChkInfo.health <= 0) {
            f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[6]);

            Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[6], 1.0f, 0.0f, lastFrame, 2, -2.0f);
            self->actionFunc = FUN_003d6e2c;
            self->actionTimer = 45;
            if (self->actor.params < 6) {
                Audio_PlayActorSound2(&self->actor, 0x0100028A);
            } else {
                Audio_PlayActorSound2(&self->actor, 0x010003D0);
            }
            self->invincibilityTimer = 100;
            self->actor.flags &= ~1;
        } else {
            f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[1]);

            Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[1], 2.0f, 0.0f, lastFrame, 0, -2.0f);
            self->actionFunc = FUN_003d364c;
            self->actionTimer = 30;
            if (self->actor.params < 6) {
                Audio_PlayActorSound2(&self->actor, 0x01000289);
            } else {
                Audio_PlayActorSound2(&self->actor, 0x010003CF);
            }
        }
    }
}

void FUN_001741c8(EnGoma* self, PlayState* play) {
    s16 i;
    f32 lastFrame = Animation_GetLastFrame(&self->skelanime, gEnGomaData.anim[3]);

    Animation_Change_00375c08(&self->skelanime, gEnGomaData.anim[3], 1.0f, 0.0f, lastFrame, 2, 0.0f);
    self->actionFunc = FUN_003d4b00;
    Actor_SetScale(&self->actor, 0.005f);
    self->gomaType = 0;
    self->actionTimer = 8;
    self->actor.shape.rot.y = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(play)->actor);
    self->actor.world.rot.y = self->actor.shape.rot.y;

    if (self->actor.params < 6) {
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 0x28, 0x01000285);
    } else {
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 0x28, 0x010003D2);
    }
    for (i = 0; i < 15; i++) {
        Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_GOMA,
                           Rand_CenteredFloat(10.0f) + self->actor.world.pos.x,
                           Rand_CenteredFloat(10.0f) + self->actor.world.pos.y + 15.0f,
                           Rand_CenteredFloat(10.0f) + self->actor.world.pos.z, 0,
                           Rand_CenteredFloat(0x10000 - 0.01f), 0, i + 10);
    }
    self->eggScale = 1.0f;
    self->actor.speedXZ = 0.0f;
}

extern __pure float _fabsf(float);
// FUN_001551ac (EnGoma_Egg): 18 words off; only the VFP numbering of the loop constants (30.0f / 5.0f / 0.0f / -0.5f) and vpush d8-d9 vs d8-d10 differ. ~12 variants tried (decl orders, Vector3f/Vec3f inits, locals).
#if 0
static __inline __pure float EnGoma_Fabsf(float f) {
    return _fabsf(f);
}

void FUN_001551ac(EnGoma* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 i;

    self->eggSquishAngle += 1.0f;
    Math_ApproachF(&self->eggSquishAmount, 0.1f, 1.0f, 0.005f);
    if (EnGoma_Fabsf(self->actor.world.pos.x - player->actor.world.pos.x) < 100.0f &&
        EnGoma_Fabsf(self->actor.world.pos.z - player->actor.world.pos.z) < 100.0f) {
        if (++self->playerDetectionTimer >= 10) {
            self->actionFunc = FUN_003c6ad4;
        }
    } else {
        self->playerDetectionTimer = 0;
    }

    if (!(self->eggTimer & 0xF) && Rand_ZeroOne() < 0.5f) {
        for (i = 0; i < 2; i++) {
            Vector3f vel(0.0f, 0.0f, 0.0f);
            Vector3f acc(0.0f, -0.5f, 0.0f);
            Vec3f pos;

            pos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
            pos.y = Rand_ZeroFloat(30.0f) + self->actor.world.pos.y;
            pos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;
            EffectSsHahen_Spawn(play, &pos, (Vec3f*)&vel, (Vec3f*)&acc, 0, (s16)(Rand_ZeroOne() * 5.0f) + 10, -1, 10, NULL);
        }
    }
}
#endif

void FUN_003c6ad4(EnGoma* self, PlayState* play) {
    self->actor.gravity = -1.3f;
    self->eggSquishAccel += 0.03f;
    self->eggSquishAngle += 1.0f + self->eggSquishAccel;
    Math_ApproachZeroF(&self->eggSquishAmount, 1.0f, 0.005f);
    Math_ApproachF(&self->eggYOffset, 1500.0f, 1.0f, 150.0f);

    switch (self->hatchState) {
        case 0:
            if (self->actor.bgCheckFlags & 1) {
                if (self->actor.params < 6) {
                    Audio_PlayActorSound2(&self->actor, 0x01000284);
                } else {
                    Audio_PlayActorSound2(&self->actor, 0x010003D1);
                }
                if (self->actor.params < 6) {
                    self->hatchState = 1;
                    self->actionTimer = 5;
                    Math_ApproachF(&self->eggScale, 1.5f, 0.5f, 1.0f);
                } else {
                    FUN_001741c8(self, play);
                }
            }
            break;
        case 1:
            if (self->actionTimer == 0) {
                self->hatchState = 2;
                self->actionTimer = 5;
                Math_ApproachF(&self->eggScale, 0.75f, 0.5f, 1.0f);
                self->actor.velocity.y = 5.0f;
                self->actor.speedXZ = 2.0f;
            } else {
                Math_ApproachF(&self->eggScale, 1.5f, 0.5f, 1.0f);
            }
            break;
        case 2:
            if (self->actionTimer == 0) {
                self->hatchState = 3;
                self->actionTimer = 120;
            } else {
                Math_ApproachF(&self->eggScale, 0.75f, 0.5f, 1.0f);
            }
            break;
        case 3:
            Math_ApproachF(&self->eggScale, 1.0f, 0.1f, 0.1f);
            if (self->actionTimer == 0) {
                FUN_001741c8(self, play);
            }
            break;
    }

    if (self->actor.bgCheckFlags & 1) {
        Math_ApproachZeroF(&self->actor.speedXZ, 0.2f, 0.05f);
    }
    self->eggPitch += self->actor.speedXZ * 0.1f;
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

GLOBAL_ASM("asm/EnGoma_Update.s")

// EnGoma_Draw: structurally identical (314 words off), only the stack layout differs (ROM: Vec3f at sp+4, MTX34 at sp+0x10, frame 0x40, r8=&mtx).
// Needs mtx34_trans.hpp, graphics.hpp.
#if 0
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_0033dd8c(SkelAnime* skelAnime, s32 arg1, s32 arg2, s32 arg3, f32 f0, f32 f1, f32 f2, f32 f3);
f32 sinf(f32 x);

static inline void EnGoma_ScaleMtx(MTX34* mtx, f32 x, f32 y, f32 z) {
    f32* m = (f32*)mtx;

    m[0] *= x;
    m[4] *= x;
    m[8] *= x;
    m[1] *= y;
    m[5] *= y;
    m[9] *= y;
    m[2] *= z;
    m[6] *= z;
    m[10] *= z;
}

static inline void EnGoma_TransNew(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    MTX34_MakeTrans(mtx, &t);
}

static inline void EnGoma_TransApply(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    Matrix_UpdatePosition(mtx, mtx, &t);
}

void EnGoma_Draw(Actor* actor, GameState* state) {
    EnGoma* self = (EnGoma*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;

    switch (self->gomaType) {
        case 0: {
            self->actor.naviEnemyId = 3;
            EnGoma_TransNew(&mtx, self->actor.world.pos.x,
                            self->actor.world.pos.y + play->mainCamera.skyboxOffset.y +
                                self->actor.shape.yOffset * self->actor.scale.y,
                            self->actor.world.pos.z);
            Matrix_RotateX(&mtx, 1, self->slopePitch * (1.0f / 32768.0f) * 3.1415927f);
            Matrix_RotateZ(&mtx, 1, self->slopeRoll * (1.0f / 32768.0f) * 3.1415927f);
            Matrix_RotateY(&mtx, 1, self->actor.shape.rot.y * (1.0f / 32768.0f) * 3.1415927f);
            Matrix_RotateX(&mtx, 1, self->actor.shape.rot.x * (1.0f / 32768.0f) * 3.1415927f);
            Matrix_RotateZ(&mtx, 1, self->actor.shape.rot.z * (1.0f / 32768.0f) * 3.1415927f);
            EnGoma_ScaleMtx(&mtx, self->actor.scale.x, self->actor.scale.y, self->actor.scale.z);
            FUN_0036932c(self->skelanime.unk_28, 3);
            FUN_0037266c(self->skelanime.unk_28, 1);
            FUN_0037266c(self->skelanime.unk_28, 2);
            FUN_0037266c(self->skelanime.unk_28, 0);
            FUN_0033dd8c(&self->skelanime, 0, 0, 0, self->eyeEnvColor[0] * (1.0f / 255.0f),
                         self->eyeEnvColor[1] * (1.0f / 255.0f), self->eyeEnvColor[2] * (1.0f / 255.0f), 1.0f);
            if (self->hurtTimer != 0) {
                f32 r0 = Rand_ZeroOne();
                f32 r1 = Rand_ZeroOne();
                f32 r2 = Rand_ZeroOne();

                FUN_0033dd8c(&self->skelanime, 1, 0, 0, r0, r1, r2, 1.0f);
            } else {
                FUN_0033dd8c(&self->skelanime, 1, 0, 0, self->eyeEnvColor[0] * (1.0f / 255.0f),
                             self->eyeEnvColor[1] * (1.0f / 255.0f), self->eyeEnvColor[2] * (1.0f / 255.0f), 1.0f);
            }
            SkelAnime_DrawOpa(&self->skelanime, &mtx, (OverrideLimbDrawOpa)FUN_00271db4, NULL, &self->actor, 0);
            break;
        }

        case 1: {
            self->actor.naviEnemyId = 2;
            sinf(self->eggTimer * 5.0f * 3.1415f / 180.0f);
            mtx = self->actor.modelMtx;
            EnGoma_ScaleMtx(&mtx, self->eggScale, 1.0f / self->eggScale, self->eggScale);
            Matrix_RotateY(&mtx, 1, self->eggSquishAngle * 0.15f);
            Matrix_RotateZ(&mtx, 1, self->eggSquishAngle * 0.1f);
            EnGoma_ScaleMtx(&mtx, 0.95f - self->eggSquishAmount, self->eggSquishAmount + 1.05f,
                            0.95f - self->eggSquishAmount);
            Matrix_RotateZ(&mtx, 1, -(self->eggSquishAngle * 0.1f));
            Matrix_RotateY(&mtx, 1, -(self->eggSquishAngle * 0.15f));
            EnGoma_TransApply(&mtx, 0.0f, self->eggYOffset, 0.0f);
            Matrix_RotateX(&mtx, 1, self->eggPitch);
            FUN_0037266c(self->skelanime.unk_28, 3);
            FUN_0036932c(self->skelanime.unk_28, 1);
            FUN_0036932c(self->skelanime.unk_28, 2);
            FUN_0036932c(self->skelanime.unk_28, 0);
            SkelAnime_DrawOpa(&self->skelanime, &mtx, NULL, NULL, &self->actor, 0);
            if (self->model != NULL) {
                ((SkeletonAnimationModel*)self->model)->unk_AD[0] = 0;
            }
            break;
        }

        case 2: {
            SkeletonAnimationModel* m = (SkeletonAnimationModel*)self->model;

            m->unk_AD[0] = 1;
            m->unk_AC = 1;
            m->FUN_003721e0(&self->actor.modelMtx);
            m->FUN_00372170(0);
            break;
        }
    }
}
#endif

GLOBAL_ASM("asm/EnGoma_Draw.s")
}
