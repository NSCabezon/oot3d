extern "C" {
#include "z_en_po_sisters.hpp"

#define FLAGS 0x00005215

void FUN_00373d0c(PlayState* play);
s32 LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);
void FUN_0035046c(void* p);
void FUN_00350f34(Actor* actor, ...);
void FUN_00366c24(EnPoSisters* self, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);
s16 Rand_S16Offset_003702c8(s16 base, s16 range);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
s32 Animation_OnFrameImpl(SkelAnime* skelAnime, f32 frame, f32 updateRate);
s32 FUN_003731e0(SkelAnime* skelAnime);
void FUN_003ca1e4(EnPoSisters* self, PlayState* play);
void FUN_00107f3c(EnPoSisters* self, PlayState* play);

void EnPoSisters_Init(Actor* thisx, PlayState* play);
void EnPoSisters_Destroy(Actor* thisx, PlayState* play);
void EnPoSisters_Update(Actor* thisx, PlayState* play);
void EnPoSisters_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Po_Sisters.data.s")

ActorInit En_Po_Sisters_InitVars = {
    ACTOR_EN_PO_SISTERS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_PO_SISTERS,
    sizeof(EnPoSisters),
    (ActorFunc)EnPoSisters_Init,
    (ActorFunc)EnPoSisters_Destroy,
    (ActorFunc)EnPoSisters_Update,
    (ActorFunc)EnPoSisters_Draw,
};

// Init draft, 101 words off (size 1152 vs 1148: armcc hoists the shared 1.5f of the two Animation_Change calls into s17, ROM loads it at each call;
// everything else incl. the r8/r4 thisx/self split and the 16-byte stack table matches). Needs EQUs in data_syms.s (sInitChain/sCylinderInit/
// sColChkInfoInit) and the table at 0x4D9B24 pinned as the TU .constdata. GLOBAL_ASM kept.
#if 0
void Actor_ProcessInitChain(Actor*, InitChainEntry*);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* lightInfo);
void Lights_PointGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
void Animation_PlayOnce(SkelAnime*, s32);
void FUN_0034f94c(void* p, PlayState* play, Actor* actor, s32 a);
void FUN_0019a088(EnPoSisters*, PlayState*);
void FUN_0013f280(EnPoSisters*, PlayState*);
void FUN_0010b218(EnPoSisters*, PlayState*);
extern InitChainEntry EnPoSisters_sInitChain[];
extern ColliderCylinderInit EnPoSisters_sCylinderInit;
extern CollisionCheckInfoInit2 EnPoSisters_sColChkInfoInit;
typedef struct { s32 v[4]; } EnPoSistersIdx;
void EnPoSisters_Init(Actor* thisx, PlayState* play) {
    EnPoSisters* self;
    s32 i;
    EnPoSistersIdx sIdx;
    Actor* sp1;
    Actor* sp2;
    Actor* sp3;

    Actor_ProcessInitChain(thisx, EnPoSisters_sInitChain);
    self = (EnPoSisters*)thisx;
    ActorShape_Init(&thisx->shape, 0.0f, (ActorShadowFunc)ActorShadow_DrawCircle, 50.0f);
    FUN_00372f38(thisx, play, &self->unk_978, 6, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 0, (u8*)self + 0x228, (u8*)self + 0x5D0, 18);
    ((u8*)self)[0x9EA] = 255;
    ((u8*)self)[0x9EB] = 255;
    ((u8*)self)[0x9EC] = 153;
    self->unk_9ED = 255;
    self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->lightInfo);
    Lights_PointGlowSetInfo(&self->lightInfo, self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z, 0, 0, 0, 0, 0);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &EnPoSisters_sCylinderInit);
    CollisionCheck_SetInfo(&thisx->colChkInfo, (DamageTable*)((u8*)&EnPoSisters_sColChkInfoInit + 0x70), (CollisionCheckInfoInit*)&EnPoSisters_sColChkInfoInit);
    {
    EnPoSisters* s2 = (EnPoSisters*)thisx;
    s2->unk_194 = (thisx->params >> 8) & 3;
    s2->actor.naviEnemyId = s2->unk_194 + 0x50;
    s2->unk_195 = (thisx->params >> 10) & 3;
    s2->unk_A50 = 110.0f;
    s2->unk_196 = 0x30;
    s2->unk_197 = 0x1E;
    s2->unk_198 = 1;
    s2->unk_199 = 0x20;
    { Actor* t2 = thisx; t2->flags &= ~1; }
    }
    if (thisx->params & 0x1000) {
        *(s32*)((u8*)play + 0x7F4C) = 0;
        self->unk_9ED = 0;
        self->unk_199 = 0x80;
        self->unk_19A = 0x4B;
        *(Vec3f*)((u8*)self + 0x9F0) = self->actor.home.pos;
        Actor_ChangeType(play, &play->actorCtx, &self->actor, 6);
        self->actionFunc = (EnPoSistersActionFunc)FUN_0019a088;
    } else if (self->unk_194 == 0) {
        if (self->unk_195 == 0) {
            self->collider.base.ocFlags1 = 9;
            sp1 = Actor_Spawn(&play->actorCtx, play, 0x91, self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0x400, 1);
            sp2 = Actor_Spawn(&play->actorCtx, play, 0x91, self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0x800, 1);
            sp3 = Actor_Spawn(&play->actorCtx, play, 0x91, self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0xC00, 1);
            if (sp1 == NULL || sp2 == NULL || sp3 == NULL) {
                if (sp1 != NULL) {
                    Actor_Kill(sp1);
                }
                if (sp2 != NULL) {
                    Actor_Kill(sp2);
                }
                if (sp3 != NULL) {
                    Actor_Kill(sp3);
                }
                Actor_Kill(&self->actor);
            } else {
                sp3->parent = &self->actor;
                sp2->parent = &self->actor;
                sp1->parent = &self->actor;
                Animation_Change_00375c08(&self->skelAnime, 6, 1.5f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 6), 0, 0.0f);
                self->unk_198 = 0;
                self->unk_199 = 0xA0;
                self->actionFunc = (EnPoSistersActionFunc)FUN_0013f280;
            }
            self->actor.unk_19B = 4;
        } else {
            self->actor.flags &= ~0x4200;
            self->collider.info.elemType = 4;
            self->collider.info.bumper.dmgFlags |= 1;
            self->collider.base.ocFlags1 = 0;
            FUN_00370170(self, NULL);
        }
    } else {
        if (self->unk_194 == 3) {
            Animation_PlayOnce(&self->skelAnime, 1);
            Audio_PlayActorSound2(&self->actor, 0x010002E3);
        } else {
            Animation_Change_00375c08(&self->skelAnime, 1, 1.5f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 3, 0.0f);
        }
        self->unk_9ED = 0;
        self->unk_199 = 0x20;
        self->actionFunc = (EnPoSistersActionFunc)FUN_0010b218;
    }
    thisx->params &= 0x3F;
    {
        static const EnPoSistersIdx k = { { 8, 0, 1, 2 } };
        sIdx = k;
        for (i = 0; i < 8; i++) {
            FUN_0034f94c(&self->unk_97C[i], play, thisx, sIdx.v[self->unk_194]);
        }
    }
}
#endif

GLOBAL_ASM("asm/EnPoSisters_Init.s")

void EnPoSisters_Destroy(Actor* thisx, PlayState* play) {
    EnPoSisters* self = (EnPoSisters*)thisx;
    s32 i;

    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);
    if (self->unk_194 == 0 && self->unk_195 == 0) {
        FUN_00373d0c(play);
    }
    Collider_DestroyCylinder(play, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(thisx, &self->unk_978, 0);
    for (i = 0; i < 8; i++) {
        FUN_0035046c(&self->unk_97C[i]);
    }
}

void FUN_0036efa8(EnPoSisters* self);

void FUN_003e7a78(EnPoSisters* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && self->unk_19A != 0) {
        self->unk_19A--;
    }
    if (self->unk_19A == 0 || self->actor.xzDistToPlayer < 200.0f) {
        FUN_0036efa8(self);
    }
}

void FUN_0011def4(EnPoSisters* self, PlayState* play);

void FUN_001c68f0(EnPoSisters* self) {
    self->actor.speedXZ = 5.0f;
    if (self->unk_194 == 0) {
        self->collider.base.colType = 9;
        self->collider.base.acFlags |= AC_HARD;
        Animation_MorphToLoop(&self->skelAnime, 3, -5.0f);
    }
    self->unk_19A = 8;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    self->unk_199 |= 8;
    self->actionFunc = (EnPoSistersActionFunc)FUN_0011def4;
}

void Matrix_RotateX(MTX34* mtx, f32 angle, s32 mode);

s32 FUN_00389e64(PlayState* play, s32 limbIndex, MTX34* mtx, EnPoSisters* self) {
    if (limbIndex == 1 && (self->unk_199 & 0x40)) {
        if (self->unk_19A >= 0x1AA) {
            s16 t = self->unk_19A + 6;
            f32 angle = (f32)(s16)(t * 0x1000) * (3.1415927f / 0x8000);
            Matrix_RotateX(mtx, angle, 1);
        } else {
            s32 t = (s32)((u32)self->unk_19A << 28);
            f32 angle = (f32)((t + (s32)0x90000000) >> 16) * (3.1415927f / 0x8000);
            Matrix_RotateX(mtx, angle, 1);
        }
    }
    return 0;
}

s32 Math_ScaledStepToS(s16* pValue, s16 target, s16 scale);
s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void OnePointCutscene_Init(PlayState* play, s32 csId, s32 unk2, void* actor, s32 unk4);
void FUN_00178488(EnPoSisters* self, PlayState* play);
void FUN_001e8ddc(EnPoSisters* self, PlayState* play);
extern __pure float _fabsf(float);
static __inline __pure float PoSisters_Fabsf(float f) {
    return _fabsf(f);
}


void FUN_0020627c(EnPoSisters* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) < 10.0f) {
        Animation_MorphToLoop(&self->skelAnime, 0, -3.0f);
        self->actor.speedXZ = 0.0f;
        self->unk_9ED = 0xFF;
        self->unk_199 |= 0x15;
        self->actor.flags |= 1;
        self->actionFunc = (EnPoSistersActionFunc)FUN_001e8ddc;
    } else {
        Math_ScaledStepToS(&self->actor.world.rot.y, Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos),
                           0x71C);
    }
}

void FUN_0013f280(EnPoSisters* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->actor.xzDistToPlayer < 130.0f) {
        Animation_MorphToPlayOnce(&self->skelAnime, 1, -5.0f);
        self->unk_198 = 1;
        self->unk_199 &= ~0x80;
        self->actionFunc = (EnPoSistersActionFunc)FUN_00178488;
        {
            s32 two = 0x9C;
            OnePointCutscene_Init(play, two + 0xBD0, two, &self->actor, 0);
        }
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x0100035D);
    }
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
}


void FUN_00175378(EnPoSisters* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        self->unk_9ED = 0;
        self->collider.info.bumper.dmgFlags = 0x00060001;
        FUN_0036efa8(self);
        return;
    }
    s32 end = (s32)*EnPoSisters_EndFramePtr(&self->skelAnime);
    self->unk_9ED = (u8)(((f32)end - *EnPoSisters_CurFramePtr(&self->skelAnime)) * 255.0f / (f32)end);
}

void FUN_001e8ddc(EnPoSisters* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0x71C);
    if (self->actor.xzDistToPlayer < 240.0f && PoSisters_Fabsf(self->actor.yDistToPlayer + 5.0f) < 30.0f) {
        if (self->actionFunc != (EnPoSistersActionFunc)FUN_00107f3c) {
            Animation_MorphToLoop(&self->skelAnime, 2, -3.0f);
        }
        self->unk_19A = Rand_S16Offset_003702c8(0xF, 3);
        self->unk_199 |= 7;
        self->actionFunc = (EnPoSistersActionFunc)FUN_003ca1e4;
    }
}

GLOBAL_ASM("asm/EnPoSisters_Update.s")

GLOBAL_ASM("asm/EnPoSisters_Draw.s")

void FUN_0011b2bc(EnPoSisters* self, PlayState* play);

void FUN_0034c050(EnPoSisters* self, PlayState* play) {
    Animation_Change_00375c08(&self->skelAnime, 1, 1.5f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 2,
                              -3.0f);
    if (self->unk_194 == 0) {
        self->unk_A50 = 110.0f;
        FUN_00366c24(self, play);
        self->unk_9ED = 0;
        self->actor.draw = (ActorFunc)EnPoSisters_Draw;
    } else {
        self->actor.world.rot.y = self->actor.shape.rot.y;
    }
    self->actor.speedXZ = 0.0f;
    self->unk_19A = 23;
    Audio_PlayActorSound2(&self->actor, 0x010002E3);
    self->unk_199 &= ~1;
    self->actionFunc = (EnPoSistersActionFunc)FUN_0011b2bc;
}


// G101 batch 2 prototypes
void FUN_003464b8(EnPoSisters* self, s32 val, Vec3f* center);
void FUN_001e7fc8(EnPoSisters* self, PlayState* play);
void FUN_00199aa8(EnPoSisters* self, PlayState* play);
void FUN_00175378(EnPoSisters* self, PlayState* play);
void FUN_003667ec(EnPoSisters* self, PlayState* play);
void FUN_00121e10(EnPoSisters* self, PlayState* play);
s32 Player_InCsMode(PlayState* play);

void FUN_003464b8(EnPoSisters* self, s32 val, Vec3f* center) {
    f32 amp = (f32)(val * val) * 0.1f;
    s32 i = 0;

    s32 n = self->unk_198;
    if (n > 0) {
        u8* s2 = (u8*)self + 0x900;
        do {
            Vec3f* out = (Vec3f*)((u8*)self + 0x9F0 + i * 12);
            u16 rot = self->actor.shape.rot.y;
            s16 ang;
            s32 a;
            if (*(s16*)(s2 + 0xE6) > 0) {
                a = (s32)(((f32)*(s16*)(s2 + 0xE6) * 2.0f) * (1.0f / 3.0f) + 0.5f);
            } else {
                a = (s32)(((f32)*(s16*)(s2 + 0xE6) * 2.0f) * (1.0f / 3.0f) - 0.5f);
            }
            ang = (s16)((a << 11) + (i << 13) + rot);
            out->x = center->x + amp * Math_SinS(ang);
            rot = self->actor.shape.rot.y;
            if (*(s16*)(s2 + 0xE6) > 0) {
                a = (s32)(((f32)*(s16*)(s2 + 0xE6) * 2.0f) * (1.0f / 3.0f) + 0.5f);
            } else {
                a = (s32)(((f32)*(s16*)(s2 + 0xE6) * 2.0f) * (1.0f / 3.0f) - 0.5f);
            }
            ang = (s16)((a << 11) + (i << 13) + rot);
            out->z = center->z + amp * Math_CosS(ang);
            out->y = center->y + (f32)val;
            i++;
        } while (self->unk_198 > i);
    }
}

void FUN_00199aa8(EnPoSisters* self, PlayState* play) {
    s32 r;
    f32 c_two = 2.0f;
    f32 c_third = 1.0f / 3.0f;
    f32 c_half = 0.5f;
    self->unk_19A = (s16)(self->unk_19A + 1);
    if (self->unk_19A > 0) {
        r = (s32)(((f32)self->unk_19A * c_two) * c_third + c_half);
    } else {
        r = (s32)(((f32)self->unk_19A * c_two) * c_third - c_half);
    }
    FUN_003464b8(self, r, &self->actor.home.pos);
    if (self->unk_19A == 0x30) {
        Animation_Change_00375c08(&self->skelAnime, 1, 0.833f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 3, 0.0f);
        self->unk_19A = (self->unk_194 == 0 || self->unk_194 == 1) ? 0x3C : 0x72;
        self->unk_198 = 0;
        *(s32*)((u8*)play + 0x7F4C) = 0;
        self->actionFunc = (EnPoSistersActionFunc)FUN_001e7fc8;
    }
}

void FUN_0019a088(EnPoSisters* self, PlayState* play) {
    if (*(s32*)((u8*)play + 0x7F4C) != 0 || !Player_InCsMode(play)) {
        if (self->unk_19A != 0) {
            self->unk_19A--;
            if (self->unk_19A == 0x2D) {
                if (self->unk_194 == 0) {
                    OnePointCutscene_Init(play, 0xC44, 0x3E7, NULL, 0);
                }
                *(s32*)((u8*)play + 0x7F4C) = 1;
            }
        }
        if (self->unk_19A == 0) {
            self->unk_198 = 8;
            *(Vec3f*)((u8*)self + 0x9FC) = *(Vec3f*)((u8*)self + 0x9F0);
            *(Vec3f*)((u8*)self + 0xA08) = *(Vec3f*)((u8*)self + 0x9F0);
            *(Vec3f*)((u8*)self + 0xA14) = *(Vec3f*)((u8*)self + 0x9F0);
            *(Vec3f*)((u8*)self + 0xA20) = *(Vec3f*)((u8*)self + 0x9F0);
            *(Vec3f*)((u8*)self + 0xA2C) = *(Vec3f*)((u8*)self + 0x9F0);
            *(Vec3f*)((u8*)self + 0xA38) = *(Vec3f*)((u8*)self + 0x9F0);
            *(Vec3f*)((u8*)self + 0xA44) = *(Vec3f*)((u8*)self + 0x9F0);
            self->actionFunc = (EnPoSistersActionFunc)FUN_00199aa8;
        }
    }
    FUN_00373264(&self->actor, 0x01000188);
}

void FUN_00366c24(EnPoSisters* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 amp;
    if (self->unk_195 != 0 && self->actionFunc == (EnPoSistersActionFunc)FUN_00121e10) {
        amp = self->actor.parent->xzDistToPlayer;
    } else {
        u8* pb = (u8*)player + 0x2200;
        if ((pb[0x27] == 0 || *(s8*)&pb[0x26] >= 0x18) && (player->actor.world.pos.y - player->actor.floorHeight) < 1.0f) {
            Math_StepToF(&self->unk_A50, 110.0f, 3.0f);
        } else {
            Math_StepToF(&self->unk_A50, 170.0f, 10.0f);
        }
        amp = self->unk_A50;
    }
    self->actor.world.pos.x = player->actor.world.pos.x + amp * Math_SinS(self->actor.shape.rot.y + 0x8000);
    self->actor.world.pos.z = player->actor.world.pos.z + amp * Math_CosS(self->actor.shape.rot.y + 0x8000);
}

void FUN_0011b2bc(EnPoSisters* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        self->unk_9ED = 0xFF;
        if (self->unk_194 != 0) {
            self->unk_199 |= 1;
            self->collider.info.bumper.dmgFlags = 0x4FC7FFEA;
            if (self->unk_19A != 0) {
                self->unk_19A--;
                if (self->unk_19A != 0) {
                    return;
                }
            }
            self->unk_197 = 0x1E;
            FUN_0036efa8(self);
        } else {
            Animation_MorphToLoop(&self->skelAnime, 2, -3.0f);
            self->unk_9ED = 0xFF;
            self->unk_19A = 0x1C2;
            self->unk_19C = 3;
            self->unk_199 |= 9;
            self->actor.flags |= 1;
            self->actionFunc = (EnPoSistersActionFunc)FUN_003667ec;
        }
    } else {
        self->unk_9ED = (u8)(*EnPoSisters_CurFramePtr(&self->skelAnime) * 255.0f / *EnPoSisters_EndFramePtr(&self->skelAnime));
        if (self->unk_194 == 0) {
            FUN_00366c24(self, play);
        }
    }
}

void FUN_00124860(EnPoSisters* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.world.rot.y, (s16)(self->actor.yawTowardsPlayer + 0x8000), 0x71C);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && self->unk_19A != 0) {
        self->unk_19A--;
    }
    if (self->actor.bgCheckFlags & 8) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        self->unk_199 |= 2;
        Animation_Change_00375c08(&self->skelAnime, 1, 1.5f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 1), 2, -3.0f);
        self->actor.speedXZ = 0.0f;
        self->unk_19C = 0x96;
        self->actor.world.rot.y = self->actor.shape.rot.y;
        self->unk_199 &= ~5;
        Audio_PlayActorSound2(&self->actor, 0x010002E4);
        Audio_PlayActorSound2(&self->actor, 0x0100035F);
        self->actionFunc = (EnPoSistersActionFunc)FUN_00175378;
    } else if (self->unk_19A == 0) {
        if (self->actor.xzDistToPlayer > 240.0f) {
            self->actor.world.rot.y = self->actor.shape.rot.y;
            FUN_0036efa8(self);
        }
    }
}


void FUN_0020584c(EnPoSisters* self, PlayState* play);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);

void FUN_001e7fc8(EnPoSisters* self, PlayState* play) {
    self->unk_19A--;
    if (self->unk_19A == 0) {
        Audio_PlayActorSound2(&self->actor, 0x010002E3);
        self->unk_199 &= ~0x80;
    }
    if (self->unk_19A <= 0) {
        if (FUN_003731e0(&self->skelAnime)) {
            self->unk_9ED = 0xFF;
            *(s32*)((u8*)play + 0x7F4C) |= 1 << self->unk_194;
        } else {
            self->unk_9ED = (u8)(*EnPoSisters_CurFramePtr(&self->skelAnime) * 255.0f / *EnPoSisters_EndFramePtr(&self->skelAnime));
        }
    }
    if (*(s32*)((u8*)play + 0x7F4C) == 0xF) {
        Animation_PlayLoop(&self->skelAnime, 0);
        self->unk_198 = 8;
        self->unk_19A = 0x30;
        {
            s32 k = 0x20;
            FUN_003464b8(self, k, &self->actor.home.pos);
        }
        self->actionFunc = (EnPoSistersActionFunc)FUN_0020584c;
    }
}

GLOBAL_ASM("../src/game/actors/En_Po_Sisters/data_syms.s")
extern u16 D_EnPoSisters_RotTbl[];
void FUN_001e80d4(EnPoSisters* self, PlayState* play);
void Flags_SetSwitch(PlayState* play, s32 flag);

void FUN_0020584c(EnPoSisters* self, PlayState* play) {
    f32 c_third, c_half, c_two;
    FUN_003731e0(&self->skelAnime);
    c_two = 2.0f;
    c_third = 1.0f / 3.0f;
    c_half = 0.5f;
    if (self->unk_19A != 0) {
        self->unk_19A--;
    }
    f32 d = (f32)self->unk_19A * c_two;
    s32 r = (s32)((self->unk_19A > 0) ? c_half + d * c_third : d * c_third - c_half);
    FUN_003464b8(self, r, &self->actor.home.pos);
    if (self->unk_19A == 0) {
        Animation_MorphToLoop(&self->skelAnime, 2, -3.0f);
        self->unk_198 = 0;
        self->unk_199 = 0x28;
        self->unk_19A = 0x87;
        self->unk_196 = 0x30;
        self->actor.world.rot.y = D_EnPoSisters_RotTbl[self->unk_194];
        self->actor.home.pos.y = self->actor.world.pos.y;
        if (self->unk_194 == 0) {
            Flags_SetSwitch(play, 0x1B);
        }
        Audio_PlayActorSound2(&self->actor, 0x01000179);
        self->actionFunc = (EnPoSistersActionFunc)FUN_001e80d4;
    }
}

// Unmatched drafts, batch 2 (GLOBAL_ASM kept in src/functions; see STATUS.md)
// FUN_00106074 draft, 17 words off (VFP numbering of the rounding constants; layout and size match). GLOBAL_ASM kept.
#if 0
void FUN_00106074(EnPoSisters* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    f32 sp10 = self->actor.speedXZ * 10.0f;
    f32 test = self->actor.speedXZ * 1280.0f;
    f32 r = sp10 * 256.0f;
    s32 d = (s32)((test > 0.0f) ? 0.5f + r * (1.0f / 3.0f) : r * (1.0f / 3.0f) - 0.5f);
    self->actor.shape.rot.y -= d;
    if (Math_StepToF(&self->actor.speedXZ, 0.0f, 0.1f) != 0) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (self->unk_194 != 0) {
            if (self->actionFunc != (EnPoSistersActionFunc)FUN_00107f3c) {
                Animation_MorphToLoop(&self->skelAnime, 2, -3.0f);
            }
            self->unk_19A = Rand_S16Offset_003702c8(0xF, 3);
            self->unk_199 |= 7;
            self->actionFunc = (EnPoSistersActionFunc)FUN_003ca1e4;
        } else {
            Audio_PlayActorSound2(&self->actor, 0x0100035F);
            FUN_00370170(self, play);
        }
    }
}
#endif

// FUN_003ca1e4 = func_80ADA530 (N64): float state, speed step-down, leave to FUN_00107f3c or FUN_003e7a78.
void FUN_003ca1e4(EnPoSisters* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    Math_StepToF(&self->actor.speedXZ, 1.0f, 0.2f);
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && self->unk_19A != 0) {
        self->unk_19A--;
    }
    if (self->actor.xzDistToPlayer < 200.0f && PoSisters_Fabsf(self->actor.yDistToPlayer + 5.0f) < 30.0f) {
        self->actionFunc = (EnPoSistersActionFunc)FUN_00107f3c;
    } else if (self->unk_19A == 0 && Math_StepToF(&self->actor.speedXZ, 0.0f, 0.2f) != 0) {
        Animation_MorphToLoop(&self->skelAnime, 0, -3.0f);
        self->unk_19A = Rand_S16Offset_003702c8(2, 3);
        self->actor.speedXZ = 0.0f;
        self->actionFunc = (EnPoSistersActionFunc)FUN_003e7a78;
    }
    if (self->actor.bgCheckFlags & 8) {
        Math_ScaledStepToS(&self->actor.world.rot.y, Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos),
                           0x71C);
    } else if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) > 300.0f) {
        Math_ScaledStepToS(&self->actor.world.rot.y, Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos),
                           0x71C);
    }
}
// FUN_003dc3e0 = func_80ADB17C (N64): switch-flag state, orbit shrink, then moves to the table position at 0x30.
// D_80ADD7A4 (Vec3s[4], 0x0052E088) lives in the data chunk; EQU in data_syms.s.
extern Vec3s D_80ADD7A4[];
void Flags_UnsetSwitch(PlayState* play, s32 flag);
void FUN_00372244(void* arg0, u16 arg1, u32 sfxId);
void Audio_PlaySoundAtPosition(PlayState* play, Vec3f* pos, s32 duration, u32 sfxId);
void Actor_Kill(Actor* actor);

void FUN_003dc3e0(EnPoSisters* self, PlayState* play) {
    s32 r;

    self->unk_19A = (s16)(self->unk_19A + 1);
    if (self->unk_19A == 0x60) {
        Flags_SetSwitch(play, self->actor.params);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 0x1E, 0x01000179);
        if (self->unk_194 == 0) {
            Flags_UnsetSwitch(play, 0x1B);
        }
        *(u8*)((u8*)play + 0x3237) = 0xFF;
        FUN_00372244((u8*)play + 0x5FCC, 0x1E, 0x01000487);
        Actor_Kill(&self->actor);
    } else {
        f32 c_half;
        f32 c_third;
        f32 c_two;

        c_two = 2.0f;
        c_third = 1.0f / 3.0f;
        c_half = 0.5f;
        if (self->unk_19A < 0x30) {
            if (self->unk_19A > 0) {
                r = (s32)(((f32)self->unk_19A * c_two) * c_third + c_half);
            } else {
                r = (s32)(((f32)self->unk_19A * c_two) * c_third - c_half);
            }
            FUN_003464b8(self, r, &self->actor.world.pos);
        } else {
            r = 0x40 - (s32)(((f32)self->unk_19A * c_two) * c_third + c_half);
            FUN_003464b8(self, r, &self->actor.world.pos);
        }
    }
    if (self->unk_19A == 0x30) {
        self->actor.world.pos.x = D_80ADD7A4[self->unk_194].x;
        self->actor.world.pos.y = D_80ADD7A4[self->unk_194].y;
        self->actor.world.pos.z = D_80ADD7A4[self->unk_194].z;
    }
}
// FUN_00178488 = N64 func_80AD9E60 (float timer) + parent/own-yaw branch + sfx; then orbit FUN_00366c24.
void Actor_SetFocus(Actor* actor, f32 height);
void FUN_0019c118(EnPoSisters* self, PlayState* play);
void FUN_00178488(EnPoSisters* self, PlayState* play) {
    s32 r;

    if (FUN_003731e0(&self->skelAnime)) {
        Animation_MorphToLoop(&self->skelAnime, 2, -3.0f);
        if ((Animation_GetLastFrame(&self->skelAnime, 2) + 1) * 7 > 0) {
            r = (s32)(0.5f + ((f32)((Animation_GetLastFrame(&self->skelAnime, 2) + 1) * 7) * 3.0f) * 0.5f);
        } else {
            r = (s32)(((f32)((Animation_GetLastFrame(&self->skelAnime, 2) + 1) * 7) * 3.0f) * 0.5f - 0.5f);
        }
        self->unk_19A = (s16)r;
        if (self->actor.parent != NULL) {
            self->actor.world.pos = self->actor.parent->world.pos;
            self->actor.shape.rot.y = self->actor.parent->shape.rot.y;
        } else {
            self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
            self->unk_19A++;
        }
        if (self->unk_195 == 0) {
            Audio_PlayActorSound2(&self->actor, 0x0100035F);
        }
        self->actionFunc = (EnPoSistersActionFunc)FUN_0019c118;
    }
    FUN_00366c24(self, play);
    self->actor.world.pos.y += 1.0f;
    Actor_SetFocus(&self->actor, 40.0f);
}
// FUN_0010b218 = func_80ADB9F0 (N64): appear/finish state; unk_194 == 3 jumps to the float state FUN_003ca1e4,
// others to the idle state FUN_0020627c.
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void FUN_0010b218(EnPoSisters* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        self->unk_9ED = 0xFF;
        if (self->unk_194 == 3) {
            self->actor.flags |= 1;
            self->actor.home.pos.x = 1992.0f;
            self->actor.home.pos.z = -1440.0f;
            self->unk_199 |= 0x18;
            if (self->actionFunc != (EnPoSistersActionFunc)FUN_00107f3c) {
                Animation_MorphToLoop(&self->skelAnime, 2, -3.0f);
            }
            self->unk_19A = Rand_S16Offset_003702c8(0xF, 3);
            self->unk_199 |= 7;
            self->actionFunc = (EnPoSistersActionFunc)FUN_003ca1e4;
        } else {
            if (self->unk_194 == 1) {
                self->actor.home.pos.x = -632.0f;
                self->actor.home.pos.z = -3440.0f;
            } else {
                self->actor.home.pos.x = 752.0f;
                self->actor.home.pos.z = -3440.0f;
            }
            Animation_PlayLoop(&self->skelAnime, 2);
            self->unk_199 |= 0xA;
            self->actor.speedXZ = 5.0f;
            self->actionFunc = (EnPoSistersActionFunc)FUN_0020627c;
        }
    } else {
        self->unk_9ED = (u8)((*EnPoSisters_CurFramePtr(&self->skelAnime) / *EnPoSisters_EndFramePtr(&self->skelAnime)) * 255.0f);
    }
    if (self->unk_194 != 3) {
        if (Animation_OnFrameImpl(&self->skelAnime, 1.0f, 1.0f)) {
            Audio_PlayActorSound2(&self->actor, 0x010002E3);
        }
    }
    Actor_SetFocus(&self->actor, 40.0f);
}
// Draft, 25 words off (size matches: VFP numbering of the 6 constants; 720 declaration orders tried, best 23). GLOBAL_ASM kept.
#if 0
void FUN_0011def4(EnPoSisters* self, PlayState* play) {
    f32 c_third;
    f32 c_half;
    f32 c_one;
    f32 c_384;
    f32 c_six;
    f32 c_zero;
    s32 d;

    FUN_003731e0(&self->skelAnime);
    c_one = 1.0f;
    c_zero = 0.0f;
    if (Animation_OnFrameImpl(&self->skelAnime, c_zero, c_one) && self->unk_19A != 0) {
        self->unk_19A--;
    }
    c_384 = 384.0f;
    c_six = 6.0f;
    c_third = 1.0f / 3.0f;
    c_half = 0.5f;
    if (*EnPoSisters_EndFramePtr(&self->skelAnime) * 1152.0f > c_zero) {
        f32 x = *EnPoSisters_EndFramePtr(&self->skelAnime) * c_384;
        f32 y = x * c_six;
        d = (s32)(c_half + y * c_third);
    } else {
        f32 x = *EnPoSisters_EndFramePtr(&self->skelAnime) * c_384;
        f32 y = x * c_six;
        d = (s32)(y * c_third - c_half);
    }
    self->actor.shape.rot.y += d;
    if (self->unk_19A == 0) {
        s16 diff = (s16)(self->actor.shape.rot.y - self->actor.world.rot.y);
        if (ABS(diff) < 0x1000) {
            if (self->unk_194 != 0) {
                self->collider.base.colType = 3;
                self->collider.base.acFlags &= ~AC_HARD;
                FUN_0036efa8(self);
            } else {
                Audio_PlayActorSound2(&self->actor, 0x0100035F);
                FUN_00370170(self, play);
            }
        }
    }
    if (Animation_OnFrameImpl(&self->skelAnime, c_one, c_one)) {
        Audio_PlayActorSound2(&self->actor, 0x0100035E);
    }
}
#endif
// FUN_00107f3c = func_80ADA6A0 (N64): chase the player, then attack (FUN_0010a6d8) or back to float (FUN_0036efa8).
void FUN_0010a6d8(EnPoSisters* self, PlayState* play);
void FUN_00107f3c(EnPoSisters* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 temp;

    FUN_003731e0(&self->skelAnime);
    temp = (s16)(self->actor.yawTowardsPlayer - player->actor.shape.rot.y);
    Math_StepToF(&self->actor.speedXZ, 2.0f, 0.2f);
    if (temp > 0x3000) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer + 0x3000, 0x71C);
    } else if (temp < -0x3000) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer - 0x3000, 0x71C);
    } else {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0x71C);
    }
    if (self->actor.xzDistToPlayer < 160.0f && PoSisters_Fabsf(self->actor.yDistToPlayer + 5.0f) < 30.0f) {
        if (self->unk_9ED != 0) {
            self->collider.base.colType = 9;
            self->collider.base.acFlags |= AC_HARD;
        }
        Animation_MorphToLoop(&self->skelAnime, 3, -5.0f);
        self->actor.speedXZ = 0.0f;
        s32 v;
        if ((Animation_GetLastFrame(&self->skelAnime, 3) + 1) * 3 > 0) {
            v = (s32)(0.5f + ((f32)((Animation_GetLastFrame(&self->skelAnime, 3) + 1) * 3) * 3.0f) * 0.5f);
        } else {
            v = (s32)(((f32)((Animation_GetLastFrame(&self->skelAnime, 3) + 1) * 3) * 3.0f) * 0.5f - 0.5f);
        }
        {
            s16* p = &self->unk_19A;
            *p = v;
        }
        {
            u8* f = &self->unk_199;
            *f &= ~2;
        }
        self->actionFunc = (EnPoSistersActionFunc)FUN_0010a6d8;
    } else if (self->actor.xzDistToPlayer > 240.0f) {
        FUN_0036efa8(self);
    }
}
// Draft, 43 words off (size matches; ROM recomputes the spin amount in every branch through the opaque EndFramePtr accessor;
// VFP numbering of the constants unsolved). GLOBAL_ASM kept.
#if 0
static inline f32 SpinX(EnPoSisters* self) {
    f32 frames = *EnPoSisters_EndFramePtr(&self->skelAnime) + 1.0f;
    f32 y = (f32)self->unk_19A * 2.0f;
    s32 n = (s32)((self->unk_19A > 0) ? 0.5f + y * (1.0f / 3.0f) : y * (1.0f / 3.0f) - 0.5f);
    return (frames * 3.0f - (f32)n) * 384.0f;
}

void FUN_0010a6d8(EnPoSisters* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if (self->unk_19A != 0) {
        self->unk_19A--;
    }
    s32 d;
    if (SpinX(self) > 0.0f) {
        f32 x = SpinX(self);
        d = (s32)(0.5f + (x * 2.0f) * (1.0f / 3.0f));
    } else {
        f32 x = SpinX(self);
        d = (s32)((x * 2.0f) * (1.0f / 3.0f) - 0.5f);
    }
    self->actor.shape.rot.y += d;
    if (self->unk_19A == 27 || self->unk_19A == 11) {
        Audio_PlayActorSound2(&self->actor, 0x0100035E);
    }
    if (self->unk_19A == 0) {
        self->actor.speedXZ = 5.0f;
        if (self->unk_194 == 0) {
            self->collider.base.colType = 9;
            self->collider.base.acFlags |= AC_HARD;
            Animation_MorphToLoop(&self->skelAnime, 3, -5.0f);
        }
        self->unk_19A = 8;
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->unk_199 |= 8;
        self->actionFunc = (EnPoSistersActionFunc)FUN_0011def4;
    }
}
#endif
}
