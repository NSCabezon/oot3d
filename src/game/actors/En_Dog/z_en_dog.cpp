extern "C" {
#include "z_en_dog.hpp"

#define FLAGS 0x00000000

// gSaveContext.dogParams: u16 at gSaveContext + 0x154E (not in SaveContext's header yet; En_Hy reads it the same way)
#define ENDOG_DOG_PARAMS (*(u16*)((u8*)&gSaveContext + 0x1500 + 0x4E))

void EnDog_Init(Actor* thisx, PlayState* play);
void EnDog_Destroy(Actor* thisx, PlayState* play);
void EnDog_Update(Actor* thisx, PlayState* play);
void EnDog_Draw(Actor* thisx, PlayState* play);
void EnDog_Wait(EnDog* self, PlayState* play);
void EnDog_RunAway(EnDog* self, PlayState* play);
void EnDog_FaceLink(EnDog* self, PlayState* play);
void EnDog_FollowLink(EnDog* self, PlayState* play);
void EnDog_ChooseMovement(EnDog* self, PlayState* play);
void EnDog_FollowPath(EnDog* self, PlayState* play);
s32 EnDog_CanFollow(EnDog* self, PlayState* play);
void FUN_0033dd8c(SkelAnime* skelAnime, s32 arg1, s32 arg2, s32 arg3, f32 f0, f32 f1, f32 f2, f32 f3);

typedef struct {
    f32 r, g, b, a;
} EnDogColorF;

typedef struct {
    EnDogColorF colors[2];
} EnDogColorTable;
extern const EnDogColorTable EnDog_sDrawColors;

extern ColliderCylinderInit EnDog_sCylinderInit;
extern CollisionCheckInfoInit2 EnDog_sColChkInfoInit;
extern struct_80034EC0_Entry EnDog_sAnimationInfo[];
void Audio_PlayActorSound2(Actor* actor, s32 arg1);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* b, void* c, s32 d);
Path* Path_GetByIndex(PlayState* play, s32 index, s32 max);

GLOBAL_ASM("../src/game/actors/En_Dog/data_syms.s")

GLOBAL_ASM("data/z_En_Dog.data.s")

ActorInit En_Dog_InitVars = {
    ACTOR_EN_DOG,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DOG,
    sizeof(EnDog),
    (ActorFunc)EnDog_Init,
    (ActorFunc)EnDog_Destroy,
    (ActorFunc)EnDog_Update,
    (ActorFunc)EnDog_Draw,
};

// Draft: 33 words off (kill/Wait tail layout, orr operand order); see STATUS.md
#if 0
#define ENDOG_DOG_IS_LOST (*((u8*)&gSaveContext + 0x15AA))

static inline u8* EnDog_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void EnDog_Init(Actor* thisx, PlayState* play) {
    EnDog* self = (EnDog*)thisx;
    s16 followingDog;
    void* zar;
    void* cmb;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    zar = (void*)(EnDog_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10);
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, self->actor.unk_178, -1, (Vec3s*)self->jointTable,
                 (Vec3s*)self->morphTable, 9);
    FUN_003717ac(&self->skelAnime, (struct_80034EC0_Entry*)EnDog_sAnimationInfo, 0);
    self->unk_5D0 = 1;

    if (!(self->actor.params & 0x8000)) {
        self->actor.params = (self->actor.params & ~0xF00) | ((self->actor.params & 0xF00) + 0x100);
    }

    followingDog = ((ENDOG_DOG_PARAMS & 0x0F00) >> 8);
    if (followingDog == ((self->actor.params & 0xF00) >> 8) && !(self->actor.params & 0x8000)) {
        Actor_Kill(&self->actor);
        return;
    }

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnDog_sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &EnDog_sColChkInfoInit);
    Actor_SetScale(&self->actor, 0.0075f);
    self->actor.gravity = -1.0f;
    self->waypoint = 0;
    self->path = Path_GetByIndex(play, (self->actor.params & 0xF0) >> 4, 0xF);

    switch (play->sceneNum) {
        case 0x21:
            if ((!ENDOG_DOG_IS_LOST) && (self->actor.params & 0xF00) == 0x100) {
                Actor_Kill(thisx);
            }
            break;
        case 0x35:
            if (!(self->actor.params & 0x8000)) {
                if (!ENDOG_DOG_IS_LOST) {
                    self->nextBehavior = 3;
                    self->actor.speedXZ = 0.0f;
                    self->actionFunc = EnDog_Wait;
                    return;
                } else {
                    Actor_Kill(thisx);
                    return;
                }
            }
            break;
    }

    if (self->actor.params & 0x8000) {
        self->nextBehavior = 0;
        self->actionFunc = EnDog_FollowLink;
    } else {
        self->nextBehavior = 3;
        self->actionFunc = EnDog_ChooseMovement;
    }
}
#endif
GLOBAL_ASM("asm/EnDog_Init.s")

void EnDog_Destroy(Actor* thisx, PlayState* play) {
    EnDog* self = (EnDog*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
}

void EnDog_Update(Actor* thisx, PlayState* play) {
    EnDog* self = (EnDog*)thisx;
    s32 animation = 0;

    if (self->behavior != self->nextBehavior) {
        if (self->nextBehavior == 4) {
            self->nextBehavior = 3;
        }
        if (self->nextBehavior == 6) {
            self->nextBehavior = 5;
        }

        self->behavior = self->nextBehavior;
        switch (self->behavior) {
            case 0:
                animation = 1;
                break;
            case 1:
                animation = 2;
                break;
            case 2:
                animation = 3;
                break;
            case 3:
                animation = 4;
                break;
            case 5:
                animation = 6;
                break;
        }
        FUN_003717ac(&self->skelAnime, (struct_80034EC0_Entry*)EnDog_sAnimationInfo, animation);
    }

    switch (self->behavior) {
        case 3:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, (struct_80034EC0_Entry*)EnDog_sAnimationInfo, 5);
                self->behavior = self->nextBehavior = 4;
            }
            break;
        case 5:
            if (Animation_OnFrameImpl(&self->skelAnime, self->skelAnime.endFrame, 1.0f)) {
                FUN_003717ac(&self->skelAnime, (struct_80034EC0_Entry*)EnDog_sAnimationInfo, 7);
                self->behavior = self->nextBehavior = 6;
            }
            break;
        case 0:
            if (self->skelAnime.animationType == 5) {
                if (Animation_OnFrameImpl(&self->skelAnime, 1.0f, 2.0f / 3.0f) ||
                    Animation_OnFrameImpl(&self->skelAnime, 7.0f, 2.0f / 3.0f)) {
                }
            }
            break;
        case 1:
            if (self->skelAnime.animationType == 3) {
                if (Animation_OnFrameImpl(&self->skelAnime, 2.0f, 2.0f / 3.0f) ||
                    Animation_OnFrameImpl(&self->skelAnime, 4.0f, 2.0f / 3.0f)) {
                    Audio_PlayActorSound2(&self->actor, 0x01000269);
                }
            }
            break;
        case 2:
            if (self->skelAnime.animationType == 0) {
                if (Animation_OnFrameImpl(&self->skelAnime, 13.0f, 2.0f / 3.0f) ||
                    Animation_OnFrameImpl(&self->skelAnime, 19.0f, 2.0f / 3.0f)) {
                    Audio_PlayActorSound2(&self->actor, 0x0100022F);
                }
            }
            break;
    }

    FUN_00370734(&self->skelAnime);

    if (self->unk_5D0 != 0) {
        Actor_UpdateBgCheckInfo(play, &self->actor, self->collider.dim.radius, self->collider.dim.height * 0.5f,
                                0.0f, 5);
        if (play->sceneNum == SCENE_MARKET_DAY && (self->actor.bgCheckFlags & 3)) {
            self->actor.gravity = 0.0f;
            self->actor.velocity.y = 0.0f;
            self->unk_5D0 = 0;
        }
    }

    Actor_MoveForward(thisx);
    self->actionFunc(self, play);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void EnDog_Draw(Actor* thisx, PlayState* play) {
    EnDog* self = (EnDog*)thisx;
    EnDogColorTable colors = EnDog_sDrawColors;

    FUN_0033dd8c(&self->skelAnime, 0, 4, 0, colors.colors[thisx->params & 0xF].r,
                 colors.colors[thisx->params & 0xF].g, colors.colors[thisx->params & 0xF].b,
                 colors.colors[thisx->params & 0xF].a);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, NULL, &self->actor, 1);
}

s32 EnDog_CanFollow(EnDog* self, PlayState* play) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        return 2;
    }

    if (play->sceneNum == SCENE_MARKET_DAY) {
        return 0;
    }

    if (self->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
        self->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
        if (ENDOG_DOG_PARAMS != 0) {
            return 0;
        }
        ENDOG_DOG_PARAMS = self->actor.params & 0x7FFF;
        return 1;
    }

    return 0;
}

void EnDog_ChooseMovement(EnDog* self, PlayState* play) {
    if (EnDog_CanFollow(self, play) == 1) {
        self->actionFunc = EnDog_FollowLink;
    }

    if (DECR(self->behaviorTimer) == 0) {
        self->behaviorTimer = Rand_S16Offset_003702c8(200, 100);
        s16 nextBehavior = 1 & ~play->state.frames;

        self->nextBehavior = nextBehavior;
        if (nextBehavior == 1) {
            self->behaviorTimer /= 2;
        }
        self->actionFunc = EnDog_FollowPath;
    }
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.4f, 1.0f, 0.0f);
}

static __inline __pure float EnDog_Fabsf(float x) {
    return __fabsf(x);
}

void EnDog_FaceLink(EnDog* self, PlayState* play) {
    s16 rotTowardLink;
    s16 prevRotY;
    f32 absAngleDiff;

    // if the dog is more than 200 units away from Link, turn to face him then wait
    if (200.0f <= self->actor.xzDistToPlayer) {
        self->nextBehavior = 0;

        Math_ApproachF(&self->actor.speedXZ, 1.0f, 0.6f, 1.0f);

        rotTowardLink = self->actor.yawTowardsPlayer;
        prevRotY = self->actor.world.rot.y;
        Math_SmoothStepToS(&self->actor.world.rot.y, rotTowardLink, 10, 1000, 1);

        absAngleDiff = self->actor.world.rot.y;
        absAngleDiff -= prevRotY;
        absAngleDiff = EnDog_Fabsf(absAngleDiff);
        if (absAngleDiff < 200.0f) {
            self->nextBehavior = 3;
            self->actor.speedXZ = 0.0f;
            self->actionFunc = EnDog_Wait;
        } else {
            self->actor.shape.rot = self->actor.world.rot;
            return;
        }
    } else {
        self->nextBehavior = 1;
        self->actionFunc = EnDog_RunAway;
    }
    self->actor.shape.rot = self->actor.world.rot;
}

void EnDog_FollowLink(EnDog* self, PlayState* play) {
    f32 speedXZ;

    if (ENDOG_DOG_PARAMS == 0) {
        self->nextBehavior = 3;
        self->actor.speedXZ = 0.0f;
        self->actionFunc = EnDog_Wait;
        return;
    }

    if (self->actor.xzDistToPlayer > 400.0f) {
        if (self->nextBehavior != 3 && self->nextBehavior != 4) {
            self->nextBehavior = 5;
        }
        ENDOG_DOG_PARAMS = 0;
        speedXZ = 0.0f;
    } else if (self->actor.xzDistToPlayer > 100.0f) {
        self->nextBehavior = 1;
        speedXZ = 4.0f;
    } else if (self->actor.xzDistToPlayer < 40.0f) {
        if (self->nextBehavior != 5 && self->nextBehavior != 6) {
            self->nextBehavior = 5;
        }
        speedXZ = 0.0f;
    } else {
        self->nextBehavior = 0;
        speedXZ = 1.0f;
    }

    Math_ApproachF(&self->actor.speedXZ, speedXZ, 0.6f, 1.0f);

    if (!(self->actor.xzDistToPlayer > 400.0f)) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 10, 1000, 1);
        self->actor.shape.rot = self->actor.world.rot;
    }
}

void EnDog_RunAway(EnDog* self, PlayState* play) {
    if (self->actor.xzDistToPlayer < 200.0f) {
        Math_ApproachF(&self->actor.speedXZ, 4.0f, 0.6f, 1.0f);
        s16 yawFlip = (s16)(self->actor.yawTowardsPlayer ^ 0x8000);
        Math_SmoothStepToS(&self->actor.world.rot.y, yawFlip, 10, 1000, 1);
    } else {
        self->actionFunc = EnDog_FaceLink;
    }
    self->actor.shape.rot = self->actor.world.rot;
}

void EnDog_Wait(EnDog* self, PlayState* play) {
    self->unusedAngle = (self->actor.yawTowardsPlayer - self->actor.shape.rot.y);

    // If another dog is following Link and he gets within 200 units of waiting dog, run away
    if ((ENDOG_DOG_PARAMS != 0) && (self->actor.xzDistToPlayer < 200.0f)) {
        self->nextBehavior = 1;
        self->actionFunc = EnDog_RunAway;
    }
}

#define ENDOG_EVENTINF (*(u16*)((u8*)&gSaveContext + 0x1500 + 0x90))

f32 Path_OrientAndGetDistSq(Actor* actor, Path* path, s16 waypoint, s16* yaw);

// Draft: 3 words off (ldrsh vs ldrh on waypoint; cmp operand order of the count check); see STATUS.md
#if 0
void EnDog_FollowPath(EnDog* self, PlayState* play) {
    s32 behaviors[] = { 3, 5, 2 };
    s32 frame;
    f32 speedXZ;
    s16 targetYaw;
    f32 waypointDistSq;
    s32 change;

    if (EnDog_CanFollow(self, play) == 1) {
        self->actionFunc = EnDog_FollowLink;
    }

    if (DECR(self->behaviorTimer) != 0) {
        if (self->nextBehavior == 0) {
            speedXZ = 1.0f;
        } else {
            speedXZ = 4.0f;
        }
        Math_SmoothStepToF(&self->actor.speedXZ, speedXZ, 0.4f, 1.0f, 0.0f);

        waypointDistSq = Path_OrientAndGetDistSq(&self->actor, self->path, self->waypoint, &targetYaw);
        Math_SmoothStepToS(&self->actor.world.rot.y, targetYaw, 10, 1000, 1);

        if ((waypointDistSq > 0.0f) && (waypointDistSq < 1000.0f)) {
            if (self->path != NULL) {
                if (self->reverse) {
                    change = -1;
                } else {
                    change = 1;
                }
                self->waypoint += change;
                if (self->reverse) {
                    if (self->waypoint < 0) {
                        self->waypoint = self->path->count - 1;
                    }
                } else {
                    if ((self->path->count - 1) < self->waypoint) {
                        self->waypoint = 0;
                    }
                }
            }
        }
        self->actor.shape.rot = self->actor.world.rot;

        if (self->waypoint <= 8) {
            ENDOG_EVENTINF |= 1;
        } else {
            ENDOG_EVENTINF &= ~1;
        }
    } else {
        frame = play->state.frames % 3;
        self->nextBehavior = behaviors[frame];
        self->behaviorTimer = Rand_S16Offset_003702c8(60, behaviors[frame]);
        self->actionFunc = EnDog_ChooseMovement;
    }
}
#endif
GLOBAL_ASM("asm/EnDog_FollowPath.s")

}
