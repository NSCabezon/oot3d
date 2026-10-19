extern "C" {
#include "z_en_mb.hpp"

#define FLAGS 0x00000015

void EnMb_Init(Actor* actor, GameState* state);
void EnMb_Destroy(Actor* actor, GameState* state);
void EnMb_Update(EnMb* self, PlayState* play);
void EnMb_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Mb.data.s")
GLOBAL_ASM("../src/game/actors/En_Mb/data_syms.s")

ActorInit En_Mb_InitVars = {
    ACTOR_EN_MB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_MB,
    sizeof(EnMb),
    (ActorFunc)EnMb_Init,
    (ActorFunc)EnMb_Destroy,
    (ActorFunc)EnMb_Update,
    (ActorFunc)EnMb_Draw,
};


f32 FUN_0036d260(Vec3f* a, Vec3f* b);

void FUN_0024cdec(void) {
}

void FUN_0019f61c(EnMb* self);
void FUN_00181510(EnMb* self);

void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void FUN_0010b9f8(EnMb* self, PlayState* play);

void FUN_003e28d8(EnMb* self, PlayState* play) {
    s16 finished = Math_SmoothStepToS(&self->actor.shape.rot.y, self->unk_8FE, 1, (s16)self->unk_8E8, 0);

    FUN_003731e0(&self->skelAnime);
    if (finished == 0) {
        self->actor.shape.rot.y = self->unk_8FE;
        self->timer1 = 0;
        Animation_MorphToLoop(&self->skelAnime, 1, -4.0f);
        self->actionFunc = FUN_0010b9f8;
    }
}

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

static __inline __pure float EnMb_Fabsf(float x) {
    return __fabsf(x);
}

void FUN_0036cec4(EnMb* self, s32 animIdx, s32 a2, s32 a3, s32 a4) {
    f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, animIdx);
    f32 speed = 1.0f;

    if (a4 != 0) {
        speed = (f32)a3 * frames / EnMb_Fabsf((f32)a2);
    }
    Animation_Change_00375c08(&self->skelAnime, animIdx, speed, 0.0f, (f32)Animation_GetLastFrame(&self->skelAnime, animIdx), 3, -4.0f);
    self->unk_8E8 = a3;
    self->actionFunc = FUN_003e28d8;
}

void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);
void FUN_0010b5d0(EnMb* self, PlayState* play);

void FUN_00123310(EnMb* self) {
    if (self->timer1 != 0) {
        self->timer1--;
    }
    if (self->timer2 != 0) {
        self->timer2--;
    }
    if (FUN_003731e0(&self->skelAnime)) {
        Animation_PlayOnce(&self->skelAnime, 20);
    }
    if (self->timer1 == 0) {
        Animation_PlayLoop(&self->skelAnime, 14);
        self->actor.speedXZ = 0.0f;
        self->timer1 = Rand_S16Offset_003702c8(30, 50);
        self->state = 6;
        self->actionFunc = FUN_0010b5d0;
    }
    if (self->timer2 == 0) {
        self->unk_8FC = 1;
    }
}

void FUN_001233b0(EnMb* self) {
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->timer2 != 0) {
            self->timer2--;
            if (self->timer2 == 0) {
                if (self->timer1 == 0) {
                    f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 20);
                    Animation_Change_00375c08(&self->skelAnime, 20, 3.0f, 0.0f, frames, 3, 0.0f);
                    self->timer1 = 1;
                    self->timer2 = 9;
                } else {
                    f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 20);
                    Animation_Change_00375c08(&self->skelAnime, 20, 3.0f, 0.0f, frames, 3, 0.0f);
                }
            }
        } else {
            Animation_PlayLoop(&self->skelAnime, 14);
            self->actor.speedXZ = 0.0f;
            self->timer1 = Rand_S16Offset_003702c8(30, 50);
            self->state = 6;
            self->actionFunc = FUN_0010b5d0;
        }
    }
}

void FUN_00137164(EnMb* self, PlayState* play);
void FUN_001375bc(EnMb* self, PlayState* play);

void FUN_00159734(EnMb* self, PlayState* play) {
    s32 found = 0;
    u8* ctx = (u8*)play + 0x2000;
    EnMb* other = *(EnMb**)(ctx + 0xC4);
    u32* flags;

    while (other != NULL) {
        if (other->actor.id != 0x4B) {
            other = (EnMb*)other->actor.next;
            continue;
        }
        if (other->actionFunc == FUN_00137164 || self->actionFunc == FUN_001375bc) {
            found = 1;
            break;
        }
        other = (EnMb*)other->actor.next;
    }

    flags = (u32*)((u8*)*(Player**)(ctx + 0xAC) + 0x2000);
    if (found) {
        flags[0x9B8 / 4] |= 0x20000;
    } else {
        flags[0x9B8 / 4] &= ~0x20000;
    }
}

void FUN_003e3578(EnMb* self) {
    FUN_003731e0(&self->skelAnime);
    if (self->timer1 != 0) {
        self->timer1--;
        if (self->timer1 != 0) {
            return;
        }
    }
    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f)) {
        FUN_00181510(self);
    }
}

void FUN_00285978(EnMb* self) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    FUN_003731e0(&self->skelAnime);
    if (self->timer1 != 0) {
        self->timer1--;
        if (self->timer1 > 0) {
            return;
        }
    }
    if (self->actor.params < 0) {
        Animation_MorphToLoop(&self->skelAnime, 1, -4.0f);
        self->actor.speedXZ = 0.0f;
        self->timer1 = Rand_S16Offset_003702c8(30, 50);
        self->state = 6;
        self->actionFunc = (EnMbActionFunc)FUN_003e3578;
    } else {
        Animation_PlayLoop(&self->skelAnime, 6);
        Audio_PlayActorSound2(&self->actor, 0x0100032A);
        self->actor.speedXZ = 10.0f;
        self->unk_902 = 1;
        self->state = 10;
        self->timer2 = 5;
        self->actionFunc = FUN_001375bc;
    }
}

void FUN_001e7634(EnMb* self) {
    self->unk_902 = 0;
    if (FUN_003731e0(&self->skelAnime)) {
        FUN_0019f61c(self);
    }
}

s32 Collider_DestroyTris(PlayState* play, void* collider);
void Collider_DestroyJntSph(PlayState* play, void* collider);
s32 FUN_0034eaf8(PlayState* play, void* collider);

void EnMb_Destroy(Actor* actor, GameState* state) {
    EnMb* self = (EnMb*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyTris(play, &self->frontShieldingCollider);
    Collider_DestroyCylinder(play, &self->bodyCollider);
    FUN_0034eaf8(play, &self->attackCollider);
    Collider_DestroyJntSph(play, &self->unk_B3C);
    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(actor, 0);
}

void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);
void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 primR,
                          s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s32 unk, s32 life, s32 playSfx);

void FUN_002501ac(EnMb* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    if (Animation_OnFrameImpl(&self->skelAnime, 32.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x0100045F);
    }
    if ((player->stateFlags2 & 0x80) && player->actor.parent == &self->actor) {
        player->stateFlags2 &= ~0x80;
        player->actor.parent = NULL;
        *(s16*)((u8*)player + 0x2238) = 200;
        FUN_00374bb8(play, &self->actor, 4.0f, self->actor.world.rot.y, 4.0f);
        self->unk_902 = 0;
    }
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->timer1 > 0) {
            Vector3f zeroVec(0.0f, 0.0f, 0.0f);
            s32 i;
            Vec3f effPos;

            self->timer1--;
            self->actor.shape.shadowScale = 0.0f;
            if ((self->timer1 % 3) != 0) {
                for (i = 4; i >= 0; i--) {
                    effPos.x = Rand_CenteredFloat(110.0f) + self->actor.world.pos.x;
                    effPos.y = Rand_CenteredFloat(15.0f) + (self->actor.world.pos.y + 20.0f);
                    effPos.z = Rand_CenteredFloat(110.0f) + self->actor.world.pos.z;
                    EffectSsDeadDb_Spawn(play, &effPos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, 100, 7, 255, 255, 255, 255, 0, 255, 0, 1, 11, 1);
                }
            }
        } else {
            Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xE0);
            Actor_Kill(&self->actor);
        }
    }
}

void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void FUN_001233b0(EnMb* self);
void FUN_002501ac(EnMb* self, PlayState* play);
void FUN_00285978(EnMb* self);
void FUN_003d4f38(EnMb* self, PlayState* play);

static inline void EnMb_SetupClubDead(EnMb* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, 22, -4.0f);
    self->state = 1;
    self->actor.speedXZ = 0.0f;
    self->actor.flags &= ~1;
    self->bodyCollider.dim.height = 80.0f;
    self->bodyCollider.dim.radius = 95.0f;
    self->timer1 = 45;
    Audio_PlayActorSound2(&self->actor, 0x0100032E);
    self->actionFunc = (EnMbActionFunc)FUN_003d4f38;
}

static inline void EnMb_SetupClubDamagedWhileKneeling(EnMb* self) {
    f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 21);

    self->state = 2;
    self->timer1 = 0;
    self->timer2 = 9;
    Animation_Change_00375c08(&self->skelAnime, 21, 1.0f, 4.0f, frames, 3, 0.0f);
    self->actionFunc = (EnMbActionFunc)FUN_001233b0;
}

static inline void EnMb_SetupClubWait(EnMb* self) {
    Animation_PlayLoop(&self->skelAnime, 14);
    self->actor.speedXZ = 0.0f;
    self->timer1 = Rand_S16Offset_003702c8(30, 50);
    self->state = 6;
    self->actionFunc = FUN_0010b5d0;
}

static inline void EnMb_SetupSpearDead(EnMb* self) {
    s16 relYaw = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (ABS(relYaw) <= 0x4000) {
        Animation_MorphToPlayOnce(&self->skelAnime, 11, -4.0f);
        self->actor.speedXZ = -8.0f;
    } else {
        Animation_MorphToPlayOnce(&self->skelAnime, 11, -4.0f);
        self->actor.speedXZ = 8.0f;
    }
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->timer1 = 45;
    self->state = 0;
    Audio_PlayActorSound2(&self->actor, 0x0100032E);
    self->actor.flags &= ~1;
    self->actionFunc = (EnMbActionFunc)FUN_002501ac;
}

static inline void EnMb_SetupSpearDamaged(EnMb* self) {
    s16 relYaw = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (ABS(relYaw) <= 0x4000) {
        Animation_MorphToPlayOnce(&self->skelAnime, 9, -4.0f);
        self->actor.speedXZ = -8.0f;
    } else {
        Animation_MorphToPlayOnce(&self->skelAnime, 10, -4.0f);
        self->actor.speedXZ = 8.0f;
    }
    self->timer1 = 8;
    self->actor.shape.rot.y = self->actor.world.rot.y;
    self->state = 0;
    Audio_PlayActorSound2(&self->actor, 0x0100032E);
    self->actionFunc = (EnMbActionFunc)FUN_00285978;
}

void FUN_00157484(EnMb* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((player->stateFlags2 & 0x80) && player->actor.parent == &self->actor) {
        player->stateFlags2 &= ~0x80;
        player->actor.parent = NULL;
        *(s16*)((u8*)player + 0x2238) = 200;
        FUN_00374bb8(play, &self->actor, 4.0f, self->actor.world.rot.y, 4.0f);
        self->unk_902 = 0;
    }
    if (*(u16*)((u8*)&self->actor + 0x11A) == 0) {
        if (*(u16*)&self->actor.params == 0) {
            if (self->actor.colChkInfo.health == 0) {
                EnMb_SetupClubDead(self);
                return;
            }
            if (self->state == 3) {
                EnMb_SetupClubDamagedWhileKneeling(self);
                return;
            }
            EnMb_SetupClubWait(self);
            return;
        } else {
            if (self->actor.colChkInfo.health == 0) {
                EnMb_SetupSpearDead(self);
                return;
            }
            EnMb_SetupSpearDamaged(self);
        }
    }
}

void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f);
void FUN_0036627c(void* camera, s32 a, s32 b, s32 c);

void FUN_003d4f38(EnMb* self, PlayState* play) {
    Vec3f effPos;
    Vec3f effPosBase;

    effPos = self->actor.world.pos;
    effPos.x += Math_SinS(self->actor.shape.rot.y) * -70.0f;
    effPos.z += Math_CosS(self->actor.shape.rot.y) * -70.0f;
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    effPosBase = effPos;
    if (Animation_OnFrameImpl(&self->skelAnime, 46.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, 0x0100045F);
    }
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->timer1 > 0) {
            Vector3f zeroVec(0.0f, 0.0f, 0.0f);
            s32 i;

            self->timer1--;
            if ((self->timer1 % 3) != 0) {
                for (i = 4; i >= 0; i--) {
                    effPos.x = Rand_CenteredFloat(240.0f) + effPosBase.x;
                    effPos.y = Rand_CenteredFloat(15.0f) + (effPosBase.y + 20.0f);
                    effPos.z = Rand_CenteredFloat(240.0f) + effPosBase.z;
                    EffectSsDeadDb_Spawn(play, &effPos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, 230, 7, 255, 255, 255, 255, 0, 255, 0, 1, 11, 1);
                }
            }
        } else {
            Item_DropCollectibleRandom(play, &self->actor, &effPos, 0xC0);
            Actor_Kill(&self->actor);
        }
    } else if (Animation_OnFrameImpl(&self->skelAnime, 10.0f, 1.0f) || Animation_OnFrameImpl(&self->skelAnime, 18.0f, 1.0f)) {
        FUN_0036f00c(play, &self->actor, &effPos, 50.0f, 10, 3.0f, 400, 60, 0);
        Audio_PlayActorSound2(&self->actor, 0x010002EA);
        FUN_0036627c((u8*)play + 0x364, 2, 25, 5);
    }
}

void FUN_0036cbc4(EnMb* self, PlayState* play, s32 flag);
void FUN_0010b9f8(EnMb* self, PlayState* play);

void FUN_0011b9e8(EnMb* self, PlayState* play);
void FUN_00137164(EnMb* self, PlayState* play);

void FUN_0036ccb8(EnMb* self) {
    f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 5);

    Animation_MorphToPlayOnce(&self->skelAnime, 5, -4.0f);
    self->actor.speedXZ = 0.0f;
    self->state = 10;
    {
        s32 t;
        if (6 + (s16)(s32)frames > 0) {
            t = (s32)(0.5f + ((f32)(6 + (s16)(s32)frames) * 3.0f) * 0.5f);
        } else {
            t = (s32)(((f32)(6 + (s16)(s32)frames) * 3.0f) * 0.5f - 0.5f);
        }
        self->timer2 = t;
    }
    Audio_PlayActorSound2(&self->actor, 0x0100032C);
    if (*(s16*)&self->actor.params == -1) {
        self->actionFunc = (EnMbActionFunc)FUN_0011b9e8;
    } else {
        self->actionFunc = FUN_00137164;
    }
}

static inline Actor* EnMb_SpawnAsChild(ActorContext* actorCtx, Actor* parent, PlayState* play, s16 actorId, f32 posX, f32 posY,
                                       f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params) {
    Actor* child = Actor_Spawn(actorCtx, play, actorId, posX, posY, posZ, rotX, rotY, rotZ, params, 1);

    if (child != NULL) {
        parent->child = child;
        child->parent = parent;
        if (child->room >= 0) {
            child->room = parent->room;
        }
    }
    return child;
}

void FUN_003cd6d8(Actor* parent, Vec3f* pos, Vec3s* rot, Vec3f* vel, s16 arg4, s32 unk5, PlayState* play, s16 params) {
    Actor* child = EnMb_SpawnAsChild(&play->actorCtx, parent, play, 7, pos->x, pos->y, pos->z, rot->x, rot->y,
                                     parent->objBankIndex, params);

    if (child != NULL) {
        child->scale = parent->scale;
        child->speedXZ = vel->x;
        *(u8*)((u8*)child + 0x1D1) = 2;
        { s16* p = (s16*)((u8*)child + 0x1D2); *p = arg4; }
        *(f32*)((u8*)child + 0x1D4) = vel->y;
        *(f32*)((u8*)child + 0x1D8) = vel->z;
    }
}

void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f);
void FUN_003e9230(EnMb* self, PlayState* play);

void FUN_001c698c(EnMb* self, PlayState* play) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.5f, 1.0f, 0.0f);
    if (self->actor.speedXZ > 1.0f) {
        FUN_0036f00c(play, &self->actor, &self->actor.world.pos, 5.0f, 3, 4.0f, 100, 15, 0);
    }
    if (FUN_003731e0(&self->skelAnime)) {
        if (self->timer1 == 0) {
            self->timer2--;
            if (self->timer2 == 0) {
                f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 5);
                Animation_Change_00375c08(&self->skelAnime, 5, -1.0f, frames, 0.0f, 2, 0.0f);
                self->timer1 = 1;
                self->actor.speedXZ = 0.0f;
                Audio_PlayActorSound2(&self->actor, 0x0100032D);
            }
        } else if (self->actor.params < 0) {
            f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 3);
            Animation_Change_00375c08(&self->skelAnime, 3, 0.0f, 0.0f, frames, 0, -4.0f);
            self->actor.speedXZ = 0.59999996f;
            Rand_S16Offset_003702c8(50, 70);
            self->unk_900 = 1;
            self->state = 9;
            self->actionFunc = FUN_003e9230;
            self->timer1 = self->timer3 = self->timer2 = 120;
        } else {
            FUN_0036cbc4(self, play, 0);
        }
    }
}

void FUN_001c698c(EnMb* self, PlayState* play);

void FUN_0011b9e8(EnMb* self, PlayState* play) {
    s16 yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    s32 prevFrame;
    s32 curFrame;
    s16 absDiff;

    absDiff = ABS(yawDiff);
    prevFrame = (s32)self->skelAnime.curFrame;
    if (FUN_003731e0(&self->skelAnime)) {
        Animation_PlayLoop(&self->skelAnime, 6);
        Audio_PlayActorSound2(&self->actor, 0x0100032A);
    }
    if (self->timer2 != 0) {
        self->timer2--;
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 3000, 0);
    } else {
        self->actor.speedXZ = 10.0f;
        self->unk_902 = 1;
        FUN_0036f00c(play, &self->actor, &self->actor.world.pos, 5.0f, 3, 4.0f, 100, 15, 0);
        if ((s32)self->skelAnime.curFrame != prevFrame) {
            if ((s32)self->skelAnime.curFrame == 2 || (s32)self->skelAnime.curFrame == 6) {
                Audio_PlayActorSound2(&self->actor, 0x0100032F);
            }
        }
    }
    if (absDiff > 5000) {
        self->unk_902 = 0;
        Animation_PlayOnce(&self->skelAnime, 7);
        self->state = 11;
        self->timer1 = 0;
        self->timer2 = 8;
        Audio_PlayActorSound2(&self->actor, 0x01000329);
        self->actionFunc = (EnMbActionFunc)FUN_001c698c;
    }
}

f32 FUN_0036d260(Vec3f* a, Vec3f* b);
void FUN_00136c68(EnMb* self, PlayState* play);
void FUN_001c6138(EnMb* self, PlayState* play);

void FUN_0010b5d0(EnMb* self, PlayState* play) {
    Actor* thisx = &self->actor;
    Player* player = GET_PLAYER(play);
    s16 relYawFromPlayer = self->actor.world.rot.y - self->actor.yawTowardsPlayer;

    FUN_003731e0(&self->skelAnime);
    if (FUN_0036d260(&self->actor.home.pos, &player->actor.world.pos) < self->playerDetectionRange &&
        !(player->stateFlags1 & 0x4000000) && ABS(relYawFromPlayer) < 0x3E80) {
        f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 15);
        s16 relYaw;
        s32 st = 10;

        self->state = st;
        Animation_Change_00375c08(&self->skelAnime, 15, 2.4f, 0.0f, frames, 3, 0.0f);
        self->timer2 = 1;
        relYaw = thisx->world.rot.y - self->actor.yawTowardsPlayer;
        if (ABS(relYaw) <= 0x258) {
            self->unk_902 = 2;
        } else if (relYaw >= 0) {
            self->unk_902 = 1;
        } else {
            self->unk_902 = 3;
        }
        self->actionFunc = (EnMbActionFunc)FUN_00136c68;
    }
    if (FUN_0036d260(&self->actor.home.pos, &player->actor.world.pos) < self->playerDetectionRange + 80.0f &&
        ABS(relYawFromPlayer) < 0x3E80 && !(*(u32*)&gSaveContext.unk_13D0[4] & 1)) {
        *(u32*)&gSaveContext.unk_13D0[4] |= 1;
        self->csState = 0;
        Actor_ChangeType(play, &play->actorCtx, &self->actor, 6);
        self->actionFunc = (EnMbActionFunc)FUN_001c6138;
    }
}

s32 FUN_0036cda4(EnMb* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 angle = 0;
    s16 rotY = self->actor.world.rot.y;
    f32 cosA;
    f32 sinA;
    f32 halfX;
    f32 halfZ;
    f32 dx;
    f32 dz;

    if (rotY <= -25251 || rotY >= 25251) {
        angle = (s16)0x8000;
    } else if (rotY <= -8417) {
        angle = (s16)~(((u32)-8417) >> 18);
    } else if (rotY >= 8417) {
        angle = 0x4000;
    }
    cosA = Math_CosS(angle);
    sinA = Math_SinS(angle);
    if (cosA >= 0.0f) { } else { cosA = -cosA; }
    if (sinA >= 0.0f) { } else { sinA = -sinA; }
    halfX = cosA * 38.0f;
    halfX += sinA * 400.0f;
    halfZ = sinA * 38.0f + cosA * 400.0f;
    dx = self->actor.world.pos.x - player->actor.world.pos.x;
    dz = self->actor.world.pos.z - player->actor.world.pos.z;
    if (dx >= 0.0f) { } else { dx = -dx; }
    if (dx < halfX) {
        if (dz >= 0.0f) { } else { dz = -dz; }
        if (dz < halfZ) {
            return 1;
        }
    }
    return 0;
}

extern ColliderCylinderInit EnMb_sBodyColliderInit;
extern u8 EnMb_sJntSphInit[];
extern u8 EnMb_sTrisInit[];
extern u8 EnMb_sQuadInit[];
extern DamageTable EnMb_sSpearDamageTable;
extern DamageTable EnMb_sClubDamageTable;
extern InitChainEntry EnMb_sInitChain[];

void ActorShadow_DrawFeet(Actor* actor, Lights* lights, PlayState* play);
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* src, void* elements);
void Collider_InitTris(PlayState* play, void* collider);
void Collider_SetTris(PlayState* play, void* collider, Actor* actor, void* src, void* elements);
void Collider_InitQuad(PlayState* play, void* collider);
void Collider_SetQuad(PlayState* play, void* collider, Actor* actor, void* src);
void FUN_0036cbc4(EnMb* self, PlayState* play, s32 flag);
void FUN_00159734(EnMb* self, PlayState* play);
void FUN_0024cdec(void);
void FUN_003e3578(EnMb* self);

static inline void EnMb_SetupSpearGuardLookAround(EnMb* self) {
    Animation_MorphToLoop(&self->skelAnime, 1, -4.0f);
    self->actor.speedXZ = 0.0f;
    self->timer1 = Rand_S16Offset_003702c8(30, 50);
    self->state = 6;
    self->actionFunc = (EnMbActionFunc)FUN_003e3578;
}

void EnMb_Init(Actor* thisx, GameState* state) {
    EnMb* self = (EnMb*)thisx;
    Player* player = GET_PLAYER((PlayState*)state);
    PlayState* play = (PlayState*)state;
    s16 relYawFromPlayer;

    Actor_ProcessInitChain(&self->actor, EnMb_sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 46.0f);
    self->actor.colChkInfo.mass = 0xFF;
    self->actor.colChkInfo.damageTable = &EnMb_sSpearDamageTable;
    Collider_InitCylinder(play, &self->bodyCollider);
    Collider_SetCylinder(play, &self->bodyCollider, &self->actor, &EnMb_sBodyColliderInit);
    Collider_InitJntSph(play, &self->unk_B3C);
    Collider_SetJntSph(play, &self->unk_B3C, &self->actor, EnMb_sJntSphInit, (u8*)self + 0xB5C);
    Collider_InitTris(play, &self->frontShieldingCollider);
    Collider_SetTris(play, &self->frontShieldingCollider, &self->actor, EnMb_sTrisInit, (u8*)self + 0xA2C);
    Collider_InitQuad(play, &self->attackCollider);
    Collider_SetQuad(play, &self->attackCollider, &self->actor, EnMb_sQuadInit);
    FUN_00372f38(&self->actor, play, 0);

    switch (self->actor.params) {
        case -1:
            FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, (u8*)self + 0x268, (u8*)self + 0x5A8, 16);
            thisx->colChkInfo.health = 2;
            self->actor.colChkInfo.mass = 0xFE;
            self->maxHomeDist = 1000.0f;
            self->playerDetectionRange = 1750.0f;
            EnMb_SetupSpearGuardLookAround(self);
            break;
        case -0x8000:
            Actor_ChangeType(play, &play->actorCtx, &self->actor, 1);
            thisx->flags &= ~1;
            self->actionFunc = (EnMbActionFunc)FUN_00159734;
            self->actor.draw = (ActorFunc)FUN_0024cdec;
            break;
        case 0:
            FUN_00353c9c(&self->actor, play, &self->skelAnime, 1, 14, (u8*)self + 0x268, (u8*)self + 0x5A8, 16);
            self->actor.colChkInfo.health = 6;
            self->actor.colChkInfo.mass = 0xFF;
            self->actor.colChkInfo.damageTable = &EnMb_sClubDamageTable;
            Actor_SetScale(&self->actor, 0.02f);
            self->bodyCollider.dim.height = 120.0f;
            self->bodyCollider.dim.radius = 45.0f;
            *(f32*)((u8*)self->unk_B3C.elements + 0x44) *= 1.8f;
            *(f32*)((u8*)self->unk_B3C.elements + 0x34) *= 1.8f;
            self->actor.uncullZoneForward = 4000.0f;
            self->actor.uncullZoneScale = 800.0f;
            self->actor.uncullZoneDownward = 1800.0f;
            self->playerDetectionRange = 710.0f;
            *(u32*)((u8*)self + 0x9A4) = 0x20000000;
            relYawFromPlayer = self->actor.world.rot.y - Math_Vec3f_Yaw(&self->actor.world.pos, &player->actor.world.pos);
            if (ABS(relYawFromPlayer) > 0x4000) {
                self->actor.world.rot.y = thisx->world.rot.y + 0x8000;
                self->actor.shape.rot.y = thisx->world.rot.y;
                self->actor.world.pos.z = thisx->world.pos.z + 600.0f;
            }
            ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);
            thisx->flags &= ~1;
            self->actor.naviEnemyId += 1;
            EnMb_SetupClubWait(self);
            break;
        default:
            FUN_00353c9c(thisx, play, &self->skelAnime, 0, 0, (u8*)self + 0x268, (u8*)self + 0x5A8, 16);
            Actor_SetScale(&self->actor, 0.014f);
            self->unk_929 = (u16)self->actor.params >> 8;
            self->actor.params = 1;
            self->unk_928 = 0;
            thisx->colChkInfo.health = 1;
            self->actor.colChkInfo.mass = 0xFE;
            self->maxHomeDist = 350.0f;
            self->playerDetectionRange = 1750.0f;
            thisx->flags &= ~1;
            FUN_0036cbc4(self, play, 1);
            break;
    }
}

void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
u8 Actor_ApplyDamage(Actor* actor);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);
void FUN_00123310(EnMb* self);
void FUN_00157484(EnMb* self, PlayState* play);

static inline void EnMb_SetupClubDamaged(EnMb* self) {
    Animation_PlayOnce(&self->skelAnime, 18);
    self->state = 3;
    self->timer1 = 14;
    self->timer2 = 14;
    Audio_PlayActorSound2(&self->actor, 0x0100032E);
    self->actionFunc = (EnMbActionFunc)FUN_00123310;
}

static inline void EnMb_SetupStunned(EnMb* self) {
    self->actor.speedXZ = 0.0f;
    self->state = 5;
    Actor_SetColorFilter(&self->actor, 0, 120, 0, 80);
    if (self->damageReaction == 6) {
        self->iceEffectTimer = 60;
    } else {
        if (*(u16*)&self->actor.params != 0) {
            Animation_PlayOnceSetSpeed(&self->skelAnime, 9, 0.0f);
        }
        Audio_PlayActorSound2(&self->actor, 0x0100030E);
    }
    self->actionFunc = (EnMbActionFunc)FUN_00157484;
}

#if 0 /* EnMb_Update: unmatched draft, see STATUS.md */
void EnMb_Update(EnMb* self, PlayState* play) {
    Actor* thisx = &self->actor;
    EnMb* s2 = self;
    u8 fl = self->frontShieldingCollider.base.acFlags;
    Player* player = GET_PLAYER(play);

    if (fl & 2) {
        s2->frontShieldingCollider.base.acFlags &= ~0x82;
        self->bodyCollider.base.acFlags &= ~2;
    } else if ((self->bodyCollider.base.acFlags & 2) || (self->unk_B3C.elements->bumperFlags & 2) || self->unk_8FC != 0) {
        self->bodyCollider.base.acFlags &= ~2;
        if (self->state > 4) {
            if (self->actor.colChkInfo.damageEffect != 0 && self->actor.colChkInfo.damageEffect != 5) {
                if ((*(u32*)&gSaveContext.unk_13D0[4] & 1) || *(u16*)&self->actor.params != 0) {
                    if ((player->stateFlags2 & 0x80) && player->actor.parent == &self->actor) {
                        player->stateFlags2 &= ~0x80;
                        player->actor.parent = NULL;
                        *(s16*)((u8*)player + 0x2238) = 200;
                        FUN_00374bb8(play, &self->actor, 6.0f, self->actor.world.rot.y, 6.0f);
                    }
                    self->damageReaction = self->actor.colChkInfo.damageEffect;
                    self->unk_902 = 0;
                    self->unk_8FC = 0;
                    FUN_00375fd0(&self->actor, &self->bodyCollider.info, 0);
                    if (self->actor.colChkInfo.damageEffect == 1 || self->actor.colChkInfo.damageEffect == 6) {
                        if (self->state != 5) {
                            Actor_ApplyDamage(&self->actor);
                            EnMb_SetupStunned(self);
                        }
                    } else {
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x400000, 250, 0, 12);
                        if (*(u16*)&self->actor.params == 0) {
                            if (self->actor.colChkInfo.health == 0) {
                                EnMb_SetupClubDead(self);
                            } else {
                                EnMb_SetupClubDamaged(self);
                            }
                        } else {
                            if (self->actor.colChkInfo.health == 0) {
                                EnMb_SetupSpearDead(self);
                            } else {
                                EnMb_SetupSpearDamaged(self);
                            }
                        }
                    }
                }
            }
        }
    }
    if (self->actor.colChkInfo.damageEffect != 5) {
        { PlayState* play2 = play; self->actionFunc(self, play2); }
        Actor_MoveForward(thisx);
        Actor_UpdateBgCheckInfo(play, &self->actor, 40.0f, 40.0f, 70.0f, 0x1D);
        Actor_SetFocus(&self->actor, self->actor.scale.x * 4500.0f);
        Collider_UpdateCylinder(&self->actor, &self->bodyCollider);
        if (self->actor.colChkInfo.health == 0) {
            self->bodyCollider.dim.pos.x += Math_SinS(self->actor.shape.rot.y) * -2400.0f * self->actor.scale.y;
            self->bodyCollider.dim.pos.z += Math_CosS(self->actor.shape.rot.y) * -2400.0f * self->actor.scale.y;
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->bodyCollider.base);
        if (self->state > 4) {
            if (*(u16*)&self->actor.params == 0 || self->state != 10) {
                CollisionCheck_SetAC(play, &play->colChkCtx, &self->bodyCollider.base);
                CollisionCheck_SetAC(play, &play->colChkCtx, &self->unk_B3C.base);
            }
        }
        if (self->unk_902 > 0) {
            if (self->state > 5 || self->unk_8FC != 0) {
                CollisionCheck_SetAC(play, &play->colChkCtx, &self->frontShieldingCollider.base);
            }
            if (self->unk_902 > 0) {
                CollisionCheck_SetAT(play, &play->colChkCtx, &self->attackCollider.base);
            }
        }
    }
}
#endif
GLOBAL_ASM("asm/EnMb_Update.s")


typedef struct EnMbTriModel {
    Vec3f v[3];
} EnMbTriModel;

typedef struct EnMbTriModels {
    EnMbTriModel model0;
    EnMbTriModel model1;
} EnMbTriModels;

extern const EnMbTriModels EnMb_sFrontShieldingTriModels;

void Collider_SetTrisVertices(void* collider, s32 index, Vec3f* a, Vec3f* b, Vec3f* c);
void FUN_0035479c(void* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d);
void FUN_00347d24(PlayState* play, Actor* actor, Vec3s* pos, s32 primR, s32 primG, s32 primB, s32 primA, s32 envR, s32 envG,
                  s32 envB, f32 scale);
void FUN_00233bb4(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

static void EnMb_SpearUpdateAttackCollider(Actor* thisx, PlayState* play) {
    Vector3f quadModel0(1000.0f, 1500.0f, 0.0f);
    Vector3f quadModel1(-1000.0f, 1500.0f, 0.0f);
    Vector3f quadModel2(1000.0f, 1500.0f, 4500.0f);
    Vector3f quadModel3(-1000.0f, 1500.0f, 4500.0f);
    EnMb* self = (EnMb*)thisx;
    MTX34* mtx = &thisx->modelMtx;

    if (self->actor.params > 0) {
        quadModel0.x += 2000.0f;
        quadModel0.z = -4000.0f;
        quadModel1.z = -4000.0f;
        quadModel2.x += 2000.0f;
        quadModel1.x -= 2000.0f;
        quadModel3.x -= 2000.0f;
        quadModel2.z += 4000.0f;
        quadModel3.z += 4000.0f;
    }
    VEC3TransformAsm((Vector3f*)&self->attackCollider.quad[1], mtx, &quadModel0);
    VEC3TransformAsm((Vector3f*)&self->attackCollider.quad[0], mtx, &quadModel1);
    VEC3TransformAsm((Vector3f*)&self->attackCollider.quad[3], mtx, &quadModel2);
    VEC3TransformAsm((Vector3f*)&self->attackCollider.quad[2], mtx, &quadModel3);
    FUN_0035479c(&self->attackCollider, &self->attackCollider.quad[0], &self->attackCollider.quad[1],
                 &self->attackCollider.quad[2], &self->attackCollider.quad[3]);
}

#if 0 /* FUN_00233bb4: unmatched draft, see STATUS.md */
void FUN_00335044(Actor* actor, s32 limbIndex, s32 leftFootLimb, Vec3f* leftFootPos, s32 rightFootLimb, Vec3f* rightFootPos);
void Collider_UpdateSpheres(s32 limbIndex, void* collider, MTX34* mtx);

static void EnMb_ClubUpdateAttackCollider(Actor* thisx, PlayState* play, MTX34* mtx) {
    static Vector3f quadModel0(1000.0f, 0.0f, 0.0f);
    static Vector3f quadModel1(1000.0f, 0.0f, 0.0f);
    static Vector3f quadModel2(1000.0f, -8000.0f, -1500.0f);
    static Vector3f quadModel3(1000.0f, -9000.0f, 2000.0f);
    EnMb* self = (EnMb*)thisx;

    VEC3TransformAsm((Vector3f*)&self->attackCollider.quad[1], mtx, &quadModel0);
    VEC3TransformAsm((Vector3f*)&self->attackCollider.quad[0], mtx, &quadModel1);
    VEC3TransformAsm((Vector3f*)&self->attackCollider.quad[3], mtx, &quadModel2);
    VEC3TransformAsm((Vector3f*)&self->attackCollider.quad[2], mtx, &quadModel3);
    FUN_0035479c(&self->attackCollider, &self->attackCollider.quad[0], &self->attackCollider.quad[1],
                 &self->attackCollider.quad[2], &self->attackCollider.quad[3]);
}

void FUN_00233bb4(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    s32 bodyPart = -1;
    static Vector3f unused(1100.0f, -700.0f, 0.0f);
    static Vector3f feetPos(0.0f, 0.0f, 0.0f);
    static Vector3f effSpawnOffsetFromLeftHand(1200.0f, -5500.0f, 0.0f);
    static Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    EnMb* self = (EnMb*)thisx;

    if (*(u16*)&self->actor.params == 0) {
        if (limbIndex == 9) {
            VEC3TransformAsm((Vector3f*)&self->effSpawnPos, mtx, &effSpawnOffsetFromLeftHand);
            if (self->unk_902 > 0) {
                EnMb_ClubUpdateAttackCollider((Actor*)thisx, play, mtx);
            }
        }
        FUN_00335044(&self->actor, limbIndex, 22, (Vec3f*)&feetPos, 27, (Vec3f*)&feetPos);
    }
    if (limbIndex == 3) {
        Collider_UpdateSpheres(0, &self->unk_B3C, mtx);
    }
    if (self->iceEffectTimer != 0) {
        switch (limbIndex) {
            case 3:
                bodyPart = 0;
                break;
            case 6:
                bodyPart = 1;
                break;
            case 9:
                bodyPart = 2;
                break;
            case 4:
                bodyPart = 3;
                break;
            case 7:
                bodyPart = 4;
                break;
            case 2:
                bodyPart = 5;
                break;
            case 12:
                bodyPart = 8;
                break;
            case 15:
                bodyPart = 9;
                break;
        }
        if (bodyPart >= 0) {
            Vec3f bodyPartPos;

            VEC3TransformAsm((Vector3f*)&bodyPartPos, mtx, &zeroVec);
            self->bodyPartsPos[bodyPart].x = bodyPartPos.x;
            self->bodyPartsPos[bodyPart].y = bodyPartPos.y;
            self->bodyPartsPos[bodyPart].z = bodyPartPos.z;
        }
    }
}
#endif

void FUN_00136c68(EnMb* self, PlayState* play);

static inline void EnMb_CsApproach(EnMb* self) {
    Math_SmoothStepToF(&self->csEye.x, self->csEyeTarget.x, 0.1f, self->csStep, 0.0f);
    Math_SmoothStepToF(&self->csEye.y, self->csEyeTarget.y, 0.1f, self->csStep, 0.0f);
    Math_SmoothStepToF(&self->csEye.z, self->csEyeTarget.z, 0.1f, self->csStep, 0.0f);
    Math_SmoothStepToF(&self->csAt.x, self->csAtTarget.x, 0.1f, self->csStep, 0.0f);
    Math_SmoothStepToF(&self->csAt.y, self->csAtTarget.y, 0.1f, self->csStep, 0.0f);
    Math_SmoothStepToF(&self->csAt.z, self->csAtTarget.z, 0.1f, self->csStep, 0.0f);
    Math_SmoothStepToF(&self->csStep, 50.0f, 0.1f, 5.0f, 0.0f);
}

void FUN_001c6138(EnMb* self, PlayState* play) {
    if (self->csState >= 2) {
        if (Animation_OnFrameImpl(&self->skelAnime, 16.0f, 1.0f)) {
            Audio_PlayActorSound2(&self->actor, 0x0100032F);
        } else if (Animation_OnFrameImpl(&self->skelAnime, 38.0f, 1.0f)) {
            Audio_PlayActorSound2(&self->actor, 0x01000469);
        } else if (Animation_OnFrameImpl(&self->skelAnime, 82.0f, 1.0f)) {
            Audio_PlayActorSound2(&self->actor, 0x01000385);
        }
    }
    switch (self->csState) {
        case 0: {
            f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 25);
            Camera* cam;

            Animation_Change_00375c08(&self->skelAnime, 25, 1.0f, 0.0f, frames, 3, 0.0f);
            cam = Gameplay_GetCamera(play, -1);
            self->csSavedEye = cam->eye;
            self->csSavedAt = cam->at;
            self->csCamId = Gameplay_CreateSubCamera(play);
            Gameplay_ChangeCameraStatus(play, 0, 1);
            Gameplay_ChangeCameraStatus(play, self->csCamId, 7);
            FUN_0036e980(play, &self->actor, 5);
            self->csEye = cam->eye;
            self->csAt = cam->at;
            self->csAtTarget.x = 16.0f;
            self->csAtTarget.y = 284.0f;
            self->csAtTarget.z = -1372.0f;
            self->csEyeTarget.x = 24.0f;
            self->csEyeTarget.y = 252.0f;
            self->csEyeTarget.z = -1244.0f;
            self->csTimer = 20;
            self->csState = 1;
            break;
        }
        case 1:
            FUN_0036d260(&self->csEye, &self->actor.world.pos);
            FUN_0036d260(&self->csAt, &self->actor.world.pos);
            EnMb_CsApproach(self);
            if (--self->csTimer == 0) {
                f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 25);
                s32 t;

                if (frames > 0.0f) {
                    t = (s32)(0.5f + (frames * 3.0f) * 0.5f);
                } else {
                    t = (s32)((frames * 3.0f) * 0.5f - 0.5f);
                }
                self->csTimer = t - 40;
                self->csState = 2;
            }
            break;
        case 2:
            FUN_003731e0(&self->skelAnime);
            FUN_0036d260(&self->csEye, &self->actor.world.pos);
            FUN_0036d260(&self->csAt, &self->actor.world.pos);
            EnMb_CsApproach(self);
            if (--self->csTimer == 0) {
                self->csTimer = 30;
                self->csState = 3;
            }
            break;
        case 3:
            FUN_003731e0(&self->skelAnime);
            Math_SmoothStepToF(&self->csAt.x, self->csSavedAt.x, 0.2f, 300.0f, 0.0f);
            Math_SmoothStepToF(&self->csAt.y, self->csSavedAt.y, 0.2f, 300.0f, 0.0f);
            Math_SmoothStepToF(&self->csAt.z, self->csSavedAt.z, 0.2f, 300.0f, 0.0f);
            Math_SmoothStepToF(&self->csEye.x, self->csSavedEye.x, 0.2f, 300.0f, 0.0f);
            Math_SmoothStepToF(&self->csEye.y, self->csSavedEye.y, 0.2f, 300.0f, 0.0f);
            Math_SmoothStepToF(&self->csEye.z, self->csSavedEye.z, 0.2f, 300.0f, 0.0f);
            if (--self->csTimer == 0) {
                Camera* cam = Gameplay_GetCamera(play, 0);

                cam->eye = self->csEye;
                cam->eyeNext = self->csEye;
                cam->at = self->csAt;
                FUN_0036e9b8(play, self->csCamId, 0);
                self->csCamId = 0;
                Actor_ChangeType(play, &play->actorCtx, &self->actor, 5);
                FUN_0036e980(play, &self->actor, 7);
                self->csState = 4;
                return;
            }
            break;
        case 4:
            if (FUN_003731e0(&self->skelAnime)) {
                self->csState = 5;
                return;
            }
            break;
        case 5: {
            f32 frames = (f32)Animation_GetLastFrame(&self->skelAnime, 15);
            s16 relYaw;

            self->state = 10;
            Animation_Change_00375c08(&self->skelAnime, 15, 2.4f, 0.0f, frames, 3, 0.0f);
            self->timer2 = 1;
            relYaw = self->actor.world.rot.y - self->actor.yawTowardsPlayer;
            if (ABS(relYaw) <= 0x258) {
                self->unk_902 = 2;
            } else if (relYaw >= 0) {
                self->unk_902 = 1;
            } else {
                self->unk_902 = 3;
            }
            self->actionFunc = (EnMbActionFunc)FUN_00136c68;
            break;
        }
    }
    if (self->csState < 4) {
        Gameplay_CameraSetAtEye(play, self->csCamId, &self->csAt, &self->csEye);
    }
}

#if 0 /* EnMb_Draw: unmatched draft, see STATUS.md */
void EnMb_Draw(Actor* thisx, GameState* state) {
    EnMb* self = (EnMb*)thisx;
    PlayState* play = (PlayState*)state;
    Vec3f frontShieldingTri0[3];
    Vec3f frontShieldingTri1[3];
    EnMbTriModel frontShieldingTriModel0;
    EnMbTriModel frontShieldingTriModel1;
    s32 i;
    f32 scale;
    s32 bodyPartIdx;

    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, NULL, (PostLimbDrawOpa)FUN_00233bb4, thisx, 0);
    if (thisx->params != 0) {
        frontShieldingTriModel0 = EnMb_sFrontShieldingTriModels.model0;
        frontShieldingTriModel1 = EnMb_sFrontShieldingTriModels.model1;
        if (self->unk_902 > 0) {
            EnMb_SpearUpdateAttackCollider(thisx, play);
        }
        for (i = 0; i < 3; i++) {
            VEC3TransformAsm((Vector3f*)&frontShieldingTri0[i], &self->actor.modelMtx, (Vector3f*)&frontShieldingTriModel0.v[i]);
            VEC3TransformAsm((Vector3f*)&frontShieldingTri1[i], &self->actor.modelMtx, (Vector3f*)&frontShieldingTriModel1.v[i]);
        }
        Collider_SetTrisVertices(&self->frontShieldingCollider, 0, &frontShieldingTri0[0], &frontShieldingTri0[1],
                                 &frontShieldingTri0[2]);
        Collider_SetTrisVertices(&self->frontShieldingCollider, 1, &frontShieldingTri1[0], &frontShieldingTri1[1],
                                 &frontShieldingTri1[2]);
    }
    if (self->iceEffectTimer != 0) {
        *(u16*)((u8*)&self->actor + 0x11A) += 1;
        if (self->iceEffectTimer >= 0) {
            self->iceEffectTimer--;
        }
        if ((self->iceEffectTimer & 5) == 0) {
            scale = 2.5f;
            if (self->actor.params == 0) {
                scale = 4.0f;
            }
            bodyPartIdx = self->iceEffectTimer >> 2;
            FUN_00347d24(play, thisx, &self->bodyPartsPos[bodyPartIdx], 150, 150, 150, 250, 235, 245, 255, scale);
        }
    }
}
#endif
GLOBAL_ASM("asm/EnMb_Draw.s")


}
