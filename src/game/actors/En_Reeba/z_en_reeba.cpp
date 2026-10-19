extern "C" {
#include "z_en_reeba.hpp"

#define FLAGS 0x08000015

void EnReeba_Init(EnReeba* self, PlayState* play);
void EnReeba_Destroy(Actor* actor, GameState* state);
void EnReeba_Update(Actor* actor, GameState* state);
void EnReeba_Draw(Actor* actor, GameState* state);

void FUN_0021d4e8(EnReeba* self, PlayState* play);
void FUN_003d6118(EnReeba* self, PlayState* play);
void FUN_003d4cb0(EnReeba* self, PlayState* play);
void* FUN_003687a8(void* skelAnimeModel);
void FUN_003589cc(void* gfx, s32 arg);
void FUN_00358964(void* gfx, s32 arg, void* color);
void FUN_001747a4(EnReeba* self, PlayState* play);
void FUN_00181394(EnReeba* self, PlayState* play);
extern __pure float _fabsf(float);
ZARInfo* FUN_00372f38(Actor* actor, PlayState* play, ...);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vector3f* velocity, Vector3f* accel, s16 scale, s16 scaleStep, s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unk1, s16 unk2, s32 flag);
void FUN_003e8c78(EnReeba* self, PlayState* play);
void FUN_00375b70(PlayState* play, Actor* actor);
void EffectSsEnIce_SpawnFlyingVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, f32 scale);
void FUN_0011ff28(EnReeba* self, PlayState* play);
void FUN_0021c0bc(EnReeba* self, PlayState* play);
void FUN_001754b0(EnReeba* self, PlayState* play);
void FUN_00287980(EnReeba* self, PlayState* play);
void FUN_0034c3e4(SkelAnime* skelAnime, void* anim);
void FUN_00353020(SkelAnime* skelAnime, void* anim, f32 speed, f32 start, f32 end, s32 mode, f32 morphFrames);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 FUN_00370734(SkelAnime* skelAnime);
s32 FUN_0035ea34(void* colCtx, CollisionPoly* poly, s32 bgId);
void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, s32 a3, f32 scale, f32 f5, s32 a6, s32 a7, s32 a8);
void Actor_Kill(Actor* actor);
void FUN_00174374(EnReeba* self, PlayState* play);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void FUN_003d4cb0(EnReeba* self, PlayState* play);

GLOBAL_ASM("data/z_En_Reeba.data.s")
GLOBAL_ASM("../src/game/actors/En_Reeba/data_syms.s")
extern u8 EnReeba_sCylinderInit[];
extern u8 EnReeba_sDamageTable[];

ActorInit En_Reeba_InitVars = {
    ACTOR_EN_REEBA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_REEBA,
    sizeof(EnReeba),
    (ActorFunc)EnReeba_Init,
    (ActorFunc)EnReeba_Destroy,
    (ActorFunc)EnReeba_Update,
    (ActorFunc)EnReeba_Draw,
};

void EnReeba_Init(EnReeba* self, PlayState* play) {
    Actor* thisx = &self->actor;
    s32 surfaceType;

    thisx->gravity = -3.5f;
    self->actor.naviEnemyId = 0x47;
    self->actor.targetMode = 3;
    self->actor.focus.pos = self->actor.world.pos;
    FUN_00372f38(&self->actor, play, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 6);
    self->actor.colChkInfo.mass = 0xFE;
    self->actor.colChkInfo.health = 4;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, (ColliderCylinderInit*)EnReeba_sCylinderInit);
    self->type = self->actor.params;
    self->scale = 0.04f;

    if (self->type != 0) {
        self->scale *= 1.5f;
        self->collider.dim.radius = 35.0f;
        self->collider.dim.height = 45.0f;
        self->actor.colChkInfo.health = 20;
        ((u8*)&self->collider)[0x1C] = 4;
        ((u8*)&self->collider)[0x1D] = 16;
        Actor_ChangeType(play, &play->actorCtx, &self->actor, 5);
    }

    thisx->shape.yOffset = self->yOffsetTarget = self->scale * -27500.0f;
    ActorShape_Init(&self->actor.shape, self->yOffsetTarget, (ActorShadowFunc)ActorShadow_DrawCircle, 0.0f);
    self->actor.colChkInfo.damageTable = (DamageTable*)EnReeba_sDamageTable;
    Actor_UpdateBgCheckInfo(play, &self->actor, 35.0f, 60.0f, 60.0f, 0x1D);

    surfaceType = FUN_0035ea34(&play->colCtx, self->actor.floorPoly, self->actor.floorBgId);

    if ((surfaceType != 4) && (surfaceType != 7)) {
        Actor_Kill(&self->actor);
        return;
    }

    self->actionfunc = FUN_00181394;
    thisx->unk_19B = 4;
}

void FUN_00287980(EnReeba* self, PlayState* play) {
}

void FUN_00299348(EnReeba* self, PlayState* play) {
    self->waitTimer = 21;
    self->actor.speedXZ = -8.0f;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    self->actionfunc = FUN_0021d4e8;
}

void FUN_001754b0(EnReeba* self, PlayState* play) {
    self->stunType = 0;
    Audio_PlayActorSound2(&self->actor, 0x0100046b);
    self->unk_498 = 0;
    self->unk_499 = 1;
    self->actionfunc = FUN_003d6118;
}

void FUN_001ca60c(EnReeba* self, PlayState* play) {
    Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 0.3f);

    if (self->moveTimer == 0) {
        self->actionfunc = (self->type != 0) ? FUN_0021c0bc : FUN_001754b0;
    }
}

void FUN_00174660(EnReeba* self, PlayState* play) {
    FUN_0034c3e4(&self->skelAnime, (void*)0x004D9B88);
    self->actor.speedXZ = -8.0f;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 0x10);
    self->waitTimer = 21;
    self->actor.flags &= ~1;
    self->actionfunc = FUN_003d4cb0;
}

void FUN_001c7c90(EnReeba* self, PlayState* play) {
    FUN_0034c3e4(&self->skelAnime, (void*)0x004D9B88);
    self->actor.speedXZ = -8.0f;
    self->damagedTimer = 21;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 0x10);
    self->actionfunc = FUN_0011ff28;
}

void FUN_0021c0bc(EnReeba* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);

    FUN_00353020(&self->skelAnime, (void*)0x004D9B84, 2.0f, 0.0f, frames, 0, -10.0f);
    self->actor.flags |= 5;
    self->actionfunc = FUN_00174374;
}

void FUN_00154564(EnReeba* self, PlayState* play) {
    if (self->waitTimer <= 54) {
        self->actor.shape.rot.x = Rand_CenteredFloat(3000.0f);
        self->actor.shape.rot.z = Rand_CenteredFloat(3000.0f);

        if (self->waitTimer == 0) {
            self->actor.flags &= ~0x8000000;
            self->actor.flags |= 5;
            self->actionfunc = (self->type != 0) ? FUN_0021c0bc : FUN_001754b0;
        }
    }
}

void FUN_003de24c(EnReeba* self, PlayState* play) {
    s32 surfaceType;

    FUN_00370734(&self->skelAnime);

    if (self->actor.shape.shadowScale < 12.0f) {
        Math_ApproachF(&self->actor.shape.shadowScale, 12.0f, 3.0f, 1.0f);
    }

    surfaceType = FUN_0035ea34(&play->colCtx, self->actor.floorPoly, self->actor.floorBgId);

    if ((surfaceType != 4) && (surfaceType != 7)) {
        self->actor.speedXZ = 0.0f;
        self->actionfunc = FUN_001754b0;
    } else if ((self->moveTimer == 0) || (self->actor.xzDistToPlayer < 30.0f) ||
               (self->actor.xzDistToPlayer > 400.0f) || (self->actor.bgCheckFlags & 8)) {
        self->actionfunc = FUN_001754b0;
    } else if (self->sfxTimer == 0) {
        Audio_PlayActorSound2(&self->actor, 0x010003fa);
        self->sfxTimer = 15;
    }
}

void FUN_003d6118(EnReeba* self, PlayState* play) {
    Math_ApproachZeroF(&self->actor.shape.shadowScale, 1.0f, 0.3f);
    Math_ApproachZeroF(&self->actor.speedXZ, 0.1f, 0.3f);
    FUN_00370734(&self->skelAnime);

    if (self->actor.shape.yOffset >= (self->yOffsetTarget + 10.0f)) {
    } else {
        Actor_Kill(&self->actor);
        return;
    }
    {
        if ((*(u32*)((u8*)play + 0x5BF4) % 6) == 0) {  // play->gameplayFrames
            FUN_0036f00c(play, &self->actor, &self->actor.world.pos, 1, self->actor.shape.shadowScale, 8.0f, 500, 10, 1);
        }
        Math_ApproachF(&self->actor.shape.yOffset, self->yOffsetTarget, 1.0f, self->yOffsetStep);
        Math_ApproachF(&self->yOffsetStep, 300.0f, 1.0f, 5.0f);
    }
}

void FUN_0021d4e8(EnReeba* self, PlayState* play) {
    if (self->waitTimer != 0) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 2.0f / 3.0f;
        }
        return;
    }

    self->actor.speedXZ = 0.0f;

    if (self->stunType == 4) {
        goto ice_check;
    }
    if (self->actor.colChkInfo.health == 0) {
        self->waitTimer = 45;
        self->actionfunc = FUN_001747a4;
        return;
    }

ice_check:
    {
        s16 st = self->stunType;
    if (st == 2) {
        Vec3f pos;
        f32 scale;

        pos.x = self->actor.world.pos.x + Rand_CenteredFloat(20.0f);
        pos.y = self->actor.world.pos.y + Rand_CenteredFloat(20.0f);
        pos.z = self->actor.world.pos.z + Rand_CenteredFloat(20.0f);
        scale = 3.0f;
        if (self->type != 0) {
            scale = 6.0f;
        }
        EffectSsEnIce_SpawnFlyingVec3f(play, &self->actor, &pos, 150, 150, 150, 250, 235, 245, 255, scale);
    }

    }
    self->waitTimer = 99;
    self->actionfunc = FUN_00154564;
}

void FUN_001747a4(EnReeba* self, PlayState* play) {
    if (self->waitTimer != 0) {
        if ((self->stunType == 2) && ((self->waitTimer % 24) == 0)) {
            Vec3f pos;
            f32 scale;

            pos.x = self->actor.world.pos.x + Rand_CenteredFloat(20.0f);
            pos.y = self->actor.world.pos.y + Rand_CenteredFloat(20.0f);
            pos.z = self->actor.world.pos.z + Rand_CenteredFloat(20.0f);
            scale = 3.0f;
            if (self->type != 0) {
                scale = 6.0f;
            }
            EffectSsEnIce_SpawnFlyingVec3f(play, &self->actor, &pos, 150, 150, 150, 250, 235, 245, 255, scale);
        }
    } else {
        Audio_PlayActorSound2(&self->actor, 0x010003f9);
        FUN_00375b70(play, &self->actor);
        self->actionfunc = FUN_003d4cb0;
    }
}

void EnReeba_Destroy(Actor* thisx, GameState* state) {
    EnReeba* self = (EnReeba*)thisx;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);

    if (self->actor.parent != NULL) {
        Actor* spawner = self->actor.parent;

        if (spawner->update != NULL) {
            if (*(s16*)((u8*)spawner + 0x1AA) > 0) {
                (*(s16*)((u8*)spawner + 0x1AA))--;
            }
            if (self->type != 0) {
                *(void**)((u8*)spawner + 0x1C4) = NULL;
                *(s16*)((u8*)spawner + 0x1BC) = 900;
            }
        }
    }

    FUN_003508b8(&self->actor, 0);
    SkelAnime_Free2(&self->skelAnime);
}

void EnReeba_Draw(Actor* thisx, GameState* state) {
    EnReeba* self = (EnReeba*)thisx;
    void* gfx = FUN_003687a8(self->skelAnime.unk_28);

    FUN_003589cc(gfx, 4);
    FUN_00358964(gfx, 4, (u8*)0x004D9B8C + ((self->type == 0) << 4));
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)0x00287978,
                      (PostLimbDrawOpa)FUN_00287980, &self->actor, 0);
}

// Draft: 5 words (ROM reads unk_498/499 via the self+0x400 base r5, except the FIRST unk_498 test via r4).
// R11 (2026-10-15): static inline helpers for the burrow-offset block / the setup tail (type check inside or out),
// early-return and ternary tails, greedy single-site + pair mutations (aliases, decl swaps/deletes): all 5.
#if 0
void FUN_0011ff28(EnReeba* self, PlayState* play) {
    FUN_00370734(&self->skelAnime);

    if (self->actor.speedXZ < 0.0f) {
        self->actor.speedXZ += 2.0f / 3.0f;
    }

    if (self->unk_498 != 0) {
        Math_ApproachZeroF(&self->actor.shape.yOffset, 1.0f, self->yOffsetStep);
        Math_ApproachF(&self->yOffsetStep, 300.0f, 1.0f, 5.0f);
    } else if (self->unk_499 != 0) {
        Math_ApproachZeroF(&self->actor.shape.shadowScale, 1.0f, 0.3f);
        Math_ApproachZeroF(&self->actor.speedXZ, 0.1f, 0.3f);
        Math_ApproachF(&self->actor.shape.yOffset, self->yOffsetTarget, 1.0f, self->yOffsetStep);
        Math_ApproachF(&self->yOffsetStep, 300.0f, 1.0f, 5.0f);
    }

    if (self->damagedTimer == 0) {
        if (self->type == 0) {
            self->actionfunc = FUN_001754b0;
        } else {
            self->bigLeeverTimer = 45;
            if (self->unk_498 != 0) {
                self->actionfunc = FUN_00181394;
            } else if (self->unk_499 == 0) {
                self->actionfunc = FUN_0021c0bc;
            } else {
                self->actionfunc = FUN_001754b0;
            }
        }
    }
}
#endif

void FUN_003d4cb0(EnReeba* self, PlayState* play) {
    Vec3f pos;
    Vector3f accel(0.0f, 0.0f, 0.0f);
    Vector3f velocity(0.0f, 0.0f, 0.0f);

    FUN_00370734(&self->skelAnime);

    if (self->waitTimer != 0) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 2.0f / 3.0f;
        }
        return;
    }

    self->actor.speedXZ = 0.0f;
    Math_ApproachZeroF(&self->scale, 0.1f, 0.01f);

    if (self->scale < 0.01f) {
        pos.x = self->actor.world.pos.x;
        pos.y = self->actor.world.pos.y;
        pos.z = self->actor.world.pos.z;

        velocity.y = 4.0f;

        EffectSsDeadDb_Spawn(play, &pos, &velocity, &accel, 120, 0, 255, 255, 255, 255, 255, 0, 0, 1, 11, true);

        if (self->type == 0) {
            Item_DropCollectibleRandom(play, &self->actor, &pos, 0xE0);
        } else {
            Item_DropCollectibleRandom(play, &self->actor, &pos, 0xC0);
        }

        if (self->actor.parent != NULL) {
            Actor* spawner = self->actor.parent;

            if ((spawner->update != NULL) && (self->type == 0)) {
                if (*(s16*)((u8*)spawner + 0x1B6) < 10) {
                    (*(s16*)((u8*)spawner + 0x1B6))++;
                }
            }

            Actor_Kill(&self->actor);
        }
    }
}

void FUN_00181394(EnReeba* self, PlayState* play) {
    f32 frames = Animation_GetLastFrame(&self->skelAnime, 0);
    Player* player = GET_PLAYER(play);
#define SPD ((s16)_fabsf(*(f32*)((u8*)player + 0x221C)))
#define TX (20 - SPD * 2)

    FUN_00353020(&self->skelAnime, (void*)0x004D9B84, 2.0f, 0.0f, frames, 0, -10.0f);
    self->waitTimer = (TX > 0 ? (s32)(0.5f + (TX * 3.0f) * 0.5f) : (s32)((TX * 3.0f) * 0.5f - 0.5f)) / 3;
    if (self->waitTimer < 0) {
        self->waitTimer = 3;
    }
    if (self->waitTimer > 30) {
        self->waitTimer = 30;
    }
    self->actor.world.pos.y = self->actor.floorHeight;
    self->actor.flags &= ~0x8000000;
    self->unk_498 = 1;
    self->actionfunc = FUN_003e8c78;
#undef TX
#undef SPD
}

void FUN_003741e4(PlayState* play, u32 dmgFlags, s32 idx, Vec3f* pos, s32 zero);
u8 Actor_ApplyDamage(Actor* actor);
void FUN_00299348(EnReeba* self, PlayState* play);

// Draft: instruction stream identical to ROM, only r7/r8 swap (play vs FUN_00299348 literal), 35 words.
#if 0
void EnReeba_Update(Actor* thisx, GameState* state) {
    Player* player = GET_PLAYER((PlayState*)state);
    EnReeba* self = (EnReeba*)thisx;
    EnReeba* s2 = self;
    PlayState* play = (PlayState*)state;

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Vec3s* hp = &self->collider.info.bumper.hitPos;
        ColliderInfo* acHitInfo = self->collider.info.acHitInfo;
        Vector3f hitPos(hp->x, hp->y, hp->z);

        if ((self->actionfunc != FUN_003d4cb0) && (self->actionfunc != FUN_0011ff28)) {
            self->stunType = 0;
            self->actor.shape.rot.x = self->actor.shape.rot.z = 0;

            switch (self->actor.colChkInfo.damageEffect) {
                case 0xB:
                case 0xC:
                    if ((self->actor.colChkInfo.health > 1) && (self->stunType != 4)) {
                        self->stunType = 4;
                        Audio_PlayActorSound2(&self->actor, 0x0100030E);
                        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
                        self->actionfunc = FUN_00299348;
                        FUN_003741e4(play, acHitInfo->toucher.dmgFlags, 1, (Vec3f*)&hitPos, 0);
                        break;
                    }
                    /* fallthrough */
                case 0xD:
                    if ((self->actor.colChkInfo.health > 2) && (self->stunType != 4)) {
                        self->stunType = 4;
                        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
                        Audio_PlayActorSound2(&self->actor, 0x0100030E);
                        self->actionfunc = FUN_00299348;
                        FUN_003741e4(play, acHitInfo->toucher.dmgFlags, 1, (Vec3f*)&hitPos, 0);
                        break;
                    }
                    /* fallthrough */
                case 0xE:
                    self->unkDamageField = 6;
                    Actor_ApplyDamage(&self->actor);
                    if (self->actor.colChkInfo.health == 0) {
                        Audio_PlayActorSound2(&self->actor, 0x010003F9);
                        FUN_00375b70(play, &self->actor);
                        self->actionfunc = FUN_00174660;
                    } else {
                        if (self->actionfunc == FUN_00154564) {
                            self->actor.shape.rot.x = self->actor.shape.rot.z = 0;
                        }
                        Audio_PlayActorSound2(&self->actor, 0x010003F8);
                        s2->actionfunc = FUN_001c7c90;
                    }
                    FUN_003741e4(play, acHitInfo->toucher.dmgFlags, 0, (Vec3f*)&hitPos, 0);
                    break;
                case 3:
                    Actor_ApplyDamage(&self->actor);
                    self->unkDamageField = 2;
                    self->stunType = 2;
                    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
                    self->actionfunc = FUN_00299348;
                    FUN_003741e4(play, acHitInfo->toucher.dmgFlags, 0, (Vec3f*)&hitPos, 0);
                    break;
                case 1:
                    if (self->stunType != 4) {
                        self->stunType = 4;
                        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
                        self->actionfunc = FUN_00299348;
                        FUN_003741e4(play, acHitInfo->toucher.dmgFlags, 1, (Vec3f*)&hitPos, 0);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    self->actionfunc(self, play);
    Actor_SetScale(&self->actor, self->scale);

    if (self->actor.shape.yOffset < -1000.0f) {
        self->actor.shape.shadowScale = 0.0f;
    } else {
        self->actor.shape.shadowScale = 12.0f + self->actor.shape.yOffset * 0.012f;
    }

    if (self->bigLeeverTimer != 0) {
        self->bigLeeverTimer--;
    }
    if (self->moveTimer != 0) {
        self->moveTimer--;
    }
    if (self->waitTimer != 0) {
        self->waitTimer--;
    }
    if (self->sfxTimer != 0) {
        self->sfxTimer--;
    }
    if (self->damagedTimer != 0) {
        self->damagedTimer--;
    }

    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 35.0f, 60.0f, 60.0f, 0x1D);

    if (self->collider.base.atFlags & AT_BOUNCED) {
        self->collider.base.atFlags &= ~AT_BOUNCED;

        if ((self->actionfunc == FUN_003de24c) || (self->actionfunc == FUN_00174374)) {
            self->actor.speedXZ = 8.0f;
            self->actor.world.rot.y *= -1.0f;
            self->moveTimer = 21;
            self->actionfunc = FUN_001ca60c;
            return;
        }
    }

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        if ((self->collider.base.at == &player->actor) && (self->type == 0) && (self->actionfunc != FUN_003d6118)) {
            self->actionfunc = FUN_001754b0;
        }
    }

    self->actor.focus.pos = self->actor.world.pos;

    if (self->type == 0) {
        thisx->focus.pos.y += 30.0f;
    } else {
        thisx->focus.pos.y += 15.0f;
    }

    Collider_UpdateCylinder(&self->actor, &self->collider);

    if ((self->actor.shape.yOffset >= -1000.0f) && (self->actor.colChkInfo.health > 0)) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);

        if ((self->actionfunc == FUN_003de24c) || (self->actionfunc == FUN_00174374)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnReeba_Update.s")

}
