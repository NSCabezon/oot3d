extern "C" {
#include "z_en_sw.hpp"

#define FLAGS 0x00000035

void EnSw_Init(Actor* thisx, PlayState* play);
void EnSw_Destroy(Actor* thisx, PlayState* play);
void EnSw_Update(Actor* thisx, PlayState* play);
void EnSw_Draw(Actor* thisx, PlayState* play);
void Collider_DestroyJntSph(PlayState* play, void* collider);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 BgCheck_EntityLineTest1(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** outPoly,
                            s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkActors, s32* outBgId);
extern s32 EnSw_sData[];

// Sub-object of SkelAnime at +0x30 (animationType .. playSpeed).
typedef struct {
    s32 animationType;
    char pad[0xC];
    f32 playSpeed;
} EnSwAnimCtl;

GLOBAL_ASM("../src/game/actors/En_Sw/data_syms.s")

GLOBAL_ASM("data/z_En_Sw.data_0052F258.s")
GLOBAL_ASM("data/z_En_Sw.data_0052F300.s")

void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
void FUN_003afac8(EnSw* self, PlayState* play);

s32 FUN_0035f250(PlayState* play);

void FUN_0011770c(EnSw* self) {
    if ((self->actor.params & 0xE000) == 0x8000) {
        self->unk_744 = 0;
        self->actionFunc = (EnSwActionFunc)FUN_003afac8;
    } else {
        self->unk_744 = 0xF;
        self->actionFunc = (EnSwActionFunc)FUN_003afac8;
    }
}

s32 FUN_001e29e4(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    EnSw* self = (EnSw*)arg;

    if (*(u8*)((u8*)self + 0x219) == limbIndex) {
        Matrix_RotateX(mtx, 1, self->unk_828);
    }
    return 0;
}

s32 FUN_0035f5e4(EnSw* self, f32 arg1, s16 arg2, s32 arg3, PlayState* play);
void FUN_003cdaac(EnSw* self, PlayState* play);
void FUN_003f9170(EnSw* self, PlayState* play);
void FUN_003abe00(EnSw* self, PlayState* play);

// Drafts (not matched yet, kept as GLOBAL_ASM in functions_*.cpp).
static inline s16 EnSw_Vec3fPitch(Vec3f* a, Vec3f* b) {
    f32 dx = b->x - a->x;
    f32 dy = a->y - b->y;
    f32 dz = b->z - a->z;

    return Math_Atan2S(__sqrtf(SQ(dx) + SQ(dz)), dy);
}

s16 FUN_0035f7a8(Actor* thisx, Vec3f* arg1);
#if 0
s16 FUN_0035f7a8(Actor* thisx, Vec3f* arg1) {
    s16 pitch;
    s16 yaw;

    yaw = Math_Vec3f_Yaw(&thisx->world.pos, arg1) - thisx->wallYaw;
    pitch = EnSw_Vec3fPitch(&thisx->world.pos, arg1) - 0x4000;
    return pitch * (yaw >= 0 ? -1 : 1);
}
#endif

void FUN_0035f268(Actor* thisx, Vec3f arg1, f32 arg4) {
    f32 xDist;
    f32 yDist;
    f32 zDist;
    f32 dist;
    f32 xDiff;
    f32 yDiff;
    f32 zDiff;

    Math_SmoothStepToF(&thisx->speedXZ, arg4, 0.3f, 100.0f, 0.1f);
    yDiff = arg1.y - thisx->world.pos.y;
    f32 cached = thisx->world.pos.z;
    zDiff = arg1.z - cached;
    xDiff = arg1.x - thisx->world.pos.x;
    dist = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
    if (dist == 0.0f) {
        xDist = yDist = zDist = 0.0f;
    } else {
        xDist = xDiff / dist;
        yDist = yDiff / dist;
        zDist = zDiff / dist;
    }
    xDist *= thisx->speedXZ;
    yDist *= thisx->speedXZ;
    zDist *= thisx->speedXZ;
    thisx->world.pos.x += xDist;
    thisx->world.pos.y += yDist;
    thisx->world.pos.z += zDist;
}


s16 FUN_0035f7a8(Actor* thisx, Vec3f* arg1);
void FUN_0035f268(Actor* thisx, Vec3f arg1, f32 arg4);

void FUN_003c2a50(EnSw* self) {
    FUN_0035f268(&self->actor, self->unk_7DC, 0.0f);
    if (self->actor.speedXZ == 0.0f) {
        self->unk_7D8 = FUN_0035f7a8(&self->actor, &self->actor.home.pos);
        self->unk_7DC = self->actor.home.pos;
        self->actionFunc = (EnSwActionFunc)FUN_003f9170;
    }
}

void FUN_003f9170(EnSw* self, PlayState* play) {
    if (FUN_0035f5e4(self, 6.0f, 0x3E8, 0, play)) {
        FUN_0035f268(&self->actor, self->unk_7DC, 2.0f);
        if (!(Math_Vec3f_DistXYZ(&self->actor.world.pos, &self->unk_7DC) > 4.0f)) {
            self->actionFunc = (EnSwActionFunc)FUN_003cdaac;
        }
    }
}

void FUN_0015f738(EnSw* self) {
    f32 frame = self->skelAnime.curFrame;

    if (frame == 0.0f || (frame > 7.9f && frame < 8.1f) || (frame > 16.6f && frame < 16.7f) ||
        (frame > 20.6f && frame < 20.7f) || (frame > 22.6f && frame < 22.7f) || (frame > 25.3f && frame < 25.4f)) {
        Audio_PlayActorSound2(&self->actor, 0x010002EB);
    }

    if (DECR(self->unk_74C) == 0) {
        self->unk_74C = 12;
        self->actionFunc = (EnSwActionFunc)FUN_003abe00;
    }
}

s32 FUN_0035f830(EnSw* self, PlayState* play, s32 arg2);
void FUN_0012cdb0(Actor* thisx, PlayState* play);

void FUN_003cdaac(EnSw* self, PlayState* play) {
    f32 rand;

    if (FUN_0035f5e4(self, 6.0f, 0x3E8, 1, play)) {
        f32 t;
        f32 d;
        f32 sign;

        rand = Rand_ZeroOne();
        t = (f32)((s16)(20000.0f * rand) + 0x2EE0);
        d = Rand_ZeroOne() - 0.5f;
        sign = (d >= 0.0f) ? 1.0f : -1.0f;
        self->unk_7D8 = t * sign + self->actor.world.rot.z;
        self->unk_740 = Rand_S16Offset_003702c8(10, 30);
    }

    if (DECR(self->unk_7D6) == 0 && FUN_0035f830(self, play, 1)) {
        Audio_PlayActorSound2(&self->actor, 0x01000462);
        self->unk_7D6 = 20;
        self->actionFunc = (EnSwActionFunc)FUN_0012cdb0;
    }
}

s32 FUN_0035f324(EnSw* self, PlayState* play);

void FUN_0012cdb0(Actor* thisx, PlayState* play) {
    EnSw* self = (EnSw*)thisx;
    Player* player = GET_PLAYER(play);
    Vec3f* tgt = &self->unk_7DC;

    if (DECR(self->unk_7D6) != 0) {
        if (FUN_0035f830(self, play, 1)) {
            *tgt = player->actor.world.pos;
            self->unk_7DC.y += 30.0f;
            self->unk_7D8 = FUN_0035f7a8(&self->actor, tgt);
            FUN_0035f5e4(self, 6.0f, 0xFA0, 0, play);
        } else {
            self->actionFunc = (EnSwActionFunc)FUN_003cdaac;
        }
    } else {
        if (!FUN_0035f324(self, play)) {
            self->unk_7D6 = Rand_S16Offset_003702c8(20, 10);
            self->unk_7D8 = FUN_0035f7a8(&self->actor, &thisx->home.pos);
            *tgt = thisx->home.pos;
            self->actionFunc = (EnSwActionFunc)FUN_003f9170;
            return;
        } else {
            FUN_0035f268(&self->actor, self->unk_7DC, 8.0f);
            if (DECR(self->unk_7D4) == 0) {
                Audio_PlayActorSound2(&self->actor, 0x010002FD);
                self->unk_7D4 = 4;
            }
            if (!(Math_Vec3f_DistXYZ(&thisx->world.pos, tgt) > 13.0f) || FUN_0035f250(play)) {
                self->actionFunc = (EnSwActionFunc)FUN_003c2a50;
            }
        }
    }
}

s32 FUN_0035f830(EnSw* self, PlayState* play, s32 arg2) {
    Player* player = GET_PLAYER(play);
    CollisionPoly* sp58;
    s32 sp54;
    Vec3f sp48;

    if (!(player->stateFlags1 & 0x200000) && arg2) {
        return false;
    } else if (FUN_0035f250(play) && arg2) {
        return false;
    } else if (ABS(FUN_0035f7a8(&self->actor, &player->actor.world.pos) - self->actor.shape.rot.z) > 0x1FC1) {
        return false;
    } else if (Math_Vec3f_DistXYZ(&self->actor.world.pos, &player->actor.world.pos) >= 130.0f) {
        return false;
    } else if (!BgCheck_EntityLineTest1(&play->colCtx, &self->actor.world.pos, &player->actor.world.pos, &sp48, &sp58,
                                        true, false, false, true, &sp54)) {
        return true;
    } else {
        return false;
    }
}

s32 FUN_0035f5e4(EnSw* self, f32 arg1, s16 arg2, s32 arg3, PlayState* play) {
    Camera* activeCam;
    EnSwAnimCtl* ac;
    f32 lastFrame = (f32)Animation_GetLastFrame(&self->skelAnime, EnSw_sData[1]);

    ac = (EnSwAnimCtl*)&self->skelAnime.animationType;
    if (DECR(self->unk_740) != 0) {
        Math_SmoothStepToF(&ac->playSpeed, 0.0f, 0.6f, 1000.0f, 0.01f);
        return 0;
    }

    Math_SmoothStepToF(&ac->playSpeed, arg1, 0.6f, 1000.0f, 0.01f);

    if ((arg3 == 1) && (lastFrame < (self->skelAnime.curFrame + self->skelAnime.playSpeed))) {
        return 0;
    }

    activeCam = play->cameraPtrs[play->activeCamera];

    if (Math_Vec3f_DistXYZ(&self->actor.world.pos, &activeCam->eye) < 380.0f) {
        if (DECR(self->unk_7D4) == 0) {
            Audio_PlayActorSound2(&self->actor, 0x010002FC);
            self->unk_7D4 = 4;
        }
    } else {
        self->unk_7D4 = 0;
    }
    Math_SmoothStepToS(&self->actor.shape.rot.z, self->unk_7D8, 4, arg2, arg2);
    self->actor.world.rot = self->actor.shape.rot;
    if (self->actor.shape.rot.z == self->unk_7D8) {
        return 1;
    }
    return 0;
}

void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f);
extern struct_80034EC0_Entry EnSw_sAnimInfo[];

// FUN_001c31c0 (3DS PostLimbDraw, limb 0 = N64 limb 1): 24 words off (best greedy mix of per-vector Set-helper
// param orders; the six literal Vec3f inits resist: ROM loads 1400/-2600/-800 into s5/s2/s3, copies to s0/s1/s4).
// Calls/frame/Collider_UpdateSpheres already match with the N64-order MultVec3f wrapper.
#if 0
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
static inline void EnSw_MultVec3f(Vec3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, (Vector3f*)src);
}
static inline void EnSw_Set0(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}
void FUN_001c31c0(PlayState* play, s32 limbIndex, MTX34* mtx, EnSw* self) {
    Vec3f sp7C;
    Vec3f sp70;
    Vec3f sp64;
    Vec3f sp58;
    Vec3f sp4C;
    Vec3f sp3C;

    EnSw_Set0(&sp7C, 1400.0f, -2600.0f, -800.0f);
    EnSw_Set0(&sp70, 1400.0f, -1600.0f, 0.0f);
    EnSw_Set0(&sp64, -1400.0f, -2600.0f, -800.0f);
    EnSw_Set0(&sp58, -1400.0f, -1600.0f, 0.0f);
    EnSw_Set0(&sp4C, 0.0f, 0.0f, -600.0f);
    EnSw_Set0(&sp3C, 0.0f, 0.0f, 0.0f);
    if (limbIndex == 0) {
        EnSw_MultVec3f(&sp7C, (Vector3f*)&self->unk_7E8[0], &self->unk_780);
        EnSw_MultVec3f(&sp70, (Vector3f*)&self->unk_7E8[1], &self->unk_780);
        EnSw_MultVec3f(&sp64, (Vector3f*)&self->unk_7E8[2], &self->unk_780);
        EnSw_MultVec3f(&sp58, (Vector3f*)&self->unk_7E8[3], &self->unk_780);
        EnSw_MultVec3f(&sp4C, (Vector3f*)&self->unk_7E8[4], &self->unk_780);
        EnSw_MultVec3f(&sp3C, (Vector3f*)&self->actor.focus.pos, &self->unk_780);
    }
    Collider_UpdateSpheres(limbIndex, &self->collider, &self->unk_780);
}
#endif

// FUN_003abe00 (death shrink + sparkles): 4 words off, only the literal-pool order of 0.0f/0.5f (ROM: 0.0 first,
// then 0.5; ours 0.5 first) after the velAndAccel/pos Set-helper param orders were brute-forced.
#if 0
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep,
                          s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unused,
                          s16 life, s32 arg);
void Item_DropCollectibleRandom(PlayState* play, Actor* fromActor, Vec3f* spawnPos, s16 params);
static inline void EnSw_SetVecA(Vec3f* v, f32 y, f32 x, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}
static inline void EnSw_SetVecB(Vec3f* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}
void FUN_003abe00(Actor* thisx, PlayState* play) {
    EnSw* self = (EnSw*)thisx;
    Vec3f velAndAccel;
    EnSw_SetVecA(&velAndAccel, 0.5f, 0.0f, 0.0f);
    Vec3f pos;
    EnSw_SetVecB(&pos, 0.0f, 0.0f, 0.0f);

    thisx->scale.x *= 0.9f;
    thisx->scale.y *= 0.9f;
    thisx->scale.z *= 0.9f;
    self->unk_824 *= 0.9f;

    if (DECR(self->unk_74C) != 0) {
        if ((self->unk_74C % 3) != 0) {
            pos.y = thisx->world.pos.y + (10.0f + (Rand_ZeroOne() - 0.5f) * 6.0f);
            pos.x = thisx->world.pos.x + (Rand_ZeroOne() - 0.5f) * 32.0f;
            pos.z = thisx->world.pos.z + (Rand_ZeroOne() - 0.5f) * 32.0f;
            EffectSsDeadDb_Spawn(play, &pos, &velAndAccel, &velAndAccel, 42, 0, 255, 255, 255, 255, 255, 0, 0, 1, 11,
                                 1);
        }
    } else {
        Item_DropCollectibleRandom(play, NULL, &thisx->world.pos, 0x30);
        Actor_Kill(thisx);
    }
}
#endif

// EnSw_Update (N64 Update + func_80B0C9F0 + func_80B0CBE8 inlined, FUN_003415a8 = generic 3DS gate called twice):
// draft 127 words off. ROM keeps real branches for the `health != 0` / lens test (we if-convert to ldrne/cmpne),
// and sinks the unk_74A test into the dist/params chain. N64-style static helper made it worse (177).
#if 0
s32 FUN_003415a8(Actor* thisx, PlayState* play);
void FUN_0012ba6c(EnSw* self, PlayState* play);
void FUN_003b91bc(EnSw* self, PlayState* play);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
u8 Actor_ApplyDamage(Actor* actor);
void FUN_00375b70(PlayState* play, Actor* actor);

void EnSw_Update(Actor* thisx, PlayState* play) {
    EnSw* self = (EnSw*)thisx;

    if (play->sceneNum == 0x55 && thisx->scale.x == 0.0f) {
        thisx->flags |= 0x20000000;
    } else {
        thisx->flags &= ~0x20000000;
    }

    FUN_00370734(&self->skelAnime);
    if (FUN_003415a8(thisx, play) == 0) {
        s32 hit = 0;

        if (thisx->xyzDistToPlayerSq < 160000.0f && (thisx->params & 0xE000) == 0 && self->unk_74A == 0) {
            if (thisx->colChkInfo.health != 0 && play->actorCtx.unk_02 != 0) {
                thisx->colChkInfo.damage = thisx->colChkInfo.health;
                hit = 1;
            }
        }

        if (self->unk_74A == 0) {
            if ((self->collider.base.acFlags & 2) || hit) {
                self->collider.base.acFlags &= ~2;
                self->unk_74A = 0x18;
                Actor_SetColorFilter(thisx, 0x400000, 200, 0, 0x10);
                if (Actor_ApplyDamage(thisx) != 0) {
                    Audio_PlayActorSound2(thisx, 0x010002DB);
                } else {
                    FUN_00375b70(play, thisx);
                    if ((thisx->params & 0xE000) != 0) {
                        self->skelAnime.playSpeed = 8.0f;
                        if ((play->state.frames & 1) == 0) {
                            self->unk_7B8 = 0.1f;
                        } else {
                            self->unk_7B8 = -0.1f;
                        }
                        self->unk_74C = 0xF;
                        self->unk_742 = 1;
                        self->unk_7B8 *= 4.0f;
                        self->actionFunc = (EnSwActionFunc)FUN_003b91bc;
                    } else {
                        thisx->shape.shadowDraw = ActorShadow_DrawCircle;
                        thisx->shape.shadowAlpha = 255;
                        self->unk_742 = 2;
                        thisx->shape.shadowScale = 16.0f;
                        thisx->gravity = -1.0f;
                        thisx->flags &= ~1;
                        self->actionFunc = (EnSwActionFunc)FUN_003ac31c;
                    }
                    Audio_PlayActorSound2(thisx, 0x010002F5);
                }
                goto run;
            }
        }
        if (self->unk_748 == 0 && (self->collider.base.atFlags & 2)) {
            self->unk_748 = 0x2D;
        }
    }
run:
    self->actionFunc(self, play);

    if (FUN_003415a8(thisx, play) == 0) {
        if ((thisx->params & 0xE000) != 0 && self->actionFunc != (EnSwActionFunc)FUN_0012ba6c) {
            if (self->unk_74A != 0) {
                self->unk_74A--;
            }
        } else {
            if (DECR(self->unk_748) == 0 && thisx->colChkInfo.health != 0) {
                CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
            }
            if (DECR(self->unk_74A) == 0 && thisx->colChkInfo.health != 0) {
                CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
            }
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        }
    }
}
#endif

// EnSw_Init draft (~220 words off): control flow/data match the ROM; the diff is register allocation (ROM keeps
// play in sb and `1` in sl, armcc hoists `mov sb,#1` to the entry because of phi+1) and the gs-flag word load.
// Needs EQUs EnSw_sJntSphInit 0x0052F264 / EnSw_sColChkInfoInit 0x0052F274 in data_syms.s when enabled.
#if 0
void FUN_0011770c(EnSw* self);
void FUN_0012ba6c(EnSw* self, PlayState* play);
void FUN_003cdaac(EnSw* self, PlayState* play);
s32 FUN_0035f324(EnSw* self, PlayState* play);
s32 FUN_0034ecb0(EnSw* self, PlayState* play, s32 arg2);
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
DamageTable* DamageTable_Get(s32 index);
extern u8 EnSw_sJntSphInit[];
extern CollisionCheckInfoInit2 EnSw_sColChkInfoInit;

void EnSw_Init(Actor* thisx, PlayState* play) {
    EnSw* self = (EnSw*)thisx;
    s32 phi;

    if (thisx->params & 0x8000) {
        phi = ((u32)(thisx->params - 0x8000) >> 13) + 1;
        thisx->params = (thisx->params & ~0xE000) | (phi << 13);
    }

    if ((thisx->params & 0xE000) != 0) {
        phi = ((thisx->params & 0x1F00) >> 8) - 1;
        thisx->params = (thisx->params & ~0x1F00) | (phi << 8);
    }

    *(u8*)((u8*)self + 0x82C) = (u8)thisx->params;
    if ((thisx->params &
         ((*(u32*)((u8*)gSaveContext.gsFlags + ((thisx->params >> 8) & 0x1C)) & gGsFlagsMask[(thisx->params >> 6) & 3]) >>
              gGsFlagsShift[(thisx->params >> 6) & 3] &
          0xFF)) != 0) {
        Actor_Kill(thisx);
        return;
    }

    FUN_00372f38(thisx, play, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, (thisx->params & 0xE000) != 0, 0, self->jointTable, self->morphTable,
                 0xB);
    FUN_003717ac(&self->skelAnime, EnSw_sAnimInfo, 0);
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, thisx, EnSw_sJntSphInit, self->unk_6C4 + 0);
    CollisionCheck_SetInfo2(&thisx->colChkInfo, DamageTable_Get(0xE), &EnSw_sColChkInfoInit);
    thisx->scale.x = 0.02f;

    if ((thisx->params & 0xE000) == 0) {
        thisx->world.rot.x = 0;
        thisx->world.rot.z = 0;
        thisx->shape.rot = thisx->world.rot;
        self->unk_7E8[4].y = thisx->world.pos.y;
        self->unk_7E8[4].x = thisx->world.pos.x + (Math_SinS(thisx->world.rot.y) * -60.0f);
        self->unk_7E8[4].z = thisx->world.pos.z + (Math_CosS(thisx->world.rot.y) * -60.0f);
        FUN_0035f324(self, play);
        thisx->home.pos = thisx->world.pos;
    } else {
        self->unk_728.x = Math_SinS(thisx->shape.rot.y + 0x4000);
        self->unk_728.y = 0.0f;
        self->unk_728.z = Math_CosS(thisx->shape.rot.y + 0x4000);
        self->unk_71C.x = 0.0f;
        self->unk_71C.y = 1.0f;
        self->unk_71C.z = 0.0f;
        self->unk_734.x = Math_SinS(thisx->shape.rot.y);
        self->unk_734.y = 0.0f;
        self->unk_734.z = Math_CosS(thisx->shape.rot.y);
        FUN_0034ecb0(self, play, 1);
    }

    if (((thisx->params >> 13) & 7) >= 3) {
        Audio_PlaySoundGeneral(0x01000487, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
    }

    switch ((thisx->params >> 13) & 7) {
        case 3:
        case 4:
            self->unk_718 = 1;
            thisx->velocity.y = 8.0f;
            thisx->speedXZ = 4.0f;
            thisx->gravity = -1.0f;
        case 2:
            thisx->scale.x = 0.0f;
        case 1:
            ((u8*)self->collider.elements)[5] *= 2;
            thisx->naviEnemyId = 0x20;
            thisx->colChkInfo.health *= 2;
            thisx->flags &= ~1;
            break;
        default:
            Actor_ChangeType(play, &play->actorCtx, thisx, 5);
            thisx->naviEnemyId = 0x1F;
            break;
    }

    self->unk_746 = Rand_S16Offset_003702c8(0xF, 0x1E);
    Actor_SetScale(thisx, thisx->scale.x);
    thisx->home.pos = thisx->world.pos;
    thisx->shape.rot = thisx->world.rot;
    self->unk_824 = 1.0f;

    if (((thisx->params >> 13) & 7) >= 3) {
        self->unk_744 = 0x3C;
        self->unk_74C = 1;
        self->actionFunc = (EnSwActionFunc)FUN_0011770c;
    } else if ((thisx->params & 0xE000) == 0) {
        self->actionFunc = (EnSwActionFunc)FUN_003cdaac;
    } else {
        self->actionFunc = (EnSwActionFunc)FUN_0012ba6c;
    }
}
#endif

// Landing after the fall (N64 func_80B0DB00 shape): reset the pose, roll timer from the anim length.
void FUN_003ac31c(Actor* thisx, PlayState* play) {
    EnSw* self = (EnSw*)thisx;

    Actor_MoveForward(thisx);
    thisx->shape.rot.x += 0x1000;
    thisx->shape.rot.z += 0x1000;
    Math_SmoothStepToF(&thisx->shape.yOffset, 600.0f, 0.4f, 10000.0f, 0.0f);
    Actor_UpdateBgCheckInfo(play, thisx, 20.0f, 20.0f, 0.0f, 5);

    if (thisx->bgCheckFlags & 1) {
        if (thisx->velocity.y >= 0.0f) {
            return;
        }
        if (thisx->floorHeight <= -32000.0f || thisx->floorHeight >= 32000.0f) {
            Actor_Kill(thisx);
            return;
        }

        {
            Actor* s2 = thisx;
            s2->bgCheckFlags &= ~1;
        }
        FUN_003717ac(&self->skelAnime, EnSw_sAnimInfo, 3);
        thisx->shape.rot.x = 0;
        thisx->shape.rot.y = 0;
        thisx->shape.rot.z = 0;
        thisx->speedXZ = 0.0f;
        thisx->velocity.y = 0.0f;
        {
            f32 n = self->skelAnime.animLength;
            f32 a, b;
            s32 t;

            b = 0.5f;
            a = 1.5f;
            if (n > 0.0f) {
                t = (s32)(b + n * a);
            } else {
                t = (s32)(n * a - b);
            }
            self->unk_74C = t;
        }
        ((EnSw*)thisx)->actionFunc = (EnSwActionFunc)FUN_0015f738;
        Audio_PlayActorSound2(thisx, 0x010002EB);
        FUN_0036f00c(play, thisx, &thisx->world.pos, 16.0f, 12, 2.0f, 120, 10, 0);
    }
}

ActorInit En_Sw_InitVars = {
    ACTOR_EN_SW,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ST,
    sizeof(EnSw),
    (ActorFunc)EnSw_Init,
    (ActorFunc)EnSw_Destroy,
    (ActorFunc)EnSw_Update,
    (ActorFunc)EnSw_Draw,
};

GLOBAL_ASM("asm/EnSw_Init.s")

void EnSw_Destroy(Actor* thisx, PlayState* play) {
    EnSw* self = (EnSw*)thisx;

    Collider_DestroyJntSph(play, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(thisx, 0);
}

GLOBAL_ASM("asm/EnSw_Update.s")

typedef struct {
    f32 r, g, b, a;
} EnSwColor;
extern EnSwColor EnSw_sFogColor;
void FUN_00357fd0(Player* player, void* arg1, Vec3f* pos);
void FUN_00357388(Actor* actor, EnSwColor* color, s32 arg2, s32 arg3);
void FUN_00341c18(void* arg0, s32 arg1);
void FUN_00342be0(Actor* actor, s32 a, s32 b, f32 c, f32 d, f32 e, f32 f);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_001c31c0(PlayState* play, s32 limbIndex, MTX34* mtx, void* self);
s32 FUN_001e29e4(PlayState* play, s32 limbIndex, MTX34* mtx, void* self);

static inline void EnSw_SetVec(Vec3f* v, f32 x, f32 z, f32 y) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void EnSw_Draw(Actor* thisx, PlayState* play) {
    EnSw* self = (EnSw*)thisx;

    if (thisx->scale.x != 0.0f) {
        FUN_00357fd0(GET_PLAYER(play), thisx->unk_178, &thisx->world.pos);
        s32 pp = thisx->params & 0xE000;
        EnSwActionFunc chase = (EnSwActionFunc)FUN_0012cdb0;
        if (pp != 0) {
            Matrix_RotateX(&thisx->modelMtx, 1, -1.5707964f);
            if (thisx->colChkInfo.health != 0) {
                MTX34* m = &thisx->modelMtx;
                Vec3f t;
                EnSw_SetVec(&t, 0.0f, 200.0f, 0.0f);

                Matrix_UpdatePosition(m, m, &t);
            }
        } else if (self->actionFunc == chase) {
            EnSwColor color = EnSw_sFogColor;

            FUN_00357388(thisx, &color, 5, 0);
        } else if (*(u16*)((u8*)thisx + 0x11A) == 0) {
            FUN_00341c18(thisx->unk_178, 5);
        }
        FUN_00342be0(thisx, 4, 2, 0.0f, 0.0f, 0.0f, self->unk_824);
        self->unk_780 = thisx->modelMtx;
        SkelAnime_DrawOpa(&self->skelAnime, &thisx->modelMtx, (OverrideLimbDrawOpa)FUN_001e29e4,
                          (PostLimbDrawOpa)FUN_001c31c0, thisx, 0);
    }
}

GLOBAL_ASM("asm/FUN_003415a8.s")

}
