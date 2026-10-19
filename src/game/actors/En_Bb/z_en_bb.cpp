extern "C" {
#include "z_en_bb.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x01000015

void EnBb_Init(Actor* actor, GameState* state);
void EnBb_Destroy(Actor* thisx, PlayState* play);
void EnBb_Update(Actor* actor, GameState* state);
void EnBb_Draw(Actor* thisx, GameState* state);

void Collider_DestroyJntSph(PlayState* play, void* collider);
void SkelAnime_Free2(SkelAnime* skelAnime);
void FUN_003508b8(Actor* actor, ...);
void FUN_003504d0(Actor* actor, void* model);
void FUN_0035046c(void* obj);

GLOBAL_ASM("data/z_En_Bb.data_00520A5C.s")
GLOBAL_ASM("data/z_En_Bb.data_00520B80.s")

ActorInit En_Bb_InitVars = {
    ACTOR_EN_BB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BB,
    sizeof(EnBb),
    (ActorFunc)EnBb_Init,
    (ActorFunc)EnBb_Destroy,
    (ActorFunc)EnBb_Update,
    (ActorFunc)EnBb_Draw,
};

GLOBAL_ASM("asm/EnBb_Init.s")

void FUN_0034f184(void* bodyBreak, s32 limbIndex, s32 minLimb, s32 maxLimb, s32 count, MTX34* mtx);

void FUN_002338d8(void* unused, s32 limbIndex, MTX34* mtx, EnBb* self) {
    if ((u32)limbIndex > 6) {
        return;
    }
    FUN_0034f184(self->unk_5F0, limbIndex, 0, 6, 7, mtx);
}

void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void FUN_002659fc(void);

void FUN_0027fc54(EnBb* self) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    if (self->actor.speedXZ == 0.0f) {
        self->actor.shape.yOffset = 200.0f;
        Animation_PlayLoop((SkelAnime*)self->unk_1A4, 0);
        self->actor.speedXZ = 3.0f;
        self->unk_524 = 3;
        self->unk_534 = 0x12C;
        self->actor.gravity = -2.0f;
        self->unk_560 = 0.0f;
        self->unk_55C = 0.0f;
        *(u16*)((u8*)self + 0x11A) = 0;
        self->actor.bgCheckFlags &= ~1;
        Audio_PlayActorSound2(&self->actor, 0x0100033D);
        self->unk_52C = (EnBbActionFunc)FUN_002659fc;
    }
}

void FUN_00247338(EnBb* self, PlayState* play);

void Actor_ApplyDamage(Actor* actor);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void FUN_00375fd0(Actor* actor, void* elem, s32 arg);
void FUN_003741e4(PlayState* play, void* elem, s32 arg, Vec3f* pos, s32 arg2);
void EffectSsDeadSound_SpawnStationary(PlayState* play, Vec3f* pos, s32 sfx, s32 a, s32 b, s32 c);
void FUN_0027fc54(EnBb* self);
void FUN_002A2F88(EnBb* self, PlayState* play);

static inline void EnBb_KillFlameTrail(EnBb* self) {
    Actor* actor = &self->actor;
    Actor* next;

    while (actor->child != NULL) {
        next = actor->child;
        if (next->id == 0x69) {
            next->parent = NULL;
            actor->child = NULL;
            next->params = 0xB;
        }
        actor = next;
    }
    self->actor.child = NULL;
}

static inline void EnBb_SetupDown(EnBb* self) {
    Animation_PlayLoop((SkelAnime*)self->unk_1A4, 0);
    self->actor.speedXZ = 3.0f;
    self->unk_524 = 3;
    self->unk_534 = 0x12C;
    self->actor.gravity = -2.0f;
    self->unk_560 = 0.0f;
    self->unk_55C = 0.0f;
    *(u16*)((u8*)self + 0x11A) = 0;
    self->actor.bgCheckFlags &= ~1;
    Audio_PlayActorSound2(&self->actor, 0x0100033D);
    self->unk_52C = (EnBbActionFunc)FUN_002659fc;
}

// 3DS EnBb_CollisionCheck draft (structure verified, ~50 insn-diffs: callee-saved numbering r7/r8/sb swap, hit-vector load order,
// chain-store order, 2nd SetupDown vmov copy). GLOBAL_ASM kept in functions_1C0000s.cpp.
#if 0
void FUN_001d4b0c(EnBb* self, PlayState* play) {
    s32 damaged;
    u8* elem;
    Vec3f hit;

    if (self->unk_590 & 4) {
        self->unk_590 &= ~4;
        if (self->unk_524 != 3) {
            if (self->actor.params > -3) {
                self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer + 0x8000;
                if (self->actor.params == -2) {
                    EnBb_KillFlameTrail(self);
                }
                EnBb_SetupDown(self);
                return;
            }
            self->unk_542 = 1;
        }
    }
    if (!(self->unk_591 & 2)) {
        return;
    }
    self->unk_591 &= ~2;
    damaged = 0;
    self->unk_57E = self->actor.colChkInfo.damageEffect;
    FUN_00375fd0(&self->actor, self->unk_59C, 0);
    elem = self->unk_59C->acHitElem;
    hit.x = self->unk_59C->hitX;
    hit.y = self->unk_59C->hitY;
    hit.z = self->unk_59C->hitZ;
    switch (self->unk_57E) {
        case 7:
            self->actor.freezeTimer = self->unk_59C->acHitElem[5];
            /* fallthrough */
        case 5:
            self->unk_57C = 0x48;
            goto block_15;
        case 6:
            self->actor.freezeTimer = self->unk_59C->acHitElem[5];
            break;
        case 8:
        case 9:
        case 15:
            if (self->unk_524 != 4) {
                Actor_ApplyDamage(&self->actor);
                self->unk_524 = 4;
                if (self->actor.params != -3) {
                    if (self->actor.params == -2) {
                        EnBb_KillFlameTrail(self);
                    } else {
                        if (self->actor.params > -4) {
                            self->actor.gravity = -2.0f;
                            self->actor.shape.yOffset = 1500.0f;
                        }
                        self->actor.speedXZ = 0.0f;
                        self->unk_560 = 0.0f;
                        self->unk_55C = 0.0f;
                    }
                }
                if (self->unk_57E == 8) {
                    Actor_SetColorFilter(&self->actor, 0x800000, 200, 0, 0x50);
                } else {
                    if (self->unk_57E == 9) {
                        self->unk_57C = 0x48;
                    }
                    if (self->unk_57E == 9 || self->unk_57E == 15) {
                        Audio_PlayActorSound2(&self->actor, 0x0100030E);
                        Actor_SetColorFilter(&self->actor, 0, 180, 0, 0x50);
                    }
                }
                self->actor.bgCheckFlags &= ~1;
                self->unk_52C = (EnBbActionFunc)FUN_002A2F88;
                if (self->actor.colChkInfo.damage != 0) {
                    damaged = 1;
                }
            }
            break;
        default:
        block_15:
            if (self->unk_57E == 14 || self->unk_57E == 12 || self->unk_57E == 11 || self->unk_57E == 10 ||
                self->unk_57E == 7 || self->unk_57E == 5) {
                if (self->unk_524 != 3 || self->unk_534 <= 0x11C) {
                    Actor_ApplyDamage(&self->actor);
                    if (self->actor.colChkInfo.damage != 0) {
                        damaged = 1;
                    }
                }
                if (self->unk_524 != 3 && self->actor.params != -3) {
                    EnBb_SetupDown(self);
                }
            } else if ((self->unk_524 == 3 && self->unk_534 <= 0x11C) ||
                       (self->actor.params != -3 && self->unk_560 < 20.0f)) {
                Actor_ApplyDamage(&self->actor);
                if (self->actor.colChkInfo.damage != 0) {
                    damaged = 1;
                }
            } else {
                self->unk_591 |= 2;
            }
            if (self->actor.colChkInfo.health == 0) {
                self->actor.flags &= ~1;
                if (self->actor.params == -2) {
                    EnBb_KillFlameTrail(self);
                }
                if (self->actor.params < 0) {
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                    self->actor.speedXZ = -7.0f;
                    self->unk_534 = 8;
                    self->actor.shape.rot.x += 0x4E20;
                    EffectSsDeadSound_SpawnStationary(play, &self->actor.world.pos, 0x0100033E, 1, 1, 0x28);
                }
                self->unk_524 = 1;
                self->unk_52C = (EnBbActionFunc)FUN_00247338;
            } else if (self->actor.params == -3 && (self->unk_524 == 8 || self->unk_524 == 4)) {
                if (self->actor.colChkInfo.damage != 0) {
                    Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 0x18);
                }
                self->actor.speedXZ = -8.0f;
                self->unk_550 = 0.0f;
                self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                Audio_PlayActorSound2(&self->actor, 0x01000353);
            } else if ((self->unk_524 == 3 && self->unk_534 <= 0x11C) ||
                       (self->actor.params != -3 && self->unk_560 < 20.0f)) {
                self->unk_524 = 0;
                Audio_PlayActorSound2(&self->actor, 0x01000353);
                if (self->actor.params > -4) {
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                    if (!(self->actor.bgCheckFlags & 8)) {
                        self->actor.speedXZ = -7.0f;
                    }
                    self->actor.shape.yOffset = 1500.0f;
                    if (self->actor.params == -2) {
                        EnBb_KillFlameTrail(self);
                    }
                }
                if (self->actor.colChkInfo.damage != 0) {
                    Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 0x18);
                }
                self->unk_534 = 8;
                self->unk_52C = (EnBbActionFunc)FUN_0027fc54;
            }
            break;
        case 13:
            break;
    }
    if (damaged) {
        FUN_003741e4(play, *(void**)elem, 0, &hit, 0);
    } else {
        FUN_003741e4(play, *(void**)elem, 1, &hit, 0);
    }
}
#endif

void FUN_002478a4(EnBb* self) {
    if (self->actor.params == 0xB) {
        if (self->actor.parent == NULL) {
            self->unk_524 = 1;
            self->unk_52C = (EnBbActionFunc)FUN_00247338;
            return;
        }
        self->actor.velocity.y = self->actor.parent->velocity.y;
        return;
    }
    if (self->unk_534 == 0) {
        if (self->unk_604->unk_55C != 0.0f) {
            Math_SmoothStepToF(&self->unk_55C, self->actor.scale.y, 1.0f, self->actor.scale.y * 0.1f, 0.0f);
            Math_SmoothStepToF(&self->unk_560, self->actor.scale.z, 1.0f, self->actor.scale.z * 0.1f, 0.0f);
            {
                s16 limit = *(s16*)((u8*)self + 0x542);
                if (self->unk_573 != limit) {
                    self->unk_573 += 10;
                    if (self->unk_573 > limit) {
                        self->unk_573 = limit;
                    }
                }
            }
        } else {
            if (self->unk_573 == 0) { Actor_Kill(&self->actor); return; }
            if (self->unk_573 <= 0x14) { self->unk_573 = 0; } else { self->unk_573 -= 0x14; }
        }
        self->actor.world.pos = self->actor.parent->prevPos;
    } else {
        u16 rotY;
        self->unk_534--;
        rotY = self->actor.parent->world.rot.y;
        self->actor.world.rot.y = rotY;
        self->actor.shape.rot.y = rotY;
    }
    if (self->actor.parent == NULL) {
        return;
    }
    self->actor.velocity.y = self->actor.parent->velocity.y;
}
void EnBb_Destroy(Actor* thisx, PlayState* play) {
    EnBb* self = (EnBb*)thisx;

    Collider_DestroyJntSph(play, self->unk_580);
    SkelAnime_Free2((SkelAnime*)self->unk_1A4);
    FUN_003508b8(thisx, 0);
    FUN_003504d0(thisx, self->unk_5F0);
    FUN_0035046c(self->unk_500);
    if (self->unk_577 != 0) {
        FUN_0035046c(self->unk_50C);
    }
}

void FUN_001d4b0c(EnBb* self, PlayState* play);

// Draft ~22 words off: ROM predication/BgCheck const order/r2 temp for unk_59C. GLOBAL_ASM kept.
#if 0
void EnBb_Update(EnBb* self, PlayState* play) {
    f32 zero;
    f32 sp34;
    zero = 0.0f;
    sp34 = -15.0f;

    if (self->actor.params < 0) {
        FUN_001d4b0c(self, play);
    }
    if (self->actor.colChkInfo.damageEffect == 0xD) {
        return;
    }
    self->unk_52C(self, play);
    if (self->actor.params < 0 && !(self->actor.speedXZ < -6.0f) && !(self->actor.flags & 0x8000)) {
        Actor_MoveForward(&self->actor);
    }
    if (self->unk_530 == 0) {
        if (self->actor.world.pos.y - 20.0f <= self->actor.floorHeight) {
            sp34 = 20.0f;
        }
        Actor_UpdateBgCheckInfo(play, &self->actor, sp34, 25.0f, 20.0f, 5);
    }
    self->actor.focus.pos = self->actor.world.pos;
    self->unk_59C->cx = self->actor.world.pos.x;
    self->unk_59C->cy = self->actor.world.pos.y + self->actor.shape.yOffset * self->actor.scale.y;
    self->unk_59C->cz = self->actor.world.pos.z;
    if (self->unk_524 >= 2) {
        if (self->actor.speedXZ != zero || self->unk_524 == 9) {
            if ((self->actor.params == -1 || self->actor.params == -2) && self->unk_524 == 3) {
                goto L778;
            }
            if ((self->actor.params == -4 || self->actor.params == -5) &&
                (self->unk_560 == zero || self->unk_55C == zero)) {
                goto L76C;
            }
            if (self->actor.params == -3) {
                goto L76C;
            }
            CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)self->unk_580);
        }
    L76C:
        if (self->unk_524 >= 3) {
        L778:
            if (*(u16*)((u8*)self + 0x11A) != 0 && (*(u32*)((u8*)self + 0x11C) & 0x400000)) {
                return;
            }
            if (self->unk_530 == 2) {
                return;
            }
            {
                f32 kb = 2000.0f;
                f32 ks = 1200.0f;
                if (self->actor.params == -1) {
                    if (self->unk_524 == 3) {
                self->unk_59C->unk_34 = self->actor.scale.x * kb;
                self->unk_59C->unk_44 = self->actor.scale.x * kb;
                    } else {
                self->unk_59C->unk_34 = self->actor.scale.x * ks;
                self->unk_59C->unk_44 = self->actor.scale.x * ks;
                    }
                } else if (self->actor.params == -4) {
                    if (self->unk_560 == zero || self->unk_55C == zero) {
                self->unk_59C->unk_34 = self->actor.scale.x * kb;
                self->unk_59C->unk_44 = self->actor.scale.x * kb;
                    } else {
                self->unk_59C->unk_34 = self->actor.scale.x * ks;
                self->unk_59C->unk_44 = self->actor.scale.x * ks;
                    }
                }
            }
            CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)self->unk_580);
            CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)self->unk_580);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnBb_Update.s")


// EnBb_Draw draft (3DS), raw-offset model fields, ~150 insn-diffs (frame layout 0x84 vs 0x88, vec/mtx slot order, lens/alpha
// constants s16..s20 hoisting not reproduced). GLOBAL_ASM kept below.
#if 0
void Matrix_RotateY(MTX34* mtx, s32 mode, f32 angle);
s16 Camera_GetCamDirYaw(Camera* camera);
void FUN_00357ef8(void* model, f32 r, f32 g, f32 b, f32 a);
void FUN_00373bec(void* model);
void FUN_003586ec(void* model);
void FUN_00357ed4(PlayState* play, void* obj, void* cb, void* self);
void* FUN_00362384(void* p);
void FUN_003620f0(void* p, Vec3f* a, Vec3f* b);
void FUN_0035eb74(void* p);
void FUN_001f2e74(void);
void EffectSsEnIce_SpawnFlyingVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 pr, s32 pg, s32 pb, s32 er, s32 eg,
                                    s32 eb, s32 alpha, f32 scale);
void EffectSsEnFire_SpawnVec3f(PlayState* play, Actor* actor, Vec3f* pos, s32 scale, s32 flags, s32 flags2, s32 unused,
                               s32 extra);
void FUN_002338d8(void* unused, s32 limbIndex, MTX34* mtx, EnBb* self);

extern Vec3f EnBb_sFireIceOffsets[12]; // 0x00520AD0

void EnBb_Draw(Actor* thisx, GameState* state) {
    EnBb* self = (EnBb*)thisx;
    PlayState* play = (PlayState*)state;
    Vec3f blureBase1;
    Vec3f blureBase2;
    Vec3f v1;
    Vec3f v2;
    MTX34 mtx;
    Vec3f t;

    blureBase1.x = 0.0f;
    blureBase1.y = 5000.0f;
    blureBase1.z = 0.0f;
    blureBase2.x = 0.0f;
    blureBase2.y = 2000.0f;
    blureBase2.z = 0.0f;
    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    blureBase1.z = self->unk_550 * 80.0f;
    blureBase2.z = self->unk_550 * 80.0f;
    if (self->unk_530 == 2) {
        return;
    }
    if (self->actor.params < 0) {
        SkelAnime_DrawOpa((SkelAnime*)self->unk_1A4, &self->actor.modelMtx, NULL, (PostLimbDrawOpa)FUN_002338d8, &self->actor, 0);
        if (self->unk_57C != 0) {
            *(u16*)((u8*)self + 0x11A) += 1;
            self->unk_57C--;
            if (!(self->unk_57C & 5)) {
                s32 index = (self->unk_57C > 0 ? (s32)((f32)self->unk_57C * 2.0f / 3.0f + 0.5f)
                                               : (s32)((f32)self->unk_57C * 2.0f / 3.0f - 0.5f)) >> 2;

                t.x = self->actor.world.pos.x + EnBb_sFireIceOffsets[index].x;
                t.y = self->actor.world.pos.y + EnBb_sFireIceOffsets[index].y;
                t.z = self->actor.world.pos.z + EnBb_sFireIceOffsets[index].z;
                if (self->unk_57E != 7 && self->unk_57E != 5) {
                    EffectSsEnIce_SpawnFlyingVec3f(play, &self->actor, &t, 150, 150, 150, 250, 235, 245, 255, 0.8f);
                } else {
                    t.y -= 17.0f;
                    EffectSsEnFire_SpawnVec3f(play, &self->actor, &t, 40, 1, 0, -1, 1);
                }
            }
        }
        t.x = 0.0f;
        t.y = self->unk_560 * -40.0f;
        t.z = 0.0f;
    } else {
        t.x = 0.0f;
        t.y = -40.0f;
        t.z = 0.0f;
    }
    Matrix_UpdatePosition(&mtx, &mtx, &t);
    if (self->actor.params == -3) {
        VEC3TransformAsm((Vector3f*)&v1, &mtx, (Vector3f*)&blureBase1);
        VEC3TransformAsm((Vector3f*)&v2, &mtx, (Vector3f*)&blureBase2);
        if (self->unk_550 != 0.0f) {
            if (self->unk_524 == 8) {
                if (*(u32*)((u8*)play + 0x5BF4) & 1) {
                    return;
                }
                if (self->actor.colChkInfo.health == 0) {
                    return;
                }
                FUN_003620f0(FUN_00362384(*(void**)((u8*)self + 0x578)), &v1, &v2);
                return;
            }
        } else if (self->unk_524 == 8) {
            return;
        }
        FUN_00362384(*(void**)((u8*)self + 0x578));
        FUN_0035eb74(0);
        return;
    }
    {
        f32 alphaScale;
        s32 lens;

        FUN_00357fd0(GET_PLAYER(play), self->actor.unk_178, &self->actor.world.pos);
        Matrix_RotateY(&mtx, 1,
                       (s16)(Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) - self->actor.shape.rot.y +
                             0x8000) *
                           9.58738e-5f);
        {
            f32 sx = self->unk_560 * 0.01f;
            f32 sy = self->unk_55C * 0.01f;
            f32 sz = 1.0f;

            mtx.data[0][0] *= sx;
            mtx.data[1][0] *= sx;
            mtx.data[2][0] *= sx;
            mtx.data[0][1] *= sy;
            mtx.data[1][1] *= sy;
            mtx.data[2][1] *= sy;
            mtx.data[0][2] *= sz;
            mtx.data[1][2] *= sz;
            mtx.data[2][2] *= sz;
        }
        if (self->unk_577 != 0 && *((u8*)play + 0x208F) != 0 && *((u8*)play + 0x4C35) == 0) {
            lens = 1;
        } else {
            lens = 0;
        }
        alphaScale = 1.0f;
        if (lens != 0) {
            alphaScale = 0.2f;
        }
        *(f32*)(*(u8**)((u8*)self + 0x508) + 0xC) = PauseContext_GetState() != 0 ? 0.0f : 1.0f;
        FUN_00373bec(*(void**)((u8*)self + 0x508));
        FUN_00357ef8((u8*)self + 0x500, -1.0f, -1.0f, -1.0f, (f32)self->unk_573 * 0.003921569f * alphaScale);
        ((SkeletonAnimationModel*)(*(void**)((u8*)self + 0x500)))->SetUnkAC(1);
        ((SkeletonAnimationModel*)(*(void**)((u8*)self + 0x500)))->FUN_003721e0(&mtx);
        ((SkeletonAnimationModel*)(*(void**)((u8*)self + 0x500)))->FUN_00372170(0);
        if (lens != 0) {
            FUN_00357ed4(play, (u8*)self + 0x518, (void*)FUN_001f2e74, self);
            *(f32*)(*(u8**)((u8*)self + 0x514) + 0xC) = 0.0f;
            FUN_003586ec(*(void**)((u8*)self + 0x514));
            FUN_00373bec(*(void**)((u8*)self + 0x514));
            FUN_00357ef8((u8*)self + 0x50C, -1.0f, -1.0f, -1.0f, (f32)self->unk_573 * 0.003921569f * (1.0f - alphaScale));
            ((SkeletonAnimationModel*)(*(void**)((u8*)self + 0x50C)))->SetUnkAC(1);
            ((SkeletonAnimationModel*)(*(void**)((u8*)self + 0x50C)))->FUN_003721e0(&mtx);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnBb_Draw.s")
}
