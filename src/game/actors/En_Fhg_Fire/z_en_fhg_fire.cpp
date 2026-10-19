extern "C" {
#include "z_en_fhg_fire.hpp"

#define FLAGS 0x00000030

void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);

void EnFhgFire_Init(Actor* thisx, PlayState* play);
void EnFhgFire_Destroy(Actor* thisx, PlayState* play);
void EnFhgFire_Update(Actor* thisx, PlayState* play);
void EnFhgFire_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Fhg_Fire.data.s")
GLOBAL_ASM("../src/game/actors/En_Fhg_Fire/data_syms.s")

extern ColliderCylinderInit EnFhgFire_sCylinderInit;

ActorInit En_Fhg_Fire_InitVars = {
    ACTOR_PLAYER,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_FHG,
    sizeof(EnFhgFire),
    (ActorFunc)EnFhgFire_Init,
    (ActorFunc)EnFhgFire_Destroy,
    (ActorFunc)EnFhgFire_Update,
    (ActorFunc)EnFhgFire_Draw,
};

void FUN_003a424c(PlayState* play, Actor* actor, s16 unk, u8 arg3, u8 arg4, f32 arg5, f32 arg6) {
    u8* player = (u8*)GET_PLAYER(play);

    *(u8*)(player + 0x2290) = arg4;
    *(u8*)(player + 0x2291) = arg3;
    *(f32*)(player + 0x2294) = arg5;
    *(s16*)(player + 0x2292) = unk;
    *(f32*)(player + 0x2298) = arg6;
}

void FUN_0032fa4c(PlayState* play, Actor* actor, Vec3f* pos, s32 arg3, s32 arg4); // EffectSsFhgFlash_SpawnShock

void FUN_003d4b98(EnFhgFire* self, PlayState* play) {
    u8* player = (u8*)GET_PLAYER(play);

    if (self->collider.base.atFlags & 2) {
        self->collider.base.atFlags &= ~2;
        Audio_PlayActorSound2(&self->actor, 0x01000318);
    }

    if (Rand_ZeroOne() < 0.5f) {
        Vec3f pos = self->actor.world.pos;

        pos.y -= 20.0f;
        FUN_0032fa4c(play, &self->actor, &pos, 200, 0);
    }

    Actor_MoveForward(&self->actor);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    if (*(player + 0x2488) == 0) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    }

    Actor_UpdateBgCheckInfo(play, &self->actor, 50.0f, 50.0f, 100.0f, 1);
    if (self->actor.bgCheckFlags & 8) {
        Actor_Kill(&self->actor);
    }
}

void FUN_00363a98(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 life, s32 type); // EffectSsFhgFlash_SpawnLightBall

void FUN_001a08a8(EnFhgFire* self, PlayState* play) {
    Actor* parent = self->actor.parent;
    s16 i;

    if ((self->work[FHGFIRE_VARIANCE_TIMER] % 2) != 0) {
        Actor_SetScale(&self->actor, 6.0f);
    } else {
        Actor_SetScale(&self->actor, 5.25f);
    }

    self->actor.world.pos = *(Vec3f*)((u8*)parent + 0x2B0);
    self->actor.shape.rot.z += (s16)(Rand_ZeroOne() * 0x4E20) + 0x4000;

    if (self->work[FHGFIRE_FIRE_MODE] == FHGFIRE_LIGHT_GREEN) {
        Vec3f ballPos;
        Vector3f ballVel(0.0f, 0.0f, 0.0f);
        Vector3f ballAccel(0.0f, 0.0f, 0.0f);
        for (i = 0; i < 2; i++) {
            ballPos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
            ballPos.y = Rand_CenteredFloat(20.0f) + self->actor.world.pos.y;
            ballPos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
            ballAccel.y = -0.08f;

            FUN_00363a98(play, &ballPos, (Vec3f*)&ballVel, (Vec3f*)&ballAccel, (s16)(Rand_ZeroOne() * 80.0f) + 150,
                         0 /* FHGFLASH_LIGHTBALL_GREEN */);
        }
    }

    if (self->work[FHGFIRE_TIMER] == 0) {
        Actor_Kill(&self->actor);
    }
}

// 3DS frame scaling of a random N64 frame count (Rand expression is evaluated in the test and in one arm).
#define FHGFIRE_TRAIL_FRAMES(n) ((n) > 0 ? (s32)(0.5f + ((n) * 3.0f) / 2.0f) : (s32)(((n) * 3.0f) / 2.0f - 0.5f))

void FUN_003ca984(EnFhgFire* self, PlayState* play) {
    self->actor.shape.rot.x += (s16)(Rand_ZeroOne() * 4000.0f) + 0x4000;

    switch (self->work[FHGFIRE_FIRE_MODE]) {
        case TRAIL_INIT:
            self->work[FHGFIRE_FIRE_MODE] = TRAIL_APPEAR;
            self->work[FHGFIRE_TIMER] = FHGFIRE_TRAIL_FRAMES((s16)(Rand_ZeroOne() * 7.0f) + 7);
            /* fallthrough */
        case TRAIL_APPEAR:
            Math_ApproachF(&self->fwork[FHGFIRE_SCALE], 1.7f, 1.0f, 0.34f);

            if (self->work[FHGFIRE_TIMER] == 0) {
                self->work[FHGFIRE_FIRE_MODE] = TRAIL_DISSIPATE;
                self->work[FHGFIRE_TIMER] = 15;
                self->actor.world.pos.z += Math_SinS(self->actor.shape.rot.y) * -200.0f * self->fwork[FHGFIRE_SCALE];
                self->actor.world.pos.x += Math_CosS(self->actor.shape.rot.y) * 200.0f * self->fwork[FHGFIRE_SCALE];
                self->actor.shape.rot.y += 0x8000;
            }
            break;
        case TRAIL_DISSIPATE:
            Math_ApproachZeroF(&self->fwork[FHGFIRE_SCALE], 1.0f, 0.34f);
            if (self->work[FHGFIRE_TIMER] == 0) {
                Actor_Kill(&self->actor);
            }
            break;
    }

    Actor_SetScale(&self->actor, self->fwork[FHGFIRE_SCALE]);
    if (self->actor.scale.x > 1.0f) {
        self->actor.scale.x = 1.0f;
    }
}

void FUN_003ddcc0(EnFhgFire* self, PlayState* play); // N64 EnFhgFire_LightningStrike

s16 FUN_00368fec(Camera* camera);              // Camera_GetInputDirYaw
void FUN_0036fca8(Actor* actor, PlayState* play, s32 quakeIndex, s32 rumbleIndex); // Actor_RequestQuakeAndRumble

// x/y setter with z = 0: gives the ROM's x = copy of the shared 0.0f, stores in x,y,z order.
static inline void EnFhgFire_SetVecXY(Vec3f* v, f32 x, f32 y) {
    v->x = x;
    v->y = y;
    v->z = 0.0f;
}

// N64 EnFhgFire_LightningStrike (STRIKE_INIT = 0, STRIKE_BURST = 10, STRIKE_TRAILS = 11)
void FUN_003ddcc0(EnFhgFire* self, PlayState* play) {
    Camera* mainCam = Gameplay_GetCamera(play, 0);
    s16 i;

    switch (self->work[FHGFIRE_FIRE_MODE]) {
        case 0:
            self->work[FHGFIRE_FIRE_MODE] = 10;
            self->work[FHGFIRE_TIMER] = 11;
            break;
        case 10:
            self->actor.shape.rot.y = FUN_00368fec(mainCam) + 0x8000 * (self->work[FHGFIRE_VARIANCE_TIMER] & 0xFF);
            Math_ApproachF(&self->fwork[FHGFIRE_SCALE], 1.0f, 1.0f, 0.2f);

            if (self->work[FHGFIRE_TIMER] == 0) {
                self->work[FHGFIRE_FIRE_MODE] = 11;
                self->actor.shape.rot.z += 0x8000;
                self->actor.world.pos.y -= 200.0f;
                self->work[FHGFIRE_TIMER] = 56;

                Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_FHG_FIRE, self->actor.world.pos.x,
                                   self->actor.world.pos.y, self->actor.world.pos.z, 500, 0, 0,
                                   FHGFIRE_LIGHTNING_BURST);
                {
                    Vec3f sp7C;
                    Vec3f sp70;
                    EnFhgFire_SetVecXY(&sp70, 0.0f, -1.0f);

                    for (i = 0; i < 35; i++) {
                        sp7C.x = Rand_CenteredFloat(30.f);
                        sp7C.y = Rand_ZeroFloat(5.0f) + 3.0f;
                        sp7C.z = Rand_CenteredFloat(30.f);
                        sp70.y = -0.2f;
                        FUN_00363a98(play, &self->actor.world.pos, &sp7C, &sp70,
                                     (s16)(Rand_ZeroOne() * 100.0f) + 240, 0 /* FHGFLASH_LIGHTBALL_GREEN */);
                    }
                }
                FUN_0036fca8(&self->actor, play, 4, 10);
            }

            break;
        case 11:
            self->actor.shape.rot.y = FUN_00368fec(mainCam) + (self->work[FHGFIRE_VARIANCE_TIMER] & 0xFF) * 0x8000;

            Math_ApproachF(&self->fwork[FHGFIRE_SCALE], 0.0f, 1.0f, 0.2f);
            if (self->work[FHGFIRE_TIMER] == 45) {
                s16 randY = (Rand_ZeroOne() < 0.5f) ? 0x1000 : 0;

                for (i = 0; i < 8; i++) {
                    Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_FHG_FIRE, self->actor.world.pos.x,
                                       self->actor.world.pos.y, self->actor.world.pos.z, 0, (i * 0x2000) + randY,
                                       0x4000, FHGFIRE_LIGHTNING_TRAIL + i);
                }

                for (i = 0; i < 8; i++) {
                    Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_FHG_FIRE, self->actor.world.pos.x,
                                       self->actor.world.pos.y, self->actor.world.pos.z, 0, (i * 0x2000) + randY, 0,
                                       FHGFIRE_LIGHTNING_SHOCK);
                }
            }

            if (self->work[FHGFIRE_TIMER] == 0) {
                Actor_Kill(&self->actor);
            }
    }

    Actor_SetScale(&self->actor, self->fwork[FHGFIRE_SCALE]);
}

extern u8 gCustomLensFlareOn;
extern Vec3f gCustomLensFlarePos;
extern s16 gLensFlareScale;
extern f32 gLensFlareColorIntensity;
extern s16 gLensFlareGlareStrength;

// N64 EnFhgFire_LightningBurst
void FUN_001096cc(EnFhgFire* self, PlayState* play) {
    u8* player = (u8*)GET_PLAYER(play);

    *((u8*)play + 0x3261) = 1; // envCtx.fillScreen
    self->actor.shape.rot.y += 0x1000;

    if (self->work[FHGFIRE_FX_TIMER] == 74) {
        *((u8*)play + 0x3237) = 1; // envCtx.lightSettingOverride
        *(u16*)((u8*)play + 0x3254) = 255; // envCtx.lightBlendRateOverride
    }
    if (self->work[FHGFIRE_FX_TIMER] == 47) {
        *((u8*)play + 0x3237) = 0;
        *(u16*)((u8*)play + 0x3254) = 20;
    }
    if (self->work[FHGFIRE_FX_TIMER] > 71) {
        *((u8*)play + 0x3262) = *((u8*)play + 0x3263) = *((u8*)play + 0x3264) = 255; // screenFillColor
        if ((self->work[FHGFIRE_TIMER] % 2) != 0) {
            *((u8*)play + 0x3265) = 70;
        } else {
            *((u8*)play + 0x3265) = 0;
        }
    } else {
        *((u8*)play + 0x3265) = 0;
    }

    if (self->work[FHGFIRE_TIMER] <= 30) {
        Math_ApproachZeroF(&self->fwork[FHGFIRE_ALPHA], 1.0f, 45.0f);
        Math_ApproachZeroF(&self->fwork[FHGFIRE_SCALE], 1.0f, 0.5f);
    } else {
        Math_ApproachF(&self->fwork[FHGFIRE_SCALE], self->fwork[FHGFIRE_BURST_SCALE], 0.5f, 3.0f);
    }

    Actor_SetScale(&self->actor, self->fwork[FHGFIRE_SCALE]);
    if (self->fwork[FHGFIRE_BURST_SCALE] > 3.0f) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        if (*(player + 0x2488) == 0) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        }
    }

    if (self->work[FHGFIRE_TIMER] == 0) {
        Actor_Kill(&self->actor);
        *((u8*)play + 0x3261) = 0;
    }

    if (self->lensFlareTimer != 0) {
        self->lensFlareTimer--;
        self->lensFlareOn = true;
        Math_ApproachF(&self->lensFlareScale, 40.0f, 0.3f, 10.0f);
    } else {
        Math_ApproachZeroF(&self->lensFlareScale, 1.0f, 5.0f);
        if (self->lensFlareScale == 0.0f) {
            self->lensFlareOn = false;
        }
    }

    gCustomLensFlareOn = self->lensFlareOn;
    gCustomLensFlarePos = self->actor.world.pos;
    gLensFlareScale = self->lensFlareScale;
    gLensFlareColorIntensity = 10.0f;
    gLensFlareGlareStrength = 0;
}

// N64 EnFhgFire_PhantomWarp
void FUN_00156754(EnFhgFire* self, PlayState* play) {
    u8* horse = (u8*)self->actor.parent;

    self->fwork[FHGFIRE_WARP_TEX_1_X] += 25.0f * self->fwork[FHGFIRE_WARP_TEX_SPEED];
    self->fwork[FHGFIRE_WARP_TEX_1_Y] -= 40.0f * self->fwork[FHGFIRE_WARP_TEX_SPEED];
    self->fwork[FHGFIRE_WARP_TEX_2_X] += 5.0f * self->fwork[FHGFIRE_WARP_TEX_SPEED];
    self->fwork[FHGFIRE_WARP_TEX_2_Y] -= 30.0f * self->fwork[FHGFIRE_WARP_TEX_SPEED];

    if (self->actor.params == FHGFIRE_WARP_DEATH) {
        if (self->work[FHGFIRE_TIMER] > 105) {
            Audio_PlayActorSound2(&self->actor, 0x01000197);
            Audio_PlayActorSound2(&self->actor, 0x010001AB);
        }

        if (self->work[FHGFIRE_TIMER] == 105) {
            Audio_PlayActorSound2(&self->actor, 0x01000196);
            Audio_PlayActorSound2(&self->actor, 0x010001AA);
        }
    }

    if (self->work[FHGFIRE_TIMER] > 75) {
        f32 scrollDirection = 1.0f;

        if (self->models[0] != NULL) {
            if (self->actor.params >= FHGFIRE_WARP_RETREAT) {
                if (self->warpAnimMode != 1) {
                    self->warpAnimMode = 1;
                    {
                        SkeletonAnimationModel_unk_0C* anim = ((SkeletonAnimationModel*)self->models[0])->unk_0C;
                        anim->FUN_00372d94(self->zarInfo->GetCMABByIndex(0));
                        anim->SetAnimType(1);
                        anim->SetAnimSpeed(2.0f);
                    }
                }
            } else {
                if (self->warpAnimMode != 2) {
                    self->warpAnimMode = 2;
                    {
                        SkeletonAnimationModel_unk_0C* anim = ((SkeletonAnimationModel*)self->models[0])->unk_0C;
                        anim->FUN_00372d94(self->zarInfo->GetCMABByIndex(1));
                        anim->SetAnimType(1);
                        anim->SetAnimSpeed(2.0f);
                    }
                }
            }
        }
        if (self->actor.params >= FHGFIRE_WARP_RETREAT) {
            scrollDirection = -1.0f;
        }
        Math_ApproachF(&self->fwork[FHGFIRE_WARP_TEX_SPEED], scrollDirection, 1.0f, 0.04f);
        Math_ApproachF(&self->fwork[FHGFIRE_WARP_ALPHA], 255.0f, 1.0f, 10.2f);
    } else if (self->work[FHGFIRE_TIMER] <= 38) {
        Math_ApproachZeroF(&self->fwork[FHGFIRE_WARP_TEX_SPEED], 1.0f, 0.04f);
        Math_ApproachZeroF(&self->fwork[FHGFIRE_WARP_ALPHA], 1.0f, 10.2f);
    }

    if ((self->work[FHGFIRE_TIMER] == 0) || ((self->actor.params == FHGFIRE_WARP_EMERGE) && (*(horse + 0x1A7) != 0))) {
        Actor_Kill(&self->actor);
    }
}

void FUN_0019fe84(EnFhgFire* self, PlayState* play);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);
void Lights_PointNoGlowSetInfo(void* info, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);

static inline void EnFhgFire_SetUpdate(EnFhgFire* self, EnFhgFireUpdateFunc updateFunc) {
    self->updateFunc = updateFunc;
}

void EnFhgFire_Init(Actor* thisx, PlayState* play) {
    EnFhgFire* self = (EnFhgFire*)thisx;
    u8* player = (u8*)GET_PLAYER(play);
    s32 modelId;
    void** modelSlot;
    s32 flag;

    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
    flag = (self->actor.params & 0xFF00) >> 8;
    self->actor.params &= 0xFF;
    if ((self->actor.params == FHGFIRE_LIGHTNING_SHOCK) || (self->actor.params == FHGFIRE_LIGHTNING_BURST) ||
        (self->actor.params == FHGFIRE_ENERGY_BALL)) {
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, &self->actor, &EnFhgFire_sCylinderInit);
    }
    self->fwork[FHGFIRE_ALPHA] = 200.0f;
    Actor_SetScale(&self->actor, 0.0f);
    modelId = -1;
    modelSlot = NULL;

    if (self->actor.params == FHGFIRE_LIGHTNING_STRIKE) {
        EnFhgFire_SetUpdate(self, FUN_003ddcc0);
        Audio_PlayActorSound2(&self->actor, 0x01000312);
    } else if (self->actor.params >= FHGFIRE_LIGHTNING_TRAIL) {
        EnFhgFire_SetUpdate(self, FUN_003ca984);
        self->actor.shape.rot.x = self->actor.world.rot.x;
        self->actor.shape.rot.y = self->actor.world.rot.y;
        self->actor.shape.rot.z = self->actor.world.rot.z;
    }
    switch (self->actor.params) {
        case FHGFIRE_SPEAR_LIGHT:
            EnFhgFire_SetUpdate(self, FUN_001a08a8);
            self->work[FHGFIRE_TIMER] = thisx->world.rot.x;
            self->work[FHGFIRE_FIRE_MODE] = thisx->world.rot.y;
            modelId = 5;
            modelSlot = &self->models[2];
            break;
        case FHGFIRE_ENERGY_BALL: {
            f32 dxL;
            f32 dyL;
            f32 dzL;
            f32 dxzL;

            if (self->actor.world.rot.x != 0) {
                thisx->speedXZ = 3.0f;
            } else {
                thisx->speedXZ = AREG(0) + 6.0f;
            }
            EnFhgFire_SetUpdate(self, FUN_0019fe84);

            self->work[FHGFIRE_TIMER] = 105;
            self->work[FHGFIRE_FX_TIMER] = 3;

            dxL = *(f32*)(player + 0x28) - self->actor.world.pos.x;
            dyL = *(f32*)(player + 0x2C) + 30.0f - self->actor.world.pos.y;
            dzL = *(f32*)(player + 0x30) - thisx->world.pos.z;
            self->actor.world.rot.y = (s16)(Math_FAtan2F(dxL, dzL) * (32768.0f / 3.1415927f));
            dxzL = __sqrtf(dxL * dxL + dzL * dzL);
            self->actor.world.rot.x = (s16)(Math_FAtan2F(dyL, dxzL) * (32768.0f / 3.1415927f));
            self->collider.dim.radius = 40.0f;
            self->collider.dim.height = 50.0f;
            self->collider.dim.yShift = -25.0f;
            self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->lightInfo);
            Lights_PointNoGlowSetInfo(&self->lightInfo, (f32)(s16)self->actor.world.pos.x,
                                      (f32)(s16)self->actor.world.pos.y, (f32)(s16)self->actor.world.pos.z, 255, 255,
                                      255, 255, 0);
            modelId = 5;
            modelSlot = &self->models[2];
            break;
        }
        case FHGFIRE_LIGHTNING_SHOCK:
            self->actor.draw = NULL;
            EnFhgFire_SetUpdate(self, FUN_003d4b98);
            self->actor.speedXZ = 30.0f;
            Audio_PlayActorSound2(&self->actor, 0x01000313);
            break;
        case FHGFIRE_LIGHTNING_BURST:
            EnFhgFire_SetUpdate(self, FUN_001096cc);
            self->work[FHGFIRE_TIMER] = 48;
            self->work[FHGFIRE_FX_TIMER] = 75;
            ((EnFhgFire*)thisx)->lensFlareTimer = 15;
            self->fwork[FHGFIRE_ALPHA] = 255.0f;

            self->fwork[FHGFIRE_BURST_SCALE] = self->actor.world.rot.x / 100.0f;
            self->collider.dim.radius = self->actor.world.rot.x * 0.13f;
            self->collider.dim.height = self->actor.world.rot.x * 0.13f;
            self->collider.dim.yShift = 0;
            modelId = 2;
            modelSlot = &self->models[1];
            break;
        case FHGFIRE_WARP_EMERGE:
        case FHGFIRE_WARP_RETREAT:
        case FHGFIRE_WARP_DEATH:
            Actor_SetScale(&self->actor, 7.0f);
            EnFhgFire_SetUpdate(self, FUN_00156754);
            if (self->actor.params == FHGFIRE_WARP_DEATH) {
                self->work[FHGFIRE_TIMER] = 660;
                self->actor.scale.z = 1.0f;
            } else {
                self->work[FHGFIRE_TIMER] = 114;
                if (flag == 0) {
                    Audio_PlayActorSound2(&self->actor, 0x01000196);
                    Audio_PlayActorSound2(&self->actor, 0x010001AA);
                }
            }
            modelId = 1;
            modelSlot = &self->models[0];
            break;
        default:
            modelId = 3;
            modelSlot = &self->models[3];
            break;
    }

    self->warpAnimMode = 0;
    if (modelId != -1) {
        self->zarInfo = FUN_00372f38(&self->actor, play, modelSlot, modelId, 0);
        if (self->models[3] != NULL) {
            SkeletonAnimationModel_unk_0C* anim = ((SkeletonAnimationModel*)self->models[3])->unk_0C;

            anim->FUN_00372d94(self->zarInfo->GetCMABByIndex(2));
            anim->SetAnimType(1);
            anim->SetAnimSpeed(2.0f);
        }
    } else {
        self->zarInfo = FUN_00372f38(&self->actor, play, 0);
    }
}

void EnFhgFire_Destroy(Actor* thisx, PlayState* play) {
    EnFhgFire* self = (EnFhgFire*)thisx;

    if ((self->actor.params == FHGFIRE_LIGHTNING_SHOCK) || (self->actor.params == FHGFIRE_LIGHTNING_BURST) ||
        (self->actor.params == FHGFIRE_ENERGY_BALL)) {
        Collider_DestroyCylinder(play, &self->collider);
    }

    if (self->actor.params == FHGFIRE_ENERGY_BALL) {
        LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);
    }

    FUN_00350f34(thisx, &self->models[0], &self->models[1], &self->models[2], &self->models[3], 0);
}

void EnFhgFire_Update(Actor* thisx, PlayState* play) {
    EnFhgFire* self = (EnFhgFire*)thisx;

    self->work[FHGFIRE_VARIANCE_TIMER]++;

    if (self->work[FHGFIRE_TIMER] != 0) {
        self->work[FHGFIRE_TIMER]--;
    }
    if (self->work[FHGFIRE_FX_TIMER] != 0) {
        self->work[FHGFIRE_FX_TIMER]--;
    }

    self->updateFunc(self, play);
}

// FAR (186 of ~650 insns differ after ~10 variants + greedy alias search): N64 EnergyBall port, size 2572 vs ROM 2596. VFP/GPR numbering
// of dxL/dyL/dzL/dxPG/dyPG/dzPG, Vec3f temps and the sfx pos args all differ. Draft kept for a later pass.
#if 0
typedef struct EnFhgFirePlayer {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x1710 - 0x1A4];
    /* 0x1710 */ u32 stateFlags1;
    /* 0x1714 */ char unk_1714[0x2226 - 0x1714];
    /* 0x2226 */ s8 meleeWeaponAnimation;
} EnFhgFirePlayer;

typedef struct EnFhgFireBoss {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x26F - 0x1A4];
    /* 0x026F */ u8 returnCount;
    /* 0x0270 */ char unk_270;
    /* 0x0271 */ u8 flyMode;
    /* 0x0272 */ u8 returnSuccess;
    /* 0x0273 */ char unk_273[0x2BC - 0x273];
    /* 0x02BC */ Vec3f targetPos;
} EnFhgFireBoss;

// N64 EnFhgFire_EnergyBall
void FUN_0019fe84(EnFhgFire* self, PlayState* play) {
    f32 dxL;
    f32 dyL;
    f32 dzL;
    f32 dxzL;
    f32 dxPG;
    f32 dyPG;
    f32 dzPG;
    u8 killMode = 0;
    u8 canBottleReflect1;
    EnFhgFirePlayer* player = (EnFhgFirePlayer*)GET_PLAYER(play);

    if (self->work[FHGFIRE_KILL_TIMER] != 0) {
        self->work[FHGFIRE_KILL_TIMER]--;
        if (self->work[FHGFIRE_KILL_TIMER] == 0) {
            Actor_Kill(&self->actor);
            return;
        }
    } else {
        s32 canBottleReflect2;
        EnFhgFireBoss* bossGnd = (EnFhgFireBoss*)self->actor.parent;

        dxPG = bossGnd->targetPos.x - self->actor.world.pos.x;
        dyPG = bossGnd->targetPos.y - self->actor.world.pos.y;
        dzPG = bossGnd->targetPos.z - self->actor.world.pos.z;
        dxL = player->actor.world.pos.x - self->actor.world.pos.x;
        dyL = player->actor.world.pos.y + 40.0f - self->actor.world.pos.y;
        dzL = player->actor.world.pos.z - self->actor.world.pos.z;
        FUN_00365860(&self->actor);
        FUN_0036b96c(&self->actor);
        if (self->work[FHGFIRE_VARIANCE_TIMER] & 1) {
            Actor_SetScale(&self->actor, 6.0f);
        } else {
            Actor_SetScale(&self->actor, 5.25f);
        }
        self->actor.shape.rot.z += (s16)(Rand_ZeroOne() * 0x4E20) + 0x4000;
        if (1) {
            u8 lightBallColor1 = 0;
            s16 i1;
            Vec3f spD4;
            Vector3f spC8(0.0f, 0.0f, 0.0f);
            Vector3f spBC(0.0f, 0.0f, 0.0f);

            if (self->work[FHGFIRE_FIRE_MODE] >= FHGFIRE_LIGHT_BLUE) {
                lightBallColor1 = 1;
            }
            for (i1 = 0; i1 < 3; i1++) {
                spD4.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
                spD4.y = Rand_CenteredFloat(20.0f) + self->actor.world.pos.y;
                spD4.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
                spBC.y = -0.08f;
                FUN_00363a98(play, &spD4, (Vec3f*)&spC8, (Vec3f*)&spBC, (s16)(Rand_ZeroOne() * 80.0f) + 150, lightBallColor1);
            }
        }
        switch (self->work[FHGFIRE_FIRE_MODE]) {
            case FHGFIRE_LIGHT_GREEN:
                canBottleReflect1 =
                    ((player->stateFlags1 & 2) &&
                     (ABS((s16)(player->actor.shape.rot.y - (s16)(bossGnd->actor.yawTowardsPlayer + 0x8000))) < 0x2000) &&
                     (__sqrtf(SQ(dxL) + SQ(dyL) + SQ(dzL)) <= 25.0f))
                        ? 1
                        : 0;
                if ((self->collider.base.acFlags & 2) || canBottleReflect1) {
                    u32* acHitElem = *(u32**)((u8*)self + 0x24C);
                    s16 i2;
                    Vec3f spA8;
                    Vector3f sp9C(0.0f, -0.5f, 0.0f);
                    s16 angleModX;
                    s16 angleModY;

                    for (i2 = 0; i2 < 30; i2++) {
                        spA8.x = Rand_CenteredFloat(20.0f);
                        spA8.y = Rand_CenteredFloat(20.0f);
                        spA8.z = Rand_CenteredFloat(20.0f);
                        FUN_00363a98(play, &self->actor.world.pos, &spA8, (Vec3f*)&sp9C, (s16)(Rand_ZeroOne() * 25.0f) + 50, 0);
                    }
                    canBottleReflect2 = canBottleReflect1;
                    if (!canBottleReflect2 && (*acHitElem & 0x100000)) {
                        killMode = 2;
                        Audio_PlaySoundGeneral(0x010000FE, &player->actor.world.pos, 4, &FLOAT_0054ac20,
                                               &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                    } else {
                        if (bossGnd->flyMode == 1) {
                            angleModX = Rand_CenteredFloat(0x2000);
                            angleModY = Rand_CenteredFloat(0x2000);
                            self->actor.speedXZ = 15.0f;
                        } else {
                            angleModX = 0;
                            angleModY = 0;
                            self->work[FHGFIRE_RETURN_COUNT]++;
                            if ((self->work[FHGFIRE_RETURN_COUNT] > 3) && (Rand_ZeroOne() < 0.5f)) {
                                self->work[FHGFIRE_RETURN_COUNT] = 100;
                            }

                            if (!canBottleReflect2 && (player->meleeWeaponAnimation >= 0x18)) {
                                self->actor.speedXZ = 20.0f;
                                self->work[FHGFIRE_RETURN_COUNT] = 4;
                            } else {
                                self->actor.speedXZ += 1.0f;
                            }
                        }
                        self->actor.world.rot.y = (s16)(Math_FAtan2F(dxPG, dzPG) * (32768.0f / 3.1415927f)) + angleModY;
                        self->actor.world.rot.x =
                            (s16)(Math_FAtan2F(dyPG, __sqrtf((dxPG * dxPG) + (dzPG * dzPG))) * (32768.0f / 3.1415927f)) +
                            angleModX;
                        self->work[FHGFIRE_FIRE_MODE] = FHGFIRE_LIGHT_BLUE;
                        self->work[FHGFIRE_FX_TIMER] = 3;
                        Audio_PlaySoundGeneral(0x0100011C, &player->actor.world.pos, 4, &FLOAT_0054ac20,
                                               &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                    }
                    break;
                }
                if (__sqrtf(SQ(dxL) + SQ(dyL) + SQ(dzL)) <= 25.0f) {
                    killMode = 1;
                    Audio_PlayActorSound2(&self->actor, 0x01000318);
                    if ((bossGnd->flyMode > 1) && (self->work[FHGFIRE_RETURN_COUNT] > 1)) {
                        Audio_PlayActorSound2(&self->actor, 0x01000320);
                    }
                    FUN_003a424c(play, &self->actor, self->actor.world.rot.y, 3, 0x10, 3.0f, 0.0f);
                }
                break;
            case FHGFIRE_LIGHT_BLUE:
                if ((bossGnd->flyMode == 3) && (self->work[FHGFIRE_RETURN_COUNT] < 100)) {
                    self->actor.world.rot.y = (s16)(Math_FAtan2F(dxPG, dzPG) * (32768.0f / 3.1415927f));
                    if ((__sqrtf(SQ(dxPG) + SQ(dzPG)) < (8.0f + (self->actor.speedXZ * 3.0f)))) {
                        self->work[FHGFIRE_FIRE_MODE] = FHGFIRE_LIGHT_REFLECT;
                        bossGnd->returnSuccess = 1;
                        self->work[FHGFIRE_TIMER] = 12;
                    }
                } else {
                    if (self->work[FHGFIRE_RETURN_COUNT] >= 100) {
                        if ((__sqrtf(SQ(dxPG) + SQ(dyPG) + SQ(dzPG)) < 100.0f)) {
                            bossGnd->returnSuccess = 1;
                        }
                        self->actor.world.rot.y = (s16)(Math_FAtan2F(dxPG, dzPG) * (32768.0f / 3.1415927f));
                        self->actor.world.rot.x =
                            (s16)(Math_FAtan2F(dyPG, __sqrtf(SQ(dxPG) + SQ(dzPG))) * (32768.0f / 3.1415927f));
                    }
                    if ((ABS(dxPG) < 30.0f) && (ABS(dzPG) < 30.0f) && (ABS(dyPG) < 45.0f)) {
                        killMode = 2;
                        bossGnd->returnCount = self->work[FHGFIRE_RETURN_COUNT] + 1;
                        Audio_PlaySoundGeneral(0x01000318, &bossGnd->actor.world.pos, 4, &FLOAT_0054ac20,
                                               &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                        Audio_PlaySoundGeneral(0x0100031E, &bossGnd->actor.world.pos, 4, &FLOAT_0054ac20,
                                               &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                    }
                }
                break;
            case FHGFIRE_LIGHT_REFLECT:
                if (self->work[FHGFIRE_TIMER] == 0) {
                    s16 i3;
                    Vec3f sp88;
                    Vector3f sp7C(0.0f, -0.5f, 0.0f);

                    for (i3 = 0; i3 < 30; i3++) {
                        sp88.x = Rand_CenteredFloat(20.0f);
                        sp88.y = Rand_CenteredFloat(20.0f);
                        sp88.z = Rand_CenteredFloat(20.0f);
                        FUN_00363a98(play, &self->actor.world.pos, &sp88, (Vec3f*)&sp7C, (s16)(Rand_ZeroOne() * 40.0f) + 80, 0);
                    }
                    self->actor.world.rot.y = (s16)(Math_FAtan2F(dxL, dzL) * (32768.0f / 3.1415927f));
                    dxzL = __sqrtf(SQ(dxL) + SQ(dzL));
                    self->actor.world.rot.x = (s16)(Math_FAtan2F(dyL, dxzL) * (32768.0f / 3.1415927f));
                    self->work[FHGFIRE_FIRE_MODE] = FHGFIRE_LIGHT_GREEN;
                    Audio_PlayActorSound2(&self->actor, 0x0100011C);
                    self->actor.speedXZ += 3.0f;
                }
                break;
        }

        if (self->work[FHGFIRE_FX_TIMER] == 0) {
            Actor_UpdateBgCheckInfo(play, &self->actor, 50.0f, 50.0f, 100.0f, 7);
            if ((self->actor.bgCheckFlags & 0x19) || killMode) {
                u8 lightBallColor2 = 0;
                s16 i4;
                Vec3f sp6C;
                Vector3f sp60(0.0f, -1.0f, 0.0f);

                if (self->work[FHGFIRE_FIRE_MODE] > FHGFIRE_LIGHT_GREEN) {
                    lightBallColor2 = 1;
                }
                for (i4 = 0; i4 < 30; i4++) {
                    sp6C.x = Rand_CenteredFloat(20.0f);
                    sp6C.y = Rand_CenteredFloat(20.0f);
                    sp6C.z = Rand_CenteredFloat(20.0f);
                    sp60.y = -0.1f;
                    FUN_00363a98(play, &self->actor.world.pos, &sp6C, (Vec3f*)&sp60, (s16)(Rand_ZeroOne() * 100.0f) + 100,
                                 lightBallColor2);
                }
                if (killMode == 1) {
                    Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_FHG_FIRE, self->actor.world.pos.x,
                                       player->actor.world.pos.y + 20.0f, self->actor.world.pos.z, 0xC8, 0, 0,
                                       FHGFIRE_LIGHTNING_BURST);
                }
                bossGnd->flyMode = 1;
                self->work[FHGFIRE_KILL_TIMER] = 45;
                self->actor.draw = NULL;
                if (killMode == 0) {
                    Audio_PlayActorSound2(&self->actor, 0x0100031D);
                }
                return;
            } else {
                Collider_UpdateCylinder(&self->actor, &self->collider);
                CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
            }
        }
        Lights_PointNoGlowSetInfo(&self->lightInfo, (f32)(s16)self->actor.world.pos.x, (f32)(s16)self->actor.world.pos.y,
                                  (f32)(s16)self->actor.world.pos.z, 255, 255, 255, 200, 0);
        if (self->actor.speedXZ > 20.0f) {
            self->actor.speedXZ = 20.0f;
        }
        Audio_PlayActorSound2(&self->actor, 0x01000317);
    }
}
#endif

// NEAR 13 words (~45 variants): the else-branch Matrix_UpdatePosition(0,-100,0) vector: ROM x=s2(copy of s1=0.0), y=s3(-100), z=s1 stored x,y,z;
// helper variants give the right store order but different VFP numbering, and the following FUN_003695cc colour args are copied
// from s0 in ROM (vmov s2,s0; vmov s1,s0) vs straight from s16 here. Other three branches match.
#if 0
typedef struct EnFhgFireDrawColor {
    f32 r, g, b, a;
} EnFhgFireDrawColor;

void FUN_00358778(SkeletonAnimationModel* skelAnimModel, s32 arg1, s32 arg2, EnFhgFireDrawColor* color, s32 arg4);
void FUN_00371fac(MTX34* mtx, void* arg1);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);

struct EnFhgFireVec {
    f32& operator[](s32 i) {
        return (&x)[i];
    }
    f32 x, y, z;
};

static inline void EnFhgFire_Translate(MTX34* m, f32 a, f32 b, f32 c) {
    EnFhgFireVec p;

    p[1] = a;
    p[2] = b;
    p[0] = c;
    Matrix_UpdatePosition(m, m, (Vec3f*)&p);
}

void EnFhgFire_Draw(Actor* thisx, PlayState* play) {
    EnFhgFire* self = (EnFhgFire*)thisx;

    if (self->actor.params == FHGFIRE_LIGHTNING_BURST) {
        FUN_003695cc((SkeletonAnimationModel*)self->models[1], 0, 4, 2, 0.0f, 0.0f, 0.0f,
                     self->fwork[FHGFIRE_ALPHA] * (1.0f / 255.0f));
        ((SkeletonAnimationModel*)self->models[1])->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->models[1])->FUN_003721e0(&self->actor.modelMtx);
        ((SkeletonAnimationModel*)self->models[1])->FUN_00372170(0);
    } else if ((self->actor.params == FHGFIRE_SPEAR_LIGHT) || (self->actor.params == FHGFIRE_ENERGY_BALL)) {
        EnFhgFireDrawColor color;
        f32 angle;

        FUN_00371fac(&self->actor.modelMtx, (u8*)play + 0x2FC);
        if (self->work[FHGFIRE_FIRE_MODE] >= 1) {
            color.r = 0.0f;
            color.g = 1.0f;
            color.b = 1.0f;
        } else {
            color.r = 0.66f;
            color.g = 1.0f;
            color.b = 0.36f;
        }
        color.a = self->fwork[FHGFIRE_ALPHA] * (1.0f / 255.0f);
        FUN_00358778((SkeletonAnimationModel*)self->models[2], 0, 0, &color, 1);
        FUN_00358778((SkeletonAnimationModel*)self->models[2], 0, 4, &color, 2);

        angle = self->actor.shape.rot.z * (1.0f / 32768.0f) * 3.1416f;
        if (angle != 0.0f) {
            f32 sn = __hardfp_sinf(angle);
            f32 cs = __hardfp_cosf(angle);
            s32 i;

            for (i = 0; i < 3; i++) {
                f32 a = self->actor.modelMtx.data[i][0];
                f32 b = self->actor.modelMtx.data[i][1];

                self->actor.modelMtx.data[i][0] = a * cs + b * sn;
                self->actor.modelMtx.data[i][1] = b * cs - a * sn;
            }
        }
        ((SkeletonAnimationModel*)self->models[2])->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->models[2])->FUN_003721e0(&self->actor.modelMtx);
        ((SkeletonAnimationModel*)self->models[2])->FUN_00372170(0);
    } else if ((self->actor.params == FHGFIRE_WARP_EMERGE) || (self->actor.params == FHGFIRE_WARP_RETREAT) ||
               (self->actor.params == FHGFIRE_WARP_DEATH)) {
        FUN_003695cc((SkeletonAnimationModel*)self->models[0], 0, 4, 2, 0.0f, 0.0f, 0.0f,
                     ((EnFhgFire*)thisx)->fwork[FHGFIRE_WARP_ALPHA] * (1.0f / 255.0f));
        ((SkeletonAnimationModel*)self->models[0])->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->models[0])->FUN_003721e0(&self->actor.modelMtx);
        ((SkeletonAnimationModel*)self->models[0])->FUN_00372170(0);
    } else {
        EnFhgFire_Translate(&thisx->modelMtx, -100.0f, 0.0f, 0.0f);
        FUN_003695cc((SkeletonAnimationModel*)self->models[3], 0, 4, 2, 0.0f, 0.0f, 0.0f,
                     self->fwork[FHGFIRE_ALPHA] * (1.0f / 255.0f));
        ((SkeletonAnimationModel*)self->models[3])->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->models[3])->FUN_003721e0(&self->actor.modelMtx);
        ((SkeletonAnimationModel*)self->models[3])->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/EnFhgFire_Draw.s")
}
