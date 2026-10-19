extern "C" {
#include "z_en_geldb.hpp"

#define FLAGS 0x00000015

#define BGCHECKFLAG_GROUND 0x0001
#define BGCHECKFLAG_GROUND_TOUCH 0x0002
#define BGCHECKFLAG_WALL 0x0008
void EnGeldB_Advance(EnGeldB* self, PlayState* play);
void EnGeldB_RollForward(EnGeldB* self, PlayState* play);
void EnGeldB_RollBack(EnGeldB* self, PlayState* play);
void EnGeldB_Jump(EnGeldB* self, PlayState* play);
void EnGeldB_Ready(EnGeldB* self, PlayState* play);
void EnGeldB_Defeated(Actor* thisx, PlayState* play);
void EnGeldB_Flee(EnGeldB* self, PlayState* play);
void EnGeldB_SpinDodge(EnGeldB* self, PlayState* play);
s32 EnGeldB_ReactToPlayer(PlayState* play, EnGeldB* self, s16 arg2);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void FUN_00373d0c(PlayState* play);
void FUN_0034df48(void* a, void* b);
void Effect_Delete(PlayState* play, s32 index);
void Collider_DestroyTris(PlayState* play, void* collider);
void Collider_DestroyQuad(PlayState* play, void* collider);
void SkelAnime_Free2(SkelAnime* skelAnime);
s32 FUN_00369608(PlayState* play, Actor* actor);
Actor* FUN_00346e2c(PlayState* play, Actor* actor, f32 range, f32 range2);
s16 Actor_WorldYawTowardActor(Actor* actor, Actor* other);
f32 Actor_WorldDistXYZToPoint(Actor* actor, Vec3f* point);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
void FUN_0034c128(PlayState* play, Vec3f* pos);
void FUN_003ff758(Vec3f* pos, s32 sfxId);
void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f);
void EnGeldB_SetupReady(EnGeldB* self);
void EnGeldB_SetupSideStep(EnGeldB* self, PlayState* play);
void EnGeldB_SetupJump(EnGeldB* self);
void EnGeldB_SetupBlock(EnGeldB* self);
void EnGeldB_SetupRollBack(EnGeldB* self);
void EnGeldB_Slash(EnGeldB* self, PlayState* play);
void EnGeldB_SpinAttack(EnGeldB* self, PlayState* play);
void EnGeldB_Block(EnGeldB* self, PlayState* play);
void EnGeldB_Circle(EnGeldB* self, PlayState* play);
void EnGeldB_Sidestep(EnGeldB* self, PlayState* play);
void EnGeldB_SetupCircle(EnGeldB* self);
s32 EnGeldB_DodgeRanged(PlayState* play, EnGeldB* self);
void FUN_0034f724(PlayState* play);

static inline void EnGeldB_SetupAction(EnGeldB* self, EnGeldBActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnGeldB_Init(Actor* actor, GameState* state);
void EnGeldB_Destroy(Actor* actor, PlayState* state);
void EnGeldB_Update(Actor* actor, GameState* state);
void EnGeldB_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_GeldB.data.s")

ActorInit En_GeldB_InitVars = {
    ACTOR_EN_GELDB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GELDB,
    sizeof(EnGeldB),
    (ActorFunc)EnGeldB_Init,
    (ActorFunc)EnGeldB_Destroy,
    (ActorFunc)EnGeldB_Update,
    (ActorFunc)EnGeldB_Draw,
};

GLOBAL_ASM("asm/EnGeldB_Init.s")

// Collider_DestroyCylinder / FUN_0034eaf8 (Collider_DestroyQuad) are 8-byte `return 1` stubs that armlink inlines (dead `mov r0,#1`).
s32 Collider_DestroyCylinder(PlayState* play, ColliderCylinder* collider);
s32 FUN_0034eaf8(PlayState* play, void* quad);

void EnGeldB_Destroy(Actor* thisx, PlayState* play) {
    EnGeldB* self = (EnGeldB*)thisx;

    FUN_00373d0c(play);
    Effect_Delete(play, self->blureIndex);
    Collider_DestroyTris(play, &self->blockCollider);
    Collider_DestroyCylinder(play, &self->bodyCollider);
    FUN_0034eaf8(play, &self->swordCollider);
    ((FaceAnimations*)self->faceAnimations)->Destroy();
    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(thisx, 0);
}

#if 0 // draft (Update with CollisionCheck/TurnHead inlined), ~100 words off: play / self+0xB00 base callee-saved regs swapped (ROM: base r7, play r8), lookTimer rounding is conditional (vnmlsle/vmlagt) in the ROM, Defeated/Damaged setup stores order
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
u8 Actor_ApplyDamage(Actor* actor);
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);
void FUN_00375b70(PlayState* play, Actor* actor);
void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void EnGeldB_Stunned(EnGeldB* self, PlayState* play);
void EnGeldB_Damaged(EnGeldB* self, PlayState* play);
void EnGeldB_Defeated(Actor* thisx, PlayState* play);
void EnGeldB_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnGeldB* self = (EnGeldB*)thisx;
    Actor* key;

    // EnGeldB_CollisionCheck (inlined)
    if (*((u8*)self + 0xD05) & 0x80) {
        *((u8*)self + 0xD05) &= ~0x80;
        *((u8*)self + 0xC2D) &= ~2;
    } else if ((*((u8*)self + 0xC2D) & 2) && (self->action >= 5) && (self->spinAttackState < 2)) {
        *((u8*)self + 0xC2D) &= ~2;
        if (self->actor.colChkInfo.damageEffect != 6) {
            self->damageReaction = self->actor.colChkInfo.damageEffect;
            FUN_00375fd0(&self->actor, (u8*)self + 0xC34, 1);
            FUN_003ff758(&self->actor.world.pos, 0x01000436);
            if ((self->actor.colChkInfo.damageEffect == 1) || (self->actor.colChkInfo.damageEffect == 0xF)) {
                if (self->action != 0xF) {
                    Actor_SetColorFilter(&self->actor, 0, 0x78, 0, 0x50);
                    Actor_ApplyDamage(&self->actor);
                    if (self->actor.bgCheckFlags & 1) {
                        self->actor.speedXZ = 0.0f;
                    }
                    if ((self->damageReaction != 0xF) || (self->action == 0xC)) {
                        Animation_PlayOnceSetSpeed(&self->skelAnime, 5, 0.0f);
                    }
                    if (self->damageReaction == 0xF) {
                        self->iceTimer = 0x36;
                    }
                    Audio_PlayActorSound2(&self->actor, 0x0100030E);
                    self->actionFunc = EnGeldB_Stunned;
                    self->action = 0xF;
                }
            } else {
                Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 0x10);
                if (Actor_ApplyDamage(&self->actor) == 0) {
                    if (self->keyFlag != 0) {
                        key = (Actor*)Item_DropCollectible(play, &self->actor.world.pos, self->keyFlag | 0x11);
                        if (key != NULL) {
                            key->world.rot.y = Math_Atan2S(self->actor.home.pos.z - key->world.pos.z,
                                                           self->actor.home.pos.x - key->world.pos.x);
                            key->speedXZ = 6.0f;
                            FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
                        }
                    }
                    Animation_MorphToPlayOnce(&self->skelAnime, 4, -4.0f);
                    self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
                    if (self->actor.bgCheckFlags & 1) {
                        self->invisible = false;
                        self->actor.speedXZ = -6.0f;
                    } else {
                        self->invisible = true;
                    }
                    self->action = 1;
                    self->actor.flags &= ~1;
                    Audio_PlayActorSound2(&self->actor, 0x0100040A);
                    self->actionFunc = (EnGeldBActionFunc)EnGeldB_Defeated;
                    FUN_00375b70(play, &self->actor);
                } else {
                    Animation_MorphToPlayOnce(&self->skelAnime, 5, -4.0f);
                    if (self->actor.bgCheckFlags & 1) {
                        self->actor.speedXZ = -4.0f;
                        self->invisible = false;
                    } else {
                        self->invisible = true;
                    }
                    self->lookTimer = 0;
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                    Audio_PlayActorSound2(&self->actor, 0x01000409);
                    self->actionFunc = EnGeldB_Damaged;
                    self->action = 2;
                }
            }
        }
    }
    if (self->actor.colChkInfo.damageEffect != 6) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 15.0f, 30.0f, 60.0f, 0x1D);
        self->actionFunc(self, play);
        self->actor.focus.pos = self->actor.world.pos;
        self->actor.focus.pos.y += 40.0f;
        // EnGeldB_TurnHead (inlined)
        if ((self->action == 5) && (self->lookTimer != 0)) {
            self->headRot.y = Math_SinS(((s32)(self->lookTimer * 2.0f / 3.0f + 0.5f)) * 0x1068) * 8920.0f;
        } else if (self->action != 0xF) {
            if ((self->action != 7) && (self->action != 0xC)) {
                Math_SmoothStepToS(&self->headRot.y, self->actor.yawTowardsPlayer - self->actor.shape.rot.y, 1, 0x1F4, 0);
                self->headRot.y = CLAMP(self->headRot.y, -0x256F, 0x256F);
            } else {
                self->headRot.y = 0;
            }
        }
    }
    Collider_UpdateCylinder(&self->actor, &self->bodyCollider);
    CollisionCheck_SetOC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), (Collider*)&self->bodyCollider);
    if ((self->action >= 5) && (self->spinAttackState < 2) &&
        ((*(s16*)((u8*)self + 0x11A) == 0) || !(*(u32*)((u8*)self + 0x11C) & 0x400000))) {
        CollisionCheck_SetAC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), (Collider*)&self->bodyCollider);
    }
    if ((self->action == 6) && (self->skelAnime.curFrame == 0.0f)) {
        CollisionCheck_SetAC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), (Collider*)self->blockCollider);
    }
    if (self->swordState > 0) {
        CollisionCheck_SetAT(play, (CollisionCheckContext*)((u8*)play + 0x5C78), (Collider*)self->swordCollider);
    }
    if (self->blinkState == 0) {
        if ((Rand_ZeroOne() < 0.1f) && ((*(u32*)((u8*)play + 0x5BF4) % 4) == 0)) {
            self->blinkState++;
        }
    } else {
        self->blinkState = (self->blinkState + 1) & 3;
    }
}
#endif
GLOBAL_ASM("asm/EnGeldB_Update.s")

GLOBAL_ASM("asm/EnGeldB_Draw.s")

// 2026-10-14 sweep (compiled in z_en_geldb_dodge.cpp, where EnGeldB_Circle matched with play2): still 4 words.
// Tried all single-site + pair mutations (self alias, thisx, play2, statement swaps, block play2), range-check
// operand orders/locals, N64 SetupRollForward/SetupSpinAttack as static/static inline helpers.
// recipe_near4 2026-10-15: still 4 (dodge TU): dist/thisx locals, hi/lo f32 locals, nested if, generic alias mutations.
#if 0 // draft, 4 words off: register naming of the 150 < xzDist < 210 range-check literals
void EnGeldB_Advance(EnGeldB* self, PlayState* play) {
    s32 prevFrame;
    s32 beforeCurFrame;
    s32 afterPrevFrame;
    s16 facingAngletoLink;
    Player* player = GET_PLAYER(play);

    if (!EnGeldB_DodgeRanged(play, self)) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x2EE, 0);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (self->actor.xzDistToPlayer <= 40.0f) {
            Math_SmoothStepToF(&self->actor.speedXZ, -8.0f, 1.0f, 1.5f, 0.0f);
        } else if (self->actor.xzDistToPlayer > 55.0f) {
            Math_SmoothStepToF(&self->actor.speedXZ, 8.0f, 1.0f, 1.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 6.65f, 0.0f);
        }
        self->skelAnime.playSpeed = self->actor.speedXZ / 8.0f;
        facingAngletoLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
        facingAngletoLink = ABS(facingAngletoLink);
        if ((self->actor.xzDistToPlayer < 150.0f) && (*((u8*)player + 0x2227) != 0) && (facingAngletoLink >= 0x1F40)) {
            self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                EnGeldB_SetupCircle(self);
                return;
            }
        }

        prevFrame = (s32)self->skelAnime.curFrame;
        FUN_003731e0(&self->skelAnime);
        beforeCurFrame = (s32)(self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed));
        afterPrevFrame = (s32)ABS(self->skelAnime.playSpeed) + prevFrame;

        if (!Actor_IsFacingPlayer(&self->actor, 0x11C7)) {
            if (Rand_ZeroOne() > 0.5f) {
                EnGeldB_SetupCircle(self);
            } else {
                EnGeldB_SetupReady(self);
            }
        } else if (self->actor.xzDistToPlayer < 90.0f) {
            if (!FUN_00369608(play, &self->actor) &&
                (Rand_ZeroOne() > 0.03f || (self->actor.xzDistToPlayer <= 45.0f && facingAngletoLink < 0x38E0))) {
                Animation_PlayOnce(&self->skelAnime, 0);
                *(u8*)&self->swordCollider[0x10] &= ~4;
                self->action = 7;
                self->actor.speedXZ = 0.0f;
                self->spinAttackState = 0;
                FUN_003ff758(&self->actor.world.pos, 0x01000436);
                self->actionFunc = EnGeldB_Slash;
            } else if (FUN_00369608(play, &self->actor) && (Rand_ZeroOne() > 0.5f)) {
                EnGeldB_SetupRollBack(self);
            } else {
                EnGeldB_SetupCircle(self);
            }
        }
        if (!EnGeldB_ReactToPlayer(play, self, 0)) {
            if ((self->actor.xzDistToPlayer < 210.0f) && (self->actor.xzDistToPlayer > 150.0f) &&
                Actor_IsFacingPlayer(&self->actor, 0x71C)) {
                if (FUN_0035f228(play, &self->actor)) {
                    if (Rand_ZeroOne() > 0.5f) {
                        f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 2);

                        Animation_Change_00375c08(&self->skelAnime, 2, -1.0f, lastFrame, 0.0f, 2, -3.0f);
                        self->timer = 0;
                        self->invisible = true;
                        self->actor.speedXZ = 10.0f;
                        self->action = 14;
                        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
                        Audio_PlayActorSound2(&self->actor, 0x010002DC);
                        self->actionFunc = EnGeldB_RollForward;
                    } else {
                        f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 1);

                        Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, lastFrame, 3, 0.0f);
                        *(u8*)&self->swordCollider[0x10] &= ~6;
                        self->action = 12;
                        self->actor.speedXZ = 0.0f;
                        self->spinAttackState = 0;
                        self->actionFunc = EnGeldB_SpinAttack;
                    }
                } else {
                    EnGeldB_SetupCircle(self);
                    return;
                }
            }
            if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
                Audio_PlayActorSound2(&self->actor, 0x01000436);
            }
            if (prevFrame != (s32)self->skelAnime.curFrame) {
                if (((beforeCurFrame < 0) && (afterPrevFrame >= 1)) || ((beforeCurFrame < 4) && (afterPrevFrame >= 5))) {
                    Audio_PlayActorSound2(&self->actor, 0x01000410);
                }
            }
        }
    }
}
#endif
GLOBAL_ASM("asm/EnGeldB_Advance.s")

#if 0 // draft, 10 words off: only the player/frames-base callee-saved registers are swapped (ROM: base r5, player r6). 2026-10-14: same 10 compiled in z_en_geldb_dodge.cpp; greedy single-site mutations, frames accessor/pointer/index forms, player inline/late decl: no change
void EnGeldB_Block(EnGeldB* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 angleToLink;
    s16 angleFacingLink;

    if (self->timer != 0) {
        self->timer--;
    } else {
        self->skelAnime.playSpeed = 1.0f;
    }
    if (FUN_003731e0(&self->skelAnime)) {
        angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        if ((ABS(angleToLink) <= 0x4000) && (self->actor.xzDistToPlayer < 40.0f) &&
            (ABS(self->actor.yDistToPlayer) < 50.0f)) {
            if (FUN_0035b950(play, &self->actor, 100.0f, 0x2710, 0x4000, self->actor.shape.rot.y)) {
                if (*((u8*)player + 0x2226) == 0x11) {
                    EnGeldB_SetupSpinDodge(self, play);
                } else if (*(u32*)((u8*)play + 0x5BF4) & 1) {
                    EnGeldB_SetupBlock(self);
                } else {
                    EnGeldB_SetupRollBack(self);
                }
            } else {
                angleFacingLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
                if (!FUN_00369608(play, &self->actor) &&
                    ((*(u32*)((u8*)play + 0x5BF4) & 1) || (ABS(angleFacingLink) < 0x38E0))) {
                    Animation_PlayOnce(&self->skelAnime, 0);
                    *(u8*)&self->swordCollider[0x10] &= ~4;
                    self->action = 7;
                    self->actor.speedXZ = 0.0f;
                    self->spinAttackState = 0;
                    FUN_003ff758(&self->actor.world.pos, 0x01000436);
                    self->actionFunc = EnGeldB_Slash;
                } else {
                    EnGeldB_SetupCircle(self);
                }
            }
        } else {
            EnGeldB_SetupCircle(self);
        }
    } else if ((self->timer == 0) &&
               FUN_0035b950(play, &self->actor, 100.0f, 0x2710, 0x4000, self->actor.shape.rot.y)) {
        if (*((u8*)player + 0x2226) == 0x11) {
            EnGeldB_SetupSpinDodge(self, play);
        } else if (!EnGeldB_DodgeRanged(play, self)) {
            if (*(u32*)((u8*)play + 0x5BF4) & 1) {
                if ((self->actor.xzDistToPlayer < 100.0f) && (Rand_ZeroOne() > 0.7f)) {
                    EnGeldB_SetupJump(self);
                } else {
                    EnGeldB_SetupRollBack(self);
                }
            } else {
                EnGeldB_SetupBlock(self);
            }
        }
    }
}
#endif
GLOBAL_ASM("asm/EnGeldB_Block.s")



// (Actor*, PlayState*) signature with `thisx->` for the Actor fields keeps play in r6 and the hoisted 0 in r7 (ROM order).
void EnGeldB_Defeated(Actor* thisx, PlayState* play) {
    EnGeldB* self = (EnGeldB*)thisx;

    if (thisx->bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        thisx->speedXZ = 0.0f;
    }
    if (thisx->bgCheckFlags & BGCHECKFLAG_GROUND) {
        Math_SmoothStepToF(&thisx->speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
        self->invisible = false;
    }
    if (FUN_003731e0(&self->skelAnime)) {
        f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 8);

        Animation_Change_00375c08(&self->skelAnime, 8, -2.0f, lastFrame, 0.0f, 3, -4.0f);
        thisx->shape.rot.y = thisx->world.rot.y = thisx->yawTowardsPlayer;
        self->timer = 30;
        self->invisible = false;
        self->action = 0;
        self->actionFunc = EnGeldB_Flee;
    } else {
        if (self->skelAnime.curFrame < 9.9f) {
            return;
        }
        if (self->skelAnime.curFrame > 10.1f) {
            return;
        }
        Audio_PlayActorSound2(thisx, 0x010002EA);
        FUN_00373d0c(play);
    }
}



void EnGeldB_Flee(EnGeldB* self, PlayState* play) {
    if (self->skelAnime.curFrame > 10.2f) {
        if (self->skelAnime.curFrame < 10.4f) {
            Audio_PlayActorSound2(&self->actor, 0x010002DC);
        }
    }
    if (self->skelAnime.curFrame <= 2.0f) {
        self->actor.gravity = 0.0f;
        FUN_0036f00c(play, &self->actor, &self->leftFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
        FUN_0036f00c(play, &self->actor, &self->rightFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
    }
    if (FUN_003731e0(&self->skelAnime)) {
        Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.floorHeight + 300.0f, 1.0f, 20.5f, 0.0f);
        self->timer--;
        if (self->timer == 0) {
            Actor_Kill(&self->actor);
        }
    }
}

void EnGeldB_Jump(EnGeldB* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (self->actor.velocity.y >= 5.0f) {
        FUN_0034c128(play, &self->leftFootPos);
        FUN_0034c128(play, &self->rightFootPos);
    }
    if (FUN_003731e0(&self->skelAnime) &&
        (self->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH))) {
        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->actor.world.pos.y = self->actor.floorHeight;
        self->actor.velocity.y = self->actor.speedXZ = 0.0f;
        self->actor.shape.rot.x = 0;
        if (FUN_00369608(play, &self->actor)) {
            EnGeldB_SetupReady(self);
        } else {
            Animation_PlayOnce(&self->skelAnime, 0);
            *(u8*)&self->swordCollider[0x10] &= ~4;
            self->action = 7;
            self->actor.speedXZ = 0.0f;
            self->spinAttackState = 0;
            FUN_003ff758(&self->actor.world.pos, 0x01000436);
            self->actionFunc = EnGeldB_Slash;
        }
    }
}

s32 EnGeldB_OverrideLimbDraw(PlayState* play, s32 limbIndex, void** dList, EnGeldB* self) {
    if (limbIndex == 9) {
        FUN_0034df48(dList, &self->headRot);
    }
    return 0;
}

void EnGeldB_Pivot(EnGeldB* self, PlayState* play) {
    s16 angleToLink;
    s16 turnRate;
    f32 playSpeed;

    if (!EnGeldB_DodgeRanged(play, self) && !EnGeldB_ReactToPlayer(play, self, 0)) {
        angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        if (angleToLink > 0) {
            turnRate = (angleToLink * 0.25f) + 2000.0f;
        } else {
            turnRate = (angleToLink * 0.25f) - 2000.0f;
        }
        self->actor.world.rot.y = self->actor.shape.rot.y += turnRate;
        if (angleToLink > 0) {
            playSpeed = turnRate * 0.5f;
            playSpeed = CLAMP_MAX(playSpeed, 1.0f);
        } else {
            playSpeed = turnRate * 0.5f;
            playSpeed = CLAMP_MIN(playSpeed, -1.0f);
        }
        self->skelAnime.playSpeed = -playSpeed;
        FUN_003731e0(&self->skelAnime);
        if (Actor_IsFacingPlayer(&self->actor, 30 * 0x10000 / 360)) {
            if (Rand_ZeroOne() > 0.8f) {
                EnGeldB_SetupCircle(self);
            } else {
                f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 6);

                Animation_Change_00375c08(&self->skelAnime, 6, 1.0f, 0.0f, lastFrame, 1, -4.0f);
                self->action = 8;
                self->actionFunc = EnGeldB_Advance;
            }
        }
        if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
            Audio_PlayActorSound2(&self->actor, 0x01000436);
        }
    }
}

GLOBAL_ASM("asm/EnGeldB_PostLimbDraw.s")

#if 0 // draft, 29 words off: player / gameplayFrames-base callee-saved registers (r6/r7) and arg2 / angleToLink (sl/fp) swapped
Actor* Actor_FindNearby(PlayState* play, Actor* actor, s16 actorId, s32 category, f32 range);
f32 Actor_WorldDistXYZToActor(Actor* a, Actor* b);
s32 EnGeldB_ReactToPlayer(PlayState* play, EnGeldB* self, s16 arg2) {
    Player* player = GET_PLAYER(play);
    Actor* thisx = &self->actor;
    s16 angleToWall;
    s16 angleToLink;

    angleToWall = thisx->wallYaw - thisx->shape.rot.y;
    angleToWall = ABS(angleToWall);
    angleToLink = thisx->yawTowardsPlayer - thisx->shape.rot.y;
    angleToLink = ABS(angleToLink);

    if (FUN_0035b950(play, thisx, 100.0f, 0x2710, 0x3E80, thisx->shape.rot.y)) {
        if (*((u8*)player + 0x2226) == 0x11) {
            EnGeldB_SetupSpinDodge(self, play);
            return true;
        } else if (*(u32*)((u8*)play + 0x5BF4) % 3 == 0) {
            EnGeldB_SetupBlock(self);
            return true;
        }
    }
    if (FUN_0035b950(play, thisx, 100.0f, 0x5DC0, 0x2AA8, thisx->shape.rot.y)) {
        thisx->shape.rot.y = thisx->world.rot.y = thisx->yawTowardsPlayer;
        if ((thisx->bgCheckFlags & BGCHECKFLAG_WALL) && (ABS(angleToWall) < 0x2EE0) &&
            (thisx->xzDistToPlayer < 90.0f)) {
            EnGeldB_SetupJump(self);
            return true;
        } else if (*((u8*)player + 0x2226) == 0x11) {
            EnGeldB_SetupSpinDodge(self, play);
            return true;
        } else if ((thisx->xzDistToPlayer < 90.0f) && (*(u32*)((u8*)play + 0x5BF4) & 1)) {
            EnGeldB_SetupBlock(self);
            return true;
        } else {
            EnGeldB_SetupRollBack(self);
            return true;
        }
    } else {
        Actor* bomb = Actor_FindNearby(play, thisx, -1, 3, 80.0f);

        if (bomb != NULL) {
            thisx->shape.rot.y = thisx->world.rot.y = thisx->yawTowardsPlayer;
            if (((thisx->bgCheckFlags & BGCHECKFLAG_WALL) && (angleToWall < 0x2EE0)) || (bomb->id == 0xDA)) {
                if ((bomb->id == 0xDA) && (Actor_WorldDistXYZToActor(thisx, bomb) < 80.0f) &&
                    ((s16)(thisx->shape.rot.y - (bomb->world.rot.y - 0x8000)) < 0x3E80)) {
                    EnGeldB_SetupJump(self);
                    return true;
                } else {
                    EnGeldB_SetupSideStep(self, play);
                    return true;
                }
            } else {
                EnGeldB_SetupRollBack(self);
                return true;
            }
        }
    }

    if (arg2) {
        if (angleToLink >= 0x1B58) {
            EnGeldB_SetupSideStep(self, play);
            return true;
        } else {
            s16 angleToFacingLink = player->actor.shape.rot.y - thisx->shape.rot.y;

            if ((thisx->xzDistToPlayer <= 45.0f) && !FUN_00369608(play, thisx) &&
                ((*(u32*)((u8*)play + 0x5BF4) & 7) || (ABS(angleToFacingLink) < 0x38E0))) {
                Animation_PlayOnce(&self->skelAnime, 0);
                *(u8*)&self->swordCollider[0x10] &= ~4;
                self->action = 7;
                thisx->speedXZ = 0.0f;
                self->spinAttackState = 0;
                FUN_003ff758(&thisx->world.pos, 0x01000436);
                self->actionFunc = EnGeldB_Slash;
                return true;
            } else {
                EnGeldB_SetupCircle(self);
                return true;
            }
        }
    }
    return false;
}
#endif
GLOBAL_ASM("asm/EnGeldB_ReactToPlayer.s")

void EnGeldB_Ready(EnGeldB* self, PlayState* play) {
    Actor* thisx = &self->actor;  // reading shape.rot.y through thisx fixes the r0/r1 order of the angle computation
    Player* player = GET_PLAYER(play);
    s16 angleToLink;

    FUN_003731e0(&self->skelAnime);
    if (self->lookTimer != 0) {
        angleToLink = self->actor.yawTowardsPlayer - (thisx->shape.rot.y + self->headRot.y);
        if (ABS(angleToLink) > 0x2000) {
            self->lookTimer--;
            return;
        }
        self->lookTimer = 0;
    }
    angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if (!EnGeldB_DodgeRanged(play, self)) {
        if (self->unkTimer != 0) {
            self->unkTimer--;

            if (ABS(angleToLink) >= 0x1FFE) {
                return;
            }
            self->unkTimer = 0;
        } else if (EnGeldB_ReactToPlayer(play, self, 0)) {
            return;
        }
        angleToLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
        if ((self->actor.xzDistToPlayer < 100.0f) && (*((u8*)player + 0x2227) != 0) && (ABS(angleToLink) >= 0x1F40)) {
            self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            EnGeldB_SetupCircle(self);
        } else if (--self->timer == 0) {
            if (Actor_IsFacingPlayer(&self->actor, 0x1555)) {
                if ((210.0f > self->actor.xzDistToPlayer) && (self->actor.xzDistToPlayer > 150.0f) &&
                    (Rand_ZeroOne() < 0.3f)) {
                    if (FUN_00369608(play, &self->actor) || (Rand_ZeroOne() > 0.5f) || (ABS(angleToLink) < 0x38E0)) {
                        f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 2);

                        Animation_Change_00375c08(&self->skelAnime, 2, -1.0f, lastFrame, 0.0f, 2, -3.0f);
                        self->timer = 0;
                        self->invisible = true;
                        self->actor.speedXZ = 10.0f;
                        self->action = 14;
                        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
                        Audio_PlayActorSound2(&self->actor, 0x010002DC);
                        self->actionFunc = EnGeldB_RollForward;
                    } else {
                        f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 1);

                        Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, lastFrame, 3, 0.0f);
                        *(u8*)&self->swordCollider[0x10] &= ~6;
                        self->action = 12;
                        self->actor.speedXZ = 0.0f;
                        self->spinAttackState = 0;
                        self->actionFunc = EnGeldB_SpinAttack;
                    }
                } else if (Rand_ZeroOne() > 0.3f) {
                    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 6);

                    Animation_Change_00375c08(&self->skelAnime, 6, 1.0f, 0.0f, lastFrame, 1, -4.0f);
                    self->action = 8;
                    self->actionFunc = EnGeldB_Advance;
                } else {
                    EnGeldB_SetupCircle(self);
                }
            } else {
                Animation_MorphToLoop(&self->skelAnime, 7, -4.0f);
                self->action = 9;
                self->actionFunc = EnGeldB_Pivot;
            }
            if ((*(u32*)((u8*)play + 0x5BF4) & 0x5F) == 0) {
                Audio_PlayActorSound2(&self->actor, 0x01000436);
            }
        }
    }
}

void EnGeldB_RollBack(EnGeldB* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        if (!FUN_00369608(play, &self->actor) && (self->actor.xzDistToPlayer < 170.0f) &&
            (self->actor.xzDistToPlayer > 140.0f) && (Rand_ZeroOne() < 0.2f)) {
            f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 1);

            Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f, lastFrame, 3, 0.0f);
            *(u8*)&self->swordCollider[0x10] &= ~6;
            self->action = 12;
            self->actor.speedXZ = 0.0f;
            self->spinAttackState = 0;
            self->actionFunc = EnGeldB_SpinAttack;
        } else if (*(u32*)((u8*)play + 0x5BF4) & 1) {
            EnGeldB_SetupSideStep(self, play);
        } else {
            EnGeldB_SetupReady(self);
        }
    }
    if ((play->state.frames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000436);
    }
}



void EnGeldB_SetupAdvance(EnGeldB* self, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 6);

    Animation_Change_00375c08(&self->skelAnime, 6, 1.0f, 0.0f, lastFrame, 1, -4.0f);
    self->action = 8;
    self->actionFunc = EnGeldB_Advance;
}

void EnGeldB_SetupBlock(EnGeldB* self) {
    // The unused-looking self copy + Actor alias are needed: they keep the 0.0f literal in s19 and lastFrame in s20 (ROM order).
    EnGeldB* s2 = self;
    Actor* thisx = &s2->actor;
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 3);

    if (self->swordState != 0) {
        self->swordState = -1;
    }
    thisx->speedXZ = 0.0f;
    self->action = 6;
    self->timer = ((s32)Rand_CenteredFloat(10.0f) + 10) > 0
                      ? (s32)(0.5f + (((s32)Rand_CenteredFloat(10.0f) + 10) * 3.0f) * 0.5f)
                      : (s32)((((s32)Rand_CenteredFloat(10.0f) + 10) * 3.0f) * 0.5f - 0.5f);
    Animation_Change_00375c08(&self->skelAnime, 3, 0.0f, 0.0f, lastFrame, 2, 0.0f);
    self->actionFunc = EnGeldB_Block;
}


void EnGeldB_SetupJump(EnGeldB* self) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 2);

    Animation_Change_00375c08(&self->skelAnime, 2, -1.0f, lastFrame, 0.0f, 2, -3.0f);
    self->timer = 0;
    self->actor.speedXZ = 6.5f;
    self->actor.velocity.y = 15.0f;
    self->invisible = false;
    self->action = 3;
    Audio_PlayActorSound2(&self->actor, 0x010002DC);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actionFunc = EnGeldB_Jump;
}

void EnGeldB_SetupReady(EnGeldB* self) {
    Animation_MorphToLoop(&self->skelAnime, 10, -4.0f);
    self->action = 5;
    // Rand_S16Offset(5, 10) scaled for the 3DS timer rate (R_UPDATE_RATE folded to 2), the argument is evaluated twice
    self->timer = ((s32)(5.0f + Rand_ZeroOne() * 10.0f)) > 0
                      ? (s32)(0.5f + ((s32)(5.0f + Rand_ZeroOne() * 10.0f) * 3.0f) * 0.5f)
                      : (s32)(((s32)(5.0f + Rand_ZeroOne() * 10.0f) * 3.0f) * 0.5f - 0.5f);
    self->actor.speedXZ = 0.0f;
    s16 yaw = self->actor.shape.rot.y;
    self->actor.world.rot.y = yaw;
    self->actionFunc = EnGeldB_Ready;
}

void EnGeldB_SetupRollBack(EnGeldB* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, 2, -3.0f);
    self->timer = 0;
    self->invisible = true;
    self->actor.speedXZ = -8.0f;
    self->action = 4;
    Audio_PlayActorSound2(&self->actor, 0x010002DC);
    self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    self->actionFunc = EnGeldB_RollBack;
}



void EnGeldB_SetupSpinDodge(EnGeldB* self, PlayState* play) {
    s16 sp3E;
    Player* player = GET_PLAYER(play);
    Actor* thisx = &self->actor;
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 7);

    Animation_Change_00375c08(&self->skelAnime, 7, 1.0f, 0.0f, lastFrame, 1, 0.0f);
    sp3E = player->actor.shape.rot.y;
    if (Math_SinS(sp3E - self->actor.shape.rot.y) > 0.0f) {
        self->actor.speedXZ = -10.0f;
    } else if (Math_SinS(sp3E - self->actor.shape.rot.y) < 0.0f) {
        self->actor.speedXZ = 10.0f;
    } else if (Rand_ZeroOne() > 0.5f) {
        self->actor.speedXZ = 10.0f;
    } else {
        self->actor.speedXZ = -10.0f;
    }
    self->skelAnime.playSpeed = thisx->speedXZ * -0.5f;
    s16 y = self->actor.shape.rot.y;
    self->actor.world.rot.y = y;
    self->timer = 9;
    self->approachRate = 0.0f;
    self->unkFloat = 0.0f;
    self->action = 16;
    EnGeldB_SetupAction(self, EnGeldB_SpinDodge);
}


#if 0 // draft, ~10 words off: the first timer block gets an extra `vmov s19,s18` copy in the condition (ROM has it only in the 2nd block). 2026-10-14: 10 in dodge-file TU; mutations/operand-order swaps fix the timer vmla regs only together with a play r7/r8 shift (best 12)
void EnGeldB_Slash(EnGeldB* self, PlayState* play) {
    EnGeldB* s2 = self;
    Player* player = GET_PLAYER(play);
    s16 angleFacingLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
    s16 angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    angleFacingLink = ABS(angleFacingLink);
    angleToLink = ABS(angleToLink);

    self->actor.speedXZ = 0.0f;
    if (self->skelAnime.curFrame > 0.6f) {
        if (self->skelAnime.curFrame < 0.8f) {
            Audio_PlayActorSound2(&self->actor, 0x01000408);
            self->swordState = 1;
            goto next;
        }
    }
    if (self->skelAnime.curFrame > 5.9f) {
        if (self->skelAnime.curFrame < 6.1f) {
            self->swordState = -1;
        }
    }
next:
    if (*(u8*)&self->swordCollider[0x10] & 4) {
        self->swordState = -1;
        *(u8*)&self->swordCollider[0x10] &= ~6;
        EnGeldB_SetupRollBack(self);
    } else if (FUN_003731e0(&self->skelAnime)) {
        if (!Actor_IsFacingPlayer(&self->actor, 0x1554)) {
            EnGeldB_SetupReady(self);
            { s32 t; if ((s32)(5.0f + Rand_ZeroOne() * 5.0f) > 0) { t = (s32)(0.5f + ((s32)(5.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f); } else { t = (s32)(((s32)(5.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f - 0.5f); } s2->timer = t; }
            if (angleToLink > 0x4000) {
                self->lookTimer = 30;
            }
        } else if (Rand_ZeroOne() > 0.7f || (self->actor.xzDistToPlayer >= 120.0f)) {
            EnGeldB_SetupReady(self);
            { s32 r = (s32)(5.0f + Rand_ZeroOne() * 5.0f); s32 t; if (r > 0) { t = (s32)(0.5f + ((s32)(5.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f); } else { t = (s32)(((s32)(5.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f - 0.5f); } self->timer = t; }
        } else {
            self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                EnGeldB_SetupSideStep(self, play);
            } else if (angleFacingLink <= 0x2710) {
                if (angleToLink > 0x3E80) {
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                    EnGeldB_SetupCircle(self);
                } else {
                    EnGeldB_ReactToPlayer(play, self, 1);
                }
            } else {
                EnGeldB_SetupCircle(self);
            }
        }
    }
}
#endif
GLOBAL_ASM("asm/EnGeldB_Slash.s")

#if 0 // draft, ~8 words off: swordState/speedXZ store order in the frame-13 block, 6.0f hoisting and literal pool order. 2026-10-14: 8 in dodge-file TU; greedy mutations, zero/six decl-vs-assign orders, store swap: no gain
#define TIMER55 (((s32)(5.0f + Rand_ZeroOne() * 5.0f)) > 0 ? (s32)(0.5f + ((s32)(5.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f) : (s32)(((s32)(5.0f + Rand_ZeroOne() * 5.0f) * 3.0f) * 0.5f - 0.5f))
void EnGeldB_SpinAttack(EnGeldB* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 six = 6.0f;
    s16 angleFacingLink;
    s16 angleToLink;

    if (self->spinAttackState < 2) {
        if (*(u8*)&self->swordCollider[0x10] & 4) {
            *(u8*)&self->swordCollider[0x10] &= ~6;
            self->spinAttackState = 1;
            self->skelAnime.playSpeed = 1.5f;
        } else if (*(u8*)&self->swordCollider[0x10] & 2) {
            *(u8*)&self->swordCollider[0x10] &= ~2;
            if (&player->actor == *(Actor**)&self->swordCollider[0x04]) {
                FUN_00374bb8(play, &self->actor, six, self->actor.yawTowardsPlayer, six);
                self->spinAttackState = 2;
                FUN_0036e980(play, &self->actor, 0x18);
                FUN_00367c7c(play, 0x6003, &self->actor);
                self->timer = 45;
                self->actor.speedXZ = 0.0f;
                Audio_PlayActorSound2(&self->actor, 0x01000429);
                return;
            }
        }
    }
    if ((s32)self->skelAnime.curFrame <= 8) {
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    } else if (Animation_OnFrameImpl(&self->skelAnime, 18.6f, 1.0f)) {
        FUN_0036f00c(play, &self->actor, &self->leftFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
        FUN_0036f00c(play, &self->actor, &self->rightFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
        self->swordState = 1;
        self->actor.speedXZ = 10.0f;
        Audio_PlayActorSound2(&self->actor, 0x01000408);
    } else if (Animation_OnFrameImpl(&self->skelAnime, 26.0f, 1.0f)) {
        self->actor.speedXZ = 0.0f;
    } else if (Animation_OnFrameImpl(&self->skelAnime, 30.0f, 1.0f)) {
        self->swordState = -1;
    }
    if (FUN_003731e0(&self->skelAnime) && (self->spinAttackState < 2)) {
        if (!Actor_IsFacingPlayer(&self->actor, 0x1554)) {
            EnGeldB_SetupReady(self);
            self->timer = TIMER55;
            self->lookTimer = 69;
        } else if (self->spinAttackState != 0) {
            EnGeldB_SetupRollBack(self);
        } else if (Rand_ZeroOne() > 0.7f || (self->actor.xzDistToPlayer >= 120.0f)) {
            EnGeldB_SetupReady(self);
            self->timer = TIMER55;
        } else {
            self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                EnGeldB_SetupSideStep(self, play);
            } else {
                angleFacingLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
                angleFacingLink = ABS(angleFacingLink);
                if (angleFacingLink <= 0x2710) {
                    angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
                    angleToLink = ABS(angleToLink);
                    if (angleToLink > 0x3E80) {
                        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                        EnGeldB_SetupCircle(self);
                    } else {
                        EnGeldB_ReactToPlayer(play, self, 1);
                    }
                } else {
                    EnGeldB_SetupCircle(self);
                }
            }
        }
    }
}
#endif
GLOBAL_ASM("asm/EnGeldB_SpinAttack.s")

void EnGeldB_Stunned(EnGeldB* self, PlayState* play) {
    if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f;
        }
        self->invisible = false;
    }
    if ((*(s16*)((u8*)self + 0x11A) == 0) && (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        if (self->actor.colChkInfo.health == 0) {
            Animation_MorphToPlayOnce(&self->skelAnime, 4, -4.0f);
            self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
            if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
                self->actor.speedXZ = -6.0f;
                self->invisible = false;
            } else {
                self->invisible = true;
            }
            self->action = 1;
            self->actor.flags &= ~1;
            Audio_PlayActorSound2(&self->actor, 0x0100040A);
            self->actionFunc = (EnGeldBActionFunc)EnGeldB_Defeated;
        } else {
            EnGeldB_ReactToPlayer(play, self, 1);
        }
    }
}

void EnGeldB_Wait(EnGeldB* self, PlayState* play) {
    if ((self->invisible && !Flags_GetSwitch(play, self->actor.home.rot.z)) ||
        (self->actor.xzDistToPlayer > 300.0f && self->unk_C17 == 0)) {
        self->actor.world.pos.y = self->actor.floorHeight + 120.0f;
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    } else {
        self->unk_C17 = 1;
        self->invisible = false;
        self->actor.shape.shadowScale = 90.0f;
        FUN_0034f724(play);
    }
    if (self->actor.bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) {
        Audio_PlayActorSound2(&self->actor, 0x010002EA);
        self->actor.flags |= 1;
        self->skelAnime.playSpeed = 1.0f;
        self->actor.world.pos.y = self->actor.floorHeight;
        self->actor.focus.pos = self->actor.world.pos;
        self->actor.velocity.y = 0.0f;
        self->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND_TOUCH;
        FUN_0036f00c(play, &self->actor, &self->leftFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
        FUN_0036f00c(play, &self->actor, &self->rightFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
    }
    if (FUN_003731e0(&self->skelAnime)) {
        EnGeldB_SetupReady(self);
    }
}

}
