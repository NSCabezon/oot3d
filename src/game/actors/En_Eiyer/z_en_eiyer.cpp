extern "C" {
#include "z_en_eiyer.hpp"

#define FLAGS 0x00000005

#define BGCHECKFLAG_GROUND 0x01
#define BGCHECKFLAG_GROUND_TOUCH 0x02
#define BGCHECKFLAG_WALL 0x08
#define BGCHECKFLAG_CEILING 0x10
#define DMG_DEFAULT 0xFFCFFFFF
#define DMG_BOOMERANG_EXPLOSIVE_NUT 0x19
#define NA_SE_EN_OCTAROCK_SINK 0x01000334
#define NA_SE_EN_OCTAROCK_JUMP 0x01000332
#define NA_SE_EN_EIER_FLY 0x010003BF
#define NA_SE_EN_EIER_FLUTTER 0x010003BE
#define NA_SE_EN_DODO_M_GND 0x010002EB

#define ATTENTION_ENABLED 0x1
#define IGNORE_QUAKE 0x1000
#define CULLING_DISABLED 0x10

void EnEiyer_Init(Actor* thisx, PlayState* play);
void EnEiyer_Destroy(Actor* thisx, GameState* state);
void EnEiyer_Update(Actor* thisx, GameState* state);
void EnEiyer_Draw(Actor* thisx, PlayState* play);
static void EnEiyer_SetupAmbush(EnEiyer* self, PlayState* play);
static void EnEiyer_CheckPlayerCollision(EnEiyer* self, PlayState* play);

// Actions and helpers that live in other .cpp files of this actor (or in the functions_*.cpp dumps).
void FUN_00125e80(EnEiyer* self, PlayState* play); // AppearFromGround
void FUN_00125700(EnEiyer* self, PlayState* play);
void FUN_001ca520(EnEiyer* self, PlayState* play); // Land
void FUN_0013f714(EnEiyer* self, PlayState* play); // Die
void FUN_003d5578(EnEiyer* self, PlayState* play); // Dead
void FUN_003e1500(EnEiyer* self, PlayState* play); // Stunned
void FUN_0010a380(EnEiyer* self, PlayState* play); // StartAttack
void FUN_0011cbf0(EnEiyer* self, PlayState* play); // DiveAttack
void FUN_00120d7c(EnEiyer* self, PlayState* play);
f32* EnEiyer_CurFramePtr(SkelAnime* skelAnime); // Hurt
void FUN_00127260(EnEiyer* self, PlayState* play); // Ambush
void FUN_001cd1d4(EnEiyer* self, PlayState* play); // WanderUnderground
void FUN_003de594(EnEiyer* self, PlayState* play); // CircleUnderground
void FUN_003e2080(EnEiyer* self, PlayState* play); // Glide
void FUN_00199700(EnEiyer* self, PlayState* play); // Inactive
void FUN_00239050(EnEiyer* self);                  // SetupInactive
void FUN_003465e0(EnEiyer* self);                  // SetupAppearFromGround

s32 FUN_00370734(SkelAnime* skelAnime);
s32 FUN_003731e0(SkelAnime* skelAnime);
void FUN_00370084(s16* pValue, s16 target, s16 scale, s16 step);
s32 FUN_0036e5e0(SkelAnime* skelAnime, f32 frame, f32 updateRate);
void FUN_00373264(Actor* actor, s32 sfxId);
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);
void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable,
                  void* morphTable, s32 limbCount);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Audio_PlayActorSound2(Actor* actor, s32 sfxId);
void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 type, s16 scale);
f32 BgCheck_EntityRaycastFloor4(void* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);
s32 Math_ScaledStepToS(s16* value, s16 target, s16 step);
void Math_ApproachF(f32* pValue, f32 target, f32 fraction, f32 step);
s32 Rand_S16Offset(s16 base, s16 range);
f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);
s16 Actor_WorldYawTowardPoint(Actor* actor, Vec3f* point);
s16 Actor_WorldPitchTowardPoint(Actor* actor, Vec3f* point);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
void Actor_Kill(Actor* actor);
void SkelAnime_Free2(SkelAnime* skelAnime);
void Actor_PlaySfx(Actor* actor, s32 sfxId);
void* FUN_003687a8(SkeletonAnimationModel* model);
void FUN_00357a28(void* p, s32 idx, f32* color);
void FUN_00358964(void* p, s32 idx, f32* color);
void FUN_003589cc(void* p, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);


extern ColliderCylinderInit EnEiyer_sCylinderInit;
extern CollisionCheckInfoInit EnEiyer_sColChkInfoInit;
extern DamageTable EnEiyer_sDamageTable;
extern InitChainEntry EnEiyer_sInitChain[];

GLOBAL_ASM("../src/game/actors/En_Eiyer/data_syms.s")
GLOBAL_ASM("data/z_En_Eiyer.data.s")

ActorInit En_Eiyer_InitVars = {
    ACTOR_EN_EIYER,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_EI,
    sizeof(EnEiyer),
    (ActorFunc)EnEiyer_Init,
    (ActorFunc)EnEiyer_Destroy,
    (ActorFunc)EnEiyer_Update,
    (ActorFunc)EnEiyer_Draw,
};

void EnEiyer_Init(Actor* thisx, PlayState* play) {
    EnEiyer* self = (EnEiyer*)thisx;

    Actor_ProcessInitChain(&self->actor, EnEiyer_sInitChain);
    ActorShape_Init(&self->actor.shape, 600.0f, ActorShadow_DrawCircle, 65.0f);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(&self->actor, play, &self->skelanime, 0, 0, self->jointTable, self->morphTable, 14);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnEiyer_sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, (DamageTable*)((u8*)&EnEiyer_sColChkInfoInit + 0x40), &EnEiyer_sColChkInfoInit);

    if (self->actor.params < 3) {
        // Each clone spawns another clone
        Actor* clone = Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_EIYER, self->actor.home.pos.x,
                                          self->actor.home.pos.y, self->actor.home.pos.z, 0,
                                          self->actor.shape.rot.y + 0x4000, 0, self->actor.params + 1);
        if (clone == NULL) {
            Actor_Kill(&self->actor);
            return;
        }

        if (self->actor.params == 0) {
            Actor* child = self->actor.child;
            s32 clonesSpawned;

            for (clonesSpawned = 0; clonesSpawned != 3; clonesSpawned++) {
                if (child == NULL) {
                    break;
                }
                child = child->child;
            }

            if (clonesSpawned != 3) {
                for (child = &self->actor; child != NULL; child = child->child) {
                    Actor_Kill(child);
                }
                return;
            } else {
                self->actor.child->parent = &self->actor;
                self->actor.child->child->parent = &self->actor;
                thisx->child->child->child->parent = &self->actor;
            }
        }
    }

    if (self->actor.params == 0 || self->actor.params == 0xA) {
        FUN_003465e0(self);
    } else {
        self->actor.flags |= ATTENTION_ENABLED;
        self->actor.world.rot.y = self->actor.shape.rot.y;
        self->actionFunc = FUN_00199700;
    }
}

void EnEiyer_Destroy(Actor* actor, GameState* state) {
    EnEiyer* self = (EnEiyer*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(&self->actor, self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelanime);
}

GLOBAL_ASM("asm/EnEiyer_Update.s")

void FUN_003465e0(EnEiyer* self) {
    self->collider.info.bumper.dmgFlags = DMG_BOOMERANG_EXPLOSIVE_NUT;
    Animation_PlayLoop(&self->skelanime, 0);
    self->actor.world.pos.x = self->actor.home.pos.x;
    self->actor.world.pos.y = self->actor.home.pos.y - 40.0f;
    self->actor.world.pos.z = self->actor.home.pos.z;
    self->actor.velocity.y = 0.0f;
    self->actor.speedXZ = 0.0f;

    if (self->actor.params != 0xA) {
        if (self->actor.params == 0) {
            self->actor.world.rot.y = Rand_ZeroOne() * 0x10000;
        } else {
            self->actor.world.rot.y = self->actor.parent->world.rot.y + self->actor.params * 0x4000;
        }
        self->actor.world.pos.x = Math_SinS(self->actor.world.rot.y) * 80.0f + self->actor.home.pos.x;
        self->actor.world.pos.z = Math_CosS(self->actor.world.rot.y) * 80.0f + self->actor.home.pos.z;
        self->actor.shape.rot.y = self->actor.world.rot.y + 0x4000;
    } else {
        self->actor.world.rot.y = self->actor.shape.rot.y = Rand_ZeroOne() * 0x10000;
    }

    self->collider.base.atFlags &= ~AT_ON;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.flags |= ATTENTION_ENABLED;
    self->actor.flags &= ~IGNORE_QUAKE;
    self->actor.shape.shadowScale = 0.0f;
    self->actor.shape.yOffset = 0.0f;
    self->actionFunc = FUN_00125e80;
}

void FUN_00125e80(EnEiyer* self, PlayState* play) {
    FUN_00370734(&self->skelanime);

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.75f)) {
        if (self->actor.params == 0xA) {
            self->actor.speedXZ = -0.5f;
            self->actionFunc = FUN_001cd1d4;
        } else {
            self->actionFunc = FUN_003de594;
        }

        self->collider.base.acFlags |= AC_ON;
        self->actor.flags &= ~CULLING_DISABLED;
        self->actor.flags |= ATTENTION_ENABLED;
    }
}

void FUN_00125700(EnEiyer* self, PlayState* play) {
    FUN_003731e0(&self->skelanime);

    if (self->timer != 0) {
        self->timer--;
        if (self->timer != 0) {
            return;
        }
    }

    Animation_MorphToPlayOnce(&self->skelanime, 3, -3.0f);
    self->actor.gravity = 0.0f;
    self->collider.base.atFlags &= ~AT_ON;
    self->actor.flags |= CULLING_DISABLED;
    self->timer = -1;
    self->collider.dim.height = EnEiyer_sCylinderInit.dim.height;
    self->actionFunc = FUN_001ca520;
}

void FUN_001ca520(EnEiyer* self, PlayState* play) {
    FUN_00370734(&self->skelanime);
    Math_ScaledStepToS(&self->actor.world.rot.x, -0x4000, 0x450);
    Math_StepToF(&self->actor.speedXZ, 7.0f, 1.0f);

    if (self->timer == -1) {
        if (self->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL)) {
            self->timer = (s16)(10 * 3.0f / 2.0f + 0.5f);
            Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 30, NA_SE_EN_OCTAROCK_SINK);

            if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
                EffectSsGSplash_Spawn(play, &self->actor.world.pos, NULL, NULL, 1, 700);
            }
        }
    } else {
        if (self->timer != 0) {
            self->timer--;
            if (self->timer != 0) {
                return;
            }
        }

        self->actor.shape.rot.x = 0;
        self->actor.world.rot.x = 0;
        FUN_003465e0(self);
    }
}

void FUN_0013f714(EnEiyer* self, PlayState* play) {
    FUN_00370734(&self->skelanime);

    if (self->actor.speedXZ > 0.0f) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x4000, 0x400);
    } else {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x4000, 0x400);
    }

    self->actor.shape.rot.z += 0x1000;

    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.world.rot.x = -self->actor.shape.rot.x;

    if (self->timer == 0 || (self->actor.bgCheckFlags & BGCHECKFLAG_CEILING)) {
        self->actor.speedXZ = 0.0f;
        self->actor.velocity.y = 0.0f;
        Animation_Change_00375c08(&self->skelanime, 5, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelanime, 5), 0,
                                  -8.0f);
        // actor.colorFilterParams |= 0x2000 (no named field for 0x11C in the repo Actor struct yet)
        ((u32*)&self->actor)[0x11C / 4] |= 0x200000;
        self->actionFunc = FUN_003d5578;
    }
}

void FUN_003d5578(EnEiyer* self, PlayState* play) {
    FUN_00370734(&self->skelanime);

    s32 alpha = self->actor.shape.shadowAlpha - 5;
    self->actor.shape.shadowAlpha = (alpha < 0) ? 0 : alpha;
    self->actor.world.pos.y -= 2.0f;

    if (self->actor.shape.shadowAlpha == 0) {
        Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0x50);
        Actor_Kill(&self->actor);
    }
}

void FUN_003e1500(EnEiyer* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }

    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x200);
    FUN_00370734(&self->skelanime);

    if (FUN_0036e5e0(&self->skelanime, 0.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_EIER_FLUTTER);
    }

    if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
    }

    if (self->timer == 0) {
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->collider.dim.height = EnEiyer_sCylinderInit.dim.height;

        self->targetYaw = self->actor.shape.rot.y;
        self->basePos.y = (__hardfp_cosf(-M_PI / 8) * 5.0f) + self->actor.world.pos.y;
        Animation_MorphToLoop(&self->skelanime, 2, -5.0f);
        self->timer = 60;
        self->actionFunc = FUN_003e2080;
    }
}

void FUN_0010a380(EnEiyer* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f focus;

    FUN_00370734(&self->skelanime);

    if (self->actor.shape.rot.x > 0 && self->actor.shape.rot.x < 0x8000) {
        focus.x = player->actor.world.pos.x;
        focus.y = player->actor.world.pos.y + 20.0f;
        focus.z = player->actor.world.pos.z;

        if (Math_ScaledStepToS(&self->actor.shape.rot.x, Actor_WorldPitchTowardPoint(&self->actor, &focus), 0x1000)) {
            Player* p = GET_PLAYER(play);
            self->actor.velocity.y = 0.0f;
            self->basePos.y = p->actor.world.pos.y + 15.0f;
            self->collider.base.atFlags |= AT_ON;
            self->collider.base.atFlags &= ~AT_HIT;
            self->actionFunc = FUN_0011cbf0;
        }
    } else {
        self->actor.shape.rot.x -= 0x1000;
    }

    self->actor.world.rot.x = -self->actor.shape.rot.x;
    Math_StepToF(&self->actor.speedXZ, 5.0f, 0.3f);
    FUN_00370084(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 2, 0x71C);
    FUN_00373264(&self->actor, NA_SE_EN_EIER_FLY);
}

void FUN_0011cbf0(EnEiyer* self, PlayState* play) {
    FUN_00370734(&self->skelanime);
    self->actor.speedXZ *= 1.1f;

    if (self->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_WALL)) {
        Animation_MorphToPlayOnce(&self->skelanime, 3, -3.0f);
        self->actor.gravity = 0.0f;
        self->collider.base.atFlags &= ~AT_ON;
        self->actor.flags |= CULLING_DISABLED;
        self->timer = -1;
        self->collider.dim.height = EnEiyer_sCylinderInit.dim.height;
        self->actionFunc = FUN_001ca520;
    }

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~(AT_ON | AT_HIT);

        if (self->collider.base.atFlags & 4) {
            Animation_Change_00375c08(&self->skelanime, 6, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelanime, 6), 3,
                                      0.0f);
            s32 frames;
            f32 half = 0.5f;
            f32 k = 1.5f;
            if (self->skelanime.animLength > 0.0f) {
                frames = (s32)(half + self->skelanime.animLength * k);
            } else {
                frames = (s32)(self->skelanime.animLength * k - half);
            }
            self->timer = frames;
            self->actor.speedXZ = 0.0f;
            self->actor.shape.rot.x = 0;
            self->actor.shape.rot.z = 0;
            self->actionFunc = FUN_00125700;
            return;
        }
    }

    FUN_00373264(&self->actor, NA_SE_EN_EIER_FLY);
}

void FUN_00120d7c(EnEiyer* self, PlayState* play) {
    FUN_00370734(&self->skelanime);

    if (self->timer != 0) {
        self->timer--;
    }

    Math_ApproachF(&self->basePos.y, self->actor.floorHeight + 80.0f + 5.0f, 0.5f, self->actor.speedXZ);
    self->actor.world.pos.y = self->basePos.y - 5.0f;

    if (self->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        self->targetYaw = self->actor.wallYaw;
    } else {
        self->targetYaw = self->actor.yawTowardsPlayer + 0x8000;
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, self->targetYaw, 0x38E);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x200);
    self->actor.shape.rot.z = __hardfp_sinf(self->timer * (M_PI / 5)) * 5120.0f;

    if (self->timer == 0) {
        self->collider.base.acFlags |= AC_ON;
        self->actor.shape.rot.x = 0;
        self->actor.shape.rot.z = 0;
        self->targetYaw = self->actor.shape.rot.y;
        self->basePos.y = (__hardfp_cosf(-M_PI / 8) * 5.0f) + self->actor.world.pos.y;
        Animation_MorphToLoop(&self->skelanime, 2, -5.0f);
        self->timer = 60;
        self->actionFunc = FUN_003e2080;
    }

    self->actor.world.rot.x = -self->actor.shape.rot.x;
}

void FUN_00127260(EnEiyer* self, PlayState* play) {
    s32 animFinished = FUN_00370734(&self->skelanime);
    f32 curFrame;
    f32 xzOffset;
    s32 bgId;

    curFrame = *EnEiyer_CurFramePtr(&self->skelanime);
    if (curFrame < 12.0f) {
        xzOffset = curFrame * (0.996f * M_PI / 12.0f);
        self->actor.world.pos.y = ((1.0f - __hardfp_cosf(xzOffset)) * 40.0f) + self->actor.home.pos.y;
        xzOffset = __hardfp_sinf(xzOffset) * -40.0f;
        self->actor.world.pos.x = (xzOffset * Math_SinS(self->actor.shape.rot.y)) + self->basePos.x;
        self->actor.world.pos.z = (xzOffset * Math_CosS(self->actor.shape.rot.y)) + self->basePos.z;
    } else {
        Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 80.0f, 0.5f);
        self->actor.speedXZ = 0.8f;
    }

    if (animFinished) {
        self->collider.base.acFlags |= AC_ON;
        self->targetYaw = self->actor.shape.rot.y;
        self->basePos.y = (__hardfp_cosf(-M_PI / 8) * 5.0f) + self->actor.world.pos.y;
        Animation_MorphToLoop(&self->skelanime, 2, -5.0f);
        self->timer = 60;
        self->actionFunc = FUN_003e2080;
    } else {
        self->actor.floorHeight = BgCheck_EntityRaycastFloor4(&play->colCtx, &self->actor.floorPoly, &bgId, &self->actor,
                                                              &self->actor.world.pos);
    }
}

void FUN_001cd1d4(EnEiyer* self, PlayState* play) {
    FUN_00370734(&self->skelanime);

    if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) > 100.0f) {
        self->targetYaw = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos) + 0x8000;
    } else if (self->targetYaw == self->actor.world.rot.y && Rand_ZeroOne() > 0.99f) {
        self->targetYaw =
            self->actor.world.rot.y + (Rand_ZeroOne() < 0.5f ? -1 : 1) * (Rand_ZeroOne() * 0x2000 + 0x2000);
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, self->targetYaw, 0xB6);

    EnEiyer_CheckPlayerCollision(self, play);
}

static void EnEiyer_SetupAmbush(EnEiyer* self, PlayState* play) {
    self->actor.speedXZ = 0.0f;
    Animation_PlayOnce(&self->skelanime, 1);
    self->collider.info.bumper.dmgFlags = DMG_DEFAULT;
    self->basePos = self->actor.world.pos;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.flags |= IGNORE_QUAKE;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.shape.shadowScale = 65.0f;
    self->actor.shape.yOffset = 600.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_JUMP);
    EffectSsGSplash_Spawn(play, &self->actor.world.pos, NULL, NULL, 1, 700);
    self->actionFunc = FUN_00127260;
}

static void EnEiyer_CheckPlayerCollision(EnEiyer* self, PlayState* play) {
    if (self->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
        self->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
        EnEiyer_SetupAmbush(self, play);
    }
}

// NON_MATCHING draft: CircleUnderground: 1 word off (function-address compare emitted as pc-relative sub, ROM uses literal pool). Still GLOBAL_ASM.
// recipe_near4 2026-10-15: SYSTEMIC - all 12 ROM fns that take their OWN address use the literal pool (none matched yet). Only
// `__weak` on the definition gives 0 words (static/inline/helper/const-ptr do not); not applied (linkage hack, orchestrator call).
#if 0
void FUN_003de594(EnEiyer* self, PlayState* play) {
    FUN_00370734(&self->skelanime);
    self->actor.world.rot.y -= 0x60;
    self->actor.world.pos.x = Math_SinS(self->actor.world.rot.y) * 80.0f + self->actor.home.pos.x;
    self->actor.world.pos.z = Math_CosS(self->actor.world.rot.y) * 80.0f + self->actor.home.pos.z;
    self->actor.shape.rot.y = self->actor.world.rot.y + 0x4000;

    EnEiyer_CheckPlayerCollision(self, play);

    if (self->actor.params != 0 && ((EnEiyer*)self->actor.parent)->actionFunc != FUN_003de594) {
        FUN_00239050(self);
    }
}

#endif


void FUN_003e2080(EnEiyer* self, PlayState* play) {
    f32 curFrame;

    FUN_00370734(&self->skelanime);

    if (self->timer != 0) {
        self->timer--;
    }

    curFrame = *EnEiyer_CurFramePtr(&self->skelanime);
    Math_ApproachF(&self->basePos.y, self->actor.floorHeight + 80.0f + 5.0f, 0.3f, self->actor.speedXZ);
    self->actor.world.pos.y = self->basePos.y - __hardfp_cosf((curFrame - 5.0f) * (M_PI / 40)) * 5.0f;

    if (curFrame <= 45.0f) {
        Math_StepToF(&self->actor.speedXZ, 1.0f, 0.03f);
    } else {
        Math_StepToF(&self->actor.speedXZ, 1.5f, 0.03f);
    }

    if (self->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
        self->targetYaw = self->actor.wallYaw;
    }

    if (Math_ScaledStepToS(&self->actor.world.rot.y, self->targetYaw, 0xB6)) {
        if (self->timer != 0 || Rand_ZeroOne() > 0.05f) {
            self->actor.world.rot.y += 0x100;
        } else {
            s32 yawChange = Rand_S16Offset(0x2000, 0x2000);

            self->targetYaw = yawChange * (Rand_ZeroOne() < 0.5f ? -1 : 1) + self->actor.world.rot.y;
        }
    }

    if (self->timer == 0 && self->actor.yDistToPlayer < 0.0f && self->actor.xzDistToPlayer < 120.0f) {
        self->actionFunc = FUN_0010a380;
    }

    FUN_00373264(&self->actor, NA_SE_EN_EIER_FLY);
}

s32 FUN_001e4908(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    f32 sn;
    f32 cs;

    if (limbIndex == 1) {
        f32 angle = M_PI / 1638.4f;
        sn = __hardfp_sinf(angle);
        cs = __hardfp_cosf(angle);

        f32 m1 = mtx->data[0][1];
        f32 m2 = mtx->data[0][2];
        mtx->data[0][1] = m1 * cs + m2 * sn;
        mtx->data[0][2] = m2 * cs - m1 * sn;

        m1 = mtx->data[1][1];
        m2 = mtx->data[1][2];
        mtx->data[1][1] = m1 * cs + m2 * sn;
        mtx->data[1][2] = m2 * cs - m1 * sn;

        m1 = mtx->data[2][1];
        m2 = mtx->data[2][2];
        mtx->data[2][1] = m1 * cs + m2 * sn;
        mtx->data[2][2] = m2 * cs - m1 * sn;
    }
    return 0;
}


// NON_MATCHING draft: Draw 56 words off (ROM register/pool layout around the limb-draw calls)
#if 0
void EnEiyer_Draw(Actor* thisx, PlayState* play) {
    EnEiyer* self = (EnEiyer*)thisx;
    f32 color[4];
    void* p;

    if (self->actionFunc != FUN_003d5578) {
        p = FUN_003687a8(self->skelanime.unk_28);
        FUN_00357a28(p, 5, color);
        color[3] = 1.0f;
        FUN_00358964(p, 5, color);
        FUN_0037266c(self->skelanime.unk_28, 2);
        FUN_0037266c(self->skelanime.unk_28, 0);
        FUN_0037266c(self->skelanime.unk_28, 1);
        if (self->collider.info.bumper.dmgFlags == DMG_BOOMERANG_EXPLOSIVE_NUT) {
            FUN_0036932c(self->skelanime.unk_28, 2);
            FUN_0036932c(self->skelanime.unk_28, 0);
        }
    } else {
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 0.0f;
        color[3] = self->actor.shape.shadowAlpha * (1.0f / 255.0f);

        p = FUN_003687a8(self->skelanime.unk_28);
        FUN_003589cc(p, 5);
        FUN_00358964(p, 5, color);
    }

    SkelAnime_DrawOpa(&self->skelanime, &self->actor.modelMtx, FUN_001e4908, NULL, &self->actor, 0);
}
#endif

GLOBAL_ASM("asm/EnEiyer_Draw.s")
}
