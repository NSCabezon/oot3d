extern "C" {
#include "z_en_skb.hpp"

#define FLAGS 0x00000015

void EnSkb_Init(Actor* actor, GameState* state);
void EnSkb_Destroy(EnSkb* self, PlayState* play);
void EnSkb_Update(Actor* actor, GameState* state);
void EnSkb_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Skb.data.s")
GLOBAL_ASM("../src/game/actors/En_Skb/data_syms.s")

ActorInit En_Skb_InitVars = {
    ACTOR_EN_SKB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SKB,
    sizeof(EnSkb),
    (ActorFunc)EnSkb_Init,
    (ActorFunc)EnSkb_Destroy,
    (ActorFunc)EnSkb_Update,
    (ActorFunc)EnSkb_Draw,
};

void ActorShadow_DrawCircle(Actor* actor, Lights* lights, PlayState* play);
extern u8 EnSkb_sInitChain[];
extern u8 EnSkb_sDamageTable[];
extern u8 EnSkb_sJntSphInit[];
void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
void FUN_001c722c(EnSkb* self, PlayState* play);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);
f32 Math_SmoothStepToF(f32* pValue, f32 target, f32 fraction, f32 step, f32 minStep);
void FUN_0033ff3c(PlayState* play, EnSkb* self, Vec3f* pos);
void Actor_Kill(Actor* actor);
s32 BodyBreak_SpawnParts(Actor* actor, void* bodyBreak, PlayState* play, s32 a, s32 b);
f32 Math_SinF(f32 angle);
f32 Math_CosF(f32 angle);
void EffectSsHahen_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 flags, s16 scale, s16 objectSlot,
                         s16 params, void* dList);
void Item_DropCollectibleRandom(PlayState* play, Actor* parent, Vec3f* pos, s32 params);
EnItem00* Item_DropCollectible(PlayState* play, Vec3f* spawnPos, s16 params);
void EffectSsDeadSound_SpawnStationary(PlayState* play, Vec3f* pos, s32 sfxId, s32 a, s32 b, s32 c);
void BodyBreak_Alloc(void* bodyBreak, s32 arg, PlayState* play);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
s32 Actor_IsFacingPlayer(Actor* actor, s16 angle);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void EnSkb_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
void FUN_0034f184(void* bodyBreak, s32 limbIndex, s32 a, s32 b, s32 c, MTX34* mtx);
void FUN_0013da38(EnSkb* self, PlayState* play);

static inline void EnSkb_SetupRise(EnSkb* self) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, 3, 1.0f);
    self->unk_DF0 = 0;
    self->actor.flags &= ~1;
    Audio_PlayActorSound2(&self->actor, 0x010003F6);
    self->actionFunc = FUN_001c722c;
}

void EnSkb_Init(Actor* thisx, GameState* state) {
    EnSkb* self = (EnSkb*)thisx;
    PlayState* play = (PlayState*)state;

    FUN_00372f38(&self->actor, play, 0);
    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)EnSkb_sInitChain);
    thisx->colChkInfo.damageTable = (DamageTable*)EnSkb_sDamageTable;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.colChkInfo.mass = 0xFE;
    self->actor.colChkInfo.health = 2;
    self->actor.shape.yOffset = -8000.0f;
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 3, (u8*)self + 0x228, (u8*)self + 0x80C, 0x1D);
    self->actor.naviEnemyId = 0x55;

    Collider_InitJntSph(play, self->collider);
    Collider_SetJntSph(play, self->collider, &self->actor, EnSkb_sJntSphInit, (u8*)self + 0xE30);
    Actor_SetScale(&self->actor, ((self->actor.params * 0.1f) + 1.0f) * 0.01f);
    *(f32*)(*(u8**)((u8*)self + 0xE2C) + 0x34) = *(f32*)(*(u8**)((u8*)self + 0xE2C) + 0x44) = (f32)(10 + self->actor.params);
    *(f32*)(*(u8**)((u8*)self + 0xE2C) + 0x84) = *(f32*)(*(u8**)((u8*)self + 0xE2C) + 0x94) = (f32)(20 + (self->actor.params * 2));

    self->actor.home.pos = self->actor.world.pos;
    self->actor.floorHeight = self->actor.world.pos.y;
    EnSkb_SetupRise(self);
}


void Collider_DestroyJntSph(PlayState* play, void* collider);
void FUN_003504d0(Actor* actor, void* bodyBreak);
void FUN_003508b8(Actor* actor, ...);

void EnSkb_Destroy(EnSkb* self, PlayState* play) {
    Actor* actor = &self->actor;
    struct Spw { Actor a; char pad[0x1AA-0x1A4]; s16 count; };
    Spw* sp = (Spw*)self->actor.parent;
    if (sp != NULL && sp->a.update != NULL && sp->count > 0) {
        sp->count--;
    }
    Collider_DestroyJntSph(play, self->collider);
    FUN_003504d0(&self->actor, self->bodyBreak);
    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(&self->actor, 0);
}

s32 FUN_003731e0(SkelAnime* skelAnime);
void FUN_00366318(EnSkb* self);
void FUN_003d6a44(EnSkb* self, PlayState* play);
void FUN_0011c4c8(EnSkb* self, PlayState* play);

void FUN_00126304(EnSkb* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        FUN_00366318(self);
    }
}

void FUN_003ca54c(EnSkb* self, PlayState* play) {
    if (Math_SmoothStepToF((f32*)((u8*)self + 0xC4), -8000.0f, 1.0f, 500.0f, 0.0f) != 0.0f) {
        if ((*(s32*)((u8*)play + 0x5BF4) & 3) == 0) {
            FUN_0033ff3c(play, self, &self->actor.world.pos);
        }
    }
    Math_SmoothStepToF((f32*)((u8*)self + 0xCC), 0.0f, 1.0f, 2.5f, 0.0f);
    if (FUN_003731e0(&self->skelAnime)) {
        Actor_Kill(&self->actor);
    }
}

void FUN_00120460(EnSkb* self, PlayState* play) {
    if (self->unk_DF3 == 1) {
        if (BodyBreak_SpawnParts(&self->actor, self->bodyBreak, play, 1, 0) == 0) {
            return;
        }
    }
    if (self->unk_DF3 != 0) {
        self->unk_DF3 |= 2;
    }
    if (*(u16*)((u8*)self + 0x90) & 2) {
        *(f32*)((u8*)self + 0x6C) = 0.0f;
    }
    if ((*(u16*)((u8*)self + 0x90) & 1) && *(f32*)((u8*)self + 0x6C) < 0.0f) {
        *(f32*)((u8*)self + 0x6C) += 0.05f;
    }
    Math_SmoothStepToS((s16*)((u8*)self + 0xBE), *(s16*)((u8*)self + 0x92), 1, 0x1194, 0);
    if (FUN_003731e0(&self->skelAnime) && (*(u16*)((u8*)self + 0x90) & 1)) {
        FUN_00366318(self);
    }
}

void FUN_0033ff3c(PlayState* play, EnSkb* self, Vec3f* spawnPos) {
    Vec3f pos;
    Vector3f vel(0.0f, 8.0f, 0.0f);
    Vector3f accel(0.0f, -1.5f, 0.0f);
    f32 spreadAngle;
    f32 scale;

    PlayState* play2 = play;
    spreadAngle = (Rand_ZeroOne() - 0.5f) * 6.28f;
    pos.y = self->actor.floorHeight;
    pos.x = (Math_SinF(spreadAngle) * 15.0f) + spawnPos->x;
    pos.z = (Math_CosF(spreadAngle) * 15.0f) + spawnPos->z;
    accel.x = Rand_CenteredFloat(1.0f);
    accel.z = Rand_CenteredFloat(1.0f);
    vel.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    scale = (Rand_ZeroOne() * 5.0f) + 12.0f;
    EffectSsHahen_Spawn(play2, &pos, (Vec3f*)&vel, (Vec3f*)&accel, 2, scale * 0.8f, -1, 10, NULL);
    FUN_0037378c(play, &pos, 10.0f, 1, 150, 0, 1);
}

void FUN_001c722c(EnSkb* self, PlayState* play) {
    if (self->skelAnime.curFrame < 4.0f) {
        s16 v = *(s16*)((u8*)self + 0x92);
        *(s16*)((u8*)self + 0x36) = v;
        *(s16*)((u8*)self + 0xBE) = v;
    } else {
        self->actor.flags |= 1;
    }

    Math_SmoothStepToF((f32*)((u8*)self + 0xC4), 0.0f, 1.0f, 800.0f, 0.0f);
    Math_SmoothStepToF((f32*)((u8*)self + 0xCC), 25.0f, 1.0f, 2.5f, 0.0f);
    if ((*(s32*)((u8*)play + 0x5BF4) & 3) == 0) {
        FUN_0033ff3c(play, self, &self->actor.world.pos);
    }
    if (FUN_003731e0(&self->skelAnime) && *(f32*)((u8*)self + 0xC4) == 0.0f) {
        FUN_00366318(self);
    }
}

void FUN_003d6a44(EnSkb* self, PlayState* play) {
    if (BodyBreak_SpawnParts(&self->actor, self->bodyBreak, play, 1, 0) == 0) {
        return;
    }
    if (*(f32*)((u8*)self + 0x54) == 0.01f) {
        Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0x10);
    } else if (*(f32*)((u8*)self + 0x54) <= 0.015f) {
        Item_DropCollectible(play, &self->actor.world.pos, 1);
    } else {
        Item_DropCollectible(play, &self->actor.world.pos, 2);
        Item_DropCollectible(play, &self->actor.world.pos, 2);
        Item_DropCollectible(play, &self->actor.world.pos, 2);
    }
    self->unk_DF3 |= 8;
    Actor_Kill(&self->actor);
}

void FUN_00157844(EnSkb* self, PlayState* play) {
    u16 flags = *(u16*)((u8*)self + 0x90);

    if (flags & 2) {
        *(f32*)((u8*)self + 0x6C) = 0.0f;
    }
    if ((flags & 1) && *(f32*)((u8*)self + 0x6C) < 0.0f) {
        *(f32*)((u8*)self + 0x6C) += 0.05f;
    }
    if (*(u16*)((u8*)self + 0x11A) == 0 && (flags & 1)) {
        if (*(u8*)((u8*)self + 0xB7) != 0) {
            FUN_00366318(self);
            return;
        }
        Animation_MorphToPlayOnce(&self->skelAnime, 1, -4.0f);
        *(s16*)((u8*)self + 0xBE) = *(s16*)((u8*)self + 0x92);
        *(s16*)((u8*)self + 0x36) = *(s16*)((u8*)self + 0x92);
        if (*(u16*)((u8*)self + 0x90) & 1) {
            *(f32*)((u8*)self + 0x6C) = -6.0f;
        }
        self->unk_DF0 = 1;
        self->actor.flags &= ~1;
        BodyBreak_Alloc(self->bodyBreak, 0x1D, play);
        self->unk_DF3 |= 4;
        EffectSsDeadSound_SpawnStationary(play, &self->actor.world.pos, 0x010002A3, 1, 1, 0x28);
        self->actionFunc = FUN_003d6a44;
    }
}

extern u8 EnSkb_gUnk587958[];
void FUN_00366318(EnSkb* self) {
    if (*(u32*)(EnSkb_gUnk587958 + 0x10) == 0) {
        Animation_Change_00375c08(&self->skelAnime, 3, -1.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 3), 0.0f, 2, -4.0f);
        self->unk_DF0 = 0;
        *(f32*)((u8*)self + 0x6C) = 0.0f;
        self->unk_DF1 = 0;
        self->actor.flags &= ~1;
        Audio_PlayActorSound2(&self->actor, 0x0100046C);
        self->actionFunc = FUN_003ca54c;
    } else if (Actor_IsFacingPlayer(&self->actor, 0x11C7) &&
               (*(f32*)((u8*)self + 0x98) < (60.0f + (self->actor.params * 6.0f)))) {
        Animation_Change_00375c08(&self->skelAnime, 0, 0.6f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 0), 3, 4.0f);
        self->unk_E20 &= ~4;
        self->unk_DF0 = 3;
        *(f32*)((u8*)self + 0x6C) = 0.0f;
        self->actionFunc = FUN_0011c4c8;
    } else {
        Animation_Change_00375c08(&self->skelAnime, 4, 0.96000004f, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, 4), 0, -4.0f);
        self->unk_DF0 = 4;
        *(f32*)((u8*)self + 0x6C) = self->actor.scale.y * 160.0f;
        *(u16*)((u8*)self + 0xDF8) = 0;
        self->actionFunc = FUN_0013da38;
    }
}

void FUN_0011c4c8(EnSkb* self, PlayState* play) {
    if ((s32)self->skelAnime.curFrame == 3) {
        Audio_PlayActorSound2(&self->actor, 0x010002A1);
        self->unk_DF1 = 1;
    } else if ((s32)self->skelAnime.curFrame == 6) {
        self->unk_DF1 = 0;
    }

    if (self->unk_E20 & 4) {
        self->unk_E20 &= ~6;
        Animation_Change_00375c08(&self->skelAnime, 0, -0.4f, self->skelAnime.curFrame - 1.0f, 0.0f, 3, 0.0f);
        self->unk_E20 &= ~4;
        self->unk_DF0 = 5;
        self->unk_DF1 = 0;
        self->actionFunc = FUN_00126304;
    } else if (FUN_003731e0(&self->skelAnime)) {
        FUN_00366318(self);
    }
}

void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
u8 Actor_ApplyDamage(Actor* actor);
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);
void EffectSsEnFire_SpawnVec3f(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 params, s16 flags, s16 bodyPart,
                               s16 extra);
void FUN_00157844(EnSkb* self, PlayState* play);
void FUN_00120460(EnSkb* self, PlayState* play);

// Draft: structure matches ROM (~21 normalized insn diffs), register numbering/sfx hoist differs (~200 words). See STATUS.md.
#if 0
static inline void EnSkb_SetupDeath(EnSkb* self, PlayState* play) {
    EnSkbActionFunc fn = FUN_003d6a44;
    Animation_MorphToPlayOnce(&self->skelAnime, 1, -4.0f);
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = -6.0f;
    }
    self->unk_DF0 = 1;
    self->actor.flags &= ~1;
    BodyBreak_Alloc(self->bodyBreak, 0x1D, play);
    self->unk_DF3 |= 4;
    EffectSsDeadSound_SpawnStationary(play, &self->actor.world.pos, 0x010002A3, 1, 1, 0x28);
    self->actionFunc = fn;
}

static inline void EnSkb_SetupStunned(EnSkb* self) {
    
    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = 0.0f;
    }
    Audio_PlayActorSound2(&self->actor, 0x0100030E);
    self->unk_DF1 = 0;
    self->unk_DF0 = 6;
    self->actionFunc = FUN_00157844;
}

static inline void EnSkb_SetupTakeDamage(EnSkb* self) {
    
    Animation_MorphToPlayOnce(&self->skelAnime, 2, -4.0f);
    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = -4.0f;
    }
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Audio_PlayActorSound2(&self->actor, 0x010002A2);
    self->unk_DF0 = 2;
    self->actionFunc = FUN_00120460;
}

static inline void EnSkb_CheckDamage(EnSkb* self, PlayState* play) {
    s32 i;
    Vec3f flamePos;
    s16 scale;
    s16 colorFilterDuration;

    if ((self->unk_DF0 != 1) && (self->actor.bgCheckFlags & 0x60) && (self->actor.yDistToWater >= 40.0f)) {
        self->actor.colChkInfo.health = 0;
        self->unk_DF1 = 0;
        EnSkb_SetupDeath(self, play);
    } else if (self->unk_DF0 > 2) {
        if (self->unk_E21 & 2) {
            { EnSkb* s2 = self; s2->unk_E21 &= ~2; }
            if (self->actor.colChkInfo.damageEffect != 6) {
                self->unk_DF2 = self->actor.colChkInfo.damageEffect;
                FUN_00375fd0(&self->actor, (u8*)(*(u8**)((u8*)self + 0xE2C)) + 0x50, 1);
                self->unk_DF1 = 0;
                if (self->actor.colChkInfo.damageEffect == 1) {
                    if (self->unk_DF0 != 6) {
                        Actor_SetColorFilter(&self->actor, 0, 0x78, 0, 0x50);
                        Actor_ApplyDamage(&self->actor);
EnSkb_SetupStunned(self);
                    }
                } else {
                    colorFilterDuration = 8;
                    if (self->actor.colChkInfo.damageEffect == 7) {
                        scale = self->actor.scale.y * 7500.0f;
                        for (i = 4; i >= 0; i--) {
                            Vec3f flamePos;
                            flamePos = self->actor.world.pos;
                            flamePos.x += Rand_CenteredFloat(20.0f);
                            flamePos.z += Rand_CenteredFloat(20.0f);
                            flamePos.y += (Rand_ZeroOne() * 25.0f);
                            EffectSsEnFire_SpawnVec3f(play, &self->actor, &flamePos, scale, 0, 0, -1, 1);
                        }
                        colorFilterDuration = 25;
                    }
                    Actor_SetColorFilter(&self->actor, 0x400000, 255, 0, colorFilterDuration);
                    if (!Actor_ApplyDamage(&self->actor)) {
                        EnSkb_SetupDeath(self, play);
                        goto done;
                    }
                    u8* player = *(u8**)((u8*)play + 0x20AC);
                    if (self->unk_DF3 == 0) {
                        if ((self->actor.colChkInfo.damageEffect == 0xD) ||
                            ((self->actor.colChkInfo.damageEffect == 0xE) &&
                             (((u32)(*(s8*)(player + 0x2226) - 4) <= 7) ||
                              (*(s8*)(player + 0x2226) == 0x14 || *(s8*)(player + 0x2226) == 0x15)))) {
                            BodyBreak_Alloc(self->bodyBreak, 3, play);
                            self->unk_DF3 = 1;
                        }
                    }
EnSkb_SetupTakeDamage(self);
                }
            }
        }
    }
done:;
}

void EnSkb_Update(Actor* thisx, GameState* state) {
    EnSkb* self = (EnSkb*)thisx;
    PlayState* play = (PlayState*)state;

    EnSkb_CheckDamage(self, play);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 15.0f, 30.0f, 60.0f, 0x1D);
    self->actionFunc(self, play);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += (3000.0f * self->actor.scale.y);
    if (self->unk_ED0 != 0) {
        self->unk_ED0 -= 1;
    }
    if (self->unk_E20 & 2) {
        if (self->unk_ED0 == 0) {
            self->unk_E20 &= ~2;
            self->unk_ED0 = 15;
        }
    }
    if (self->unk_DF1 && self->unk_ED0 == 0) {
        CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)self->collider);
    }
    if (self->unk_DF0 >= 3) {
        if ((*(u16*)((u8*)self + 0x11A) == 0) || ((*(u32*)((u8*)self + 0x11C) & 0x400000) == 0)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)self->collider);
        }
    }
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)self->collider);
}
#endif
GLOBAL_ASM("asm/EnSkb_Update.s")

void EnSkb_Draw(Actor* actor, GameState* state) {
    EnSkb* self = (EnSkb*)actor;
    u8 flags = self->unk_DF3;
    SkeletonAnimationModel* model = self->skelAnime.unk_28;

    if (flags & 2) {
        FUN_0036932c(model, 2);
        FUN_0036932c(model, 1);
        FUN_0036932c(model, 4);
    } else {
        FUN_0037266c(model, 2);
        FUN_0037266c(model, 1);
        FUN_0037266c(model, 4);
    }
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, (PostLimbDrawOpa)EnSkb_PostLimbDraw, actor, 0);
}

void EnSkb_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    EnSkb* self = (EnSkb*)arg;

    if (limbIndex == 0x1C || limbIndex == 0) {
        Collider_UpdateSpheres(limbIndex, self->collider, mtx);
    }
    if (self->unk_DF3 == 1) {
        FUN_0034f184(self->bodyBreak, limbIndex, 0x12, 0x14, 0x1D, mtx);
    } else if (self->unk_DF3 & 4) {
        FUN_0034f184(self->bodyBreak, limbIndex, 0, 0x1C, 0x1D, mtx);
    }
}

}
