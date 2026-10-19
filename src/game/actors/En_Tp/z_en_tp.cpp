extern "C" {
#include "z_en_tp.hpp"

#define FLAGS 0x00000000

void EnTp_Init(Actor* thisx, PlayState* play);
void EnTp_Destroy(Actor* actor, GameState* state);
void EnTp_Update(Actor* thisx, PlayState* play);
void EnTp_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Tp.data.s")

GLOBAL_ASM("../src/game/actors/En_Tp/data_syms.s")

ActorInit En_Tp_InitVars = {
    ACTOR_EN_TP,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_TP,
    sizeof(EnTp),
    (ActorFunc)EnTp_Init,
    (ActorFunc)EnTp_Destroy,
    (ActorFunc)EnTp_Update,
    (ActorFunc)EnTp_Draw,
};

#if 0 // EnTp_Init: far (211/246 words off): ROM keeps constant 0 in sb for an un-folded `0 * i` spawn-params induction variable (r8 += sb), 1 in sl; armcc folds 0*i in every variant tried (~25: s16 cast, local/const/enum/extern multiplier, dead if, loop var types). Needs EQUs EnTp_sInitChain/JntSphInit/DamageTable (data_syms.s).
extern InitChainEntry EnTp_sInitChain[];
extern u8 EnTp_sJntSphInit[];
extern DamageTable EnTp_sDamageTable;
void Collider_InitJntSph(PlayState* play, EnTpCollider* collider);
void Collider_SetJntSph(PlayState* play, EnTpCollider* collider, Actor* actor, void* init, void* elements);
void FUN_0026e1e0(EnTp* self, PlayState* play);
void EnTp_Tail_FollowHead(Actor* thisx, PlayState* play);
void EnTp_Fragment_Fade(EnTp* self, PlayState* play);

static inline void EnTp_Head_SetupWaitPlayerAndRise(EnTp* self) {
    self->actor.speedXZ = 0.0f;
    self->action = 4;
    self->modeVar = 0;
    self->actor.shape.rot.x = 0xC000;
    self->sub.timer = 0x5A;
    self->sub.timer2 = 0;
    self->actionFunc = FUN_0026e1e0;
}

void EnTp_Init(Actor* thisx, PlayState* play2) {
    EnTp* newChild;
    EnTp* prevChild;
    s32 i;
    s32 timer;
    EnTp* self = (EnTp*)thisx;
    PlayState* play = play2;

    Actor_ProcessInitChain(&self->actor, EnTp_sInitChain);
    self->actor.targetMode = 3;
    self->actor.colChkInfo.damageTable = &EnTp_sDamageTable;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.14f);
    if (self->actor.params < 0 || self->actor.params == 0xC) {
        FUN_00372f38(&self->actor, play, &self->model, 0, 0);
        FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 6);
    } else {
        ZARInfo* zar = FUN_00372f38(&self->actor, play, &self->model, 1, 0);
        self->anim.unk_00 = self->model->unk_10;
        self->anim.FUN_00372d94(zar->GetCMABByIndex(0));
        self->anim.SetAnimType(1);
        self->actor.flags &= ~1;
    }
    self->modeVar = 0;
    self->actor.colChkInfo.health = 1;
    prevChild = self;
    self->sub.fadeAlpha = 255;
    self->scaleTimer = 0;
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, &self->actor, EnTp_sJntSphInit, (u8*)&self->collider + 0x20);
    if (self->actor.params < 0) {
        self->actor.naviEnemyId = 6;
        self->sub.timer = 0;
        self->collider.acFlags |= 4;
        self->collider.elements->worldRadius = 8.0f;
        self->collider.elements->modelRadius = self->collider.elements->worldRadius;
        EnTp_Head_SetupWaitPlayerAndRise(self);
        self->actor.focus.pos = self->actor.world.pos;
        self->actor.flags |= 0x15;
        Actor_SetScale(&self->actor, 1.5f);
        for (i = 0; i < 7; i++) {
            newChild = (EnTp*)Actor_Spawn(&play->actorCtx, play, 0x35, self->actor.world.pos.x,
                                          self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0 * i, 1);
            if (newChild != NULL) {
                prevChild->actor.child = &newChild->actor;
                newChild->actor.parent = &prevChild->actor;
                newChild->sub.sparkTimer = i + 1;
                newChild->headInstance = self;
                Actor_SetScale(&newChild->actor, 0.3f);
                if (i == 2) {
                    newChild->actor.flags |= 0x15;
                    newChild->modeVar = 1;
                }
                {
                    s32 n = i * -5;
                    if (n > 0) {
                        timer = (s32)(0.5f + (n * 3.0f) * 0.5f);
                    } else {
                        timer = (s32)((n * 3.0f) * 0.5f - 0.5f);
                    }
                }
                newChild->sub.timer2 = timer;
                newChild->sub.timer = timer;
                prevChild = newChild;
                newChild->sub.bodyElemDist = 6.0f - ((f32)i * 0.75f);
            }
        }
    } else if (self->actor.params == 0) {
        self->action = 2;
        self->actionFunc = (EnTpActionFunc)EnTp_Tail_FollowHead;
    } else {
        self->action = 0;
        self->actor.world.pos.x += (Rand_ZeroOne() - 0.5f) * 5.0f;
        self->actor.world.pos.y += (Rand_ZeroOne() - 0.5f) * 5.0f;
        self->actor.world.pos.z += (Rand_ZeroOne() - 0.5f) * 5.0f;
        self->actor.velocity.x = (Rand_ZeroOne() - 0.5f) * 1.5f;
        self->actor.velocity.y = (Rand_ZeroOne() - 0.5f) * 1.5f;
        self->actor.velocity.z = (Rand_ZeroOne() - 0.5f) * 1.5f;
        self->actor.flags &= ~1;
        self->actionFunc = EnTp_Fragment_Fade;
    }
}
#endif
GLOBAL_ASM("asm/EnTp_Init.s")

void Collider_DestroyJntSph(PlayState* play, EnTpCollider* collider);
void SkelAnime_Free2(SkelAnime* skelAnime);
void FUN_003685a0(void* arg);
void FUN_0026e1e0(EnTp* self, PlayState* play);

void EnTp_Destroy(Actor* thisx, GameState* state) {
    EnTp* self = (EnTp*)thisx;

    Collider_DestroyJntSph((PlayState*)state, &self->collider);
    FUN_003508b8(thisx, self->model, 0);
    FUN_003685a0(&self->anim);
    SkelAnime_Free2(&self->skelAnime);
}

#if 0 // EnTp_Update: far (315/355 words off): draft with CheckCollide inlined; ROM uses separate ifs, scaleTimer=0x26 + timer=3 on head death, rodata color copy. GLOBAL_ASM below.
extern Color_RGBA8 EnTp_sSparkPrimColor;
void FUN_003742c4(Actor* actor, EnTpCollider* collider, s32 flag);
void Actor_ApplyDamage(Actor* actor);
void Audio_PlayActorSound2(Actor* actor, s32 sfxId);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
void EffectSsKiraKira_SpawnSmall(PlayState* play, Vector3f* pos, Vector3f* velocity, Vector3f* accel,
                                 Color_RGBA8* primColor, Color_RGBA8* envColor);
void EnTp_Die(EnTp* self, PlayState* play);

void EnTp_Update(Actor* thisx, PlayState* play) {
    Player* player;
    Vector3f sparkVel(0.0f, 0.0f, 0.0f);
    Vector3f sparkAccel(0.0f, -0.6f, 0.0f);
    Vector3f sparkPos(0.0f, 0.0f, 0.0f);
    Color_RGBA8 sparkPrimColor = EnTp_sSparkPrimColor;
    Color_RGBA8 sparkEnvColor = { 0, 0, 0, 0 };
    EnTp* self = (EnTp*)thisx;
    s16 v;

    player = GET_PLAYER(play);
    if (player->stateFlags1 & 0x4000000) {
        self->sub.dmgReaction = 0;
    }
    if (self->actor.colChkInfo.health != 0) {
        if ((self->collider.acFlags & 2) && (self->action >= 2)) {
            s32 isHead = 0;
            s32 applyStun = 0;
            if (self->actor.params < 0) {
                isHead = 1;
            }
            self->collider.acFlags &= ~2;
            FUN_003742c4(&self->actor, &self->collider, 1);
            self->sub.dmgReaction = self->actor.colChkInfo.damageEffect;
            if (self->actor.colChkInfo.damageEffect != 0) {
                if (self->actor.colChkInfo.damageEffect == 1) {
                    applyStun = 1;
                }
                if (!isHead) {
                    Actor_ApplyDamage(&self->actor);
                }
                if (self->actor.colChkInfo.health == 0) {
                    EnTp* headInstance;
                    self->actor.flags &= ~1;
                    headInstance = self->headInstance;
                    if (headInstance->actor.params < 0) {
                        Actor* c;
                        headInstance->action = 1;
                        c = headInstance->actor.child;
                        if (c != NULL) {
                            do {
                                c->params = 0xB;
                                c->colChkInfo.health = 0;
                                c = c->child;
                            } while (c != NULL);
                        }
                        headInstance->sub.timer = 20;
                        Audio_PlayActorSound2(&headInstance->actor, 0x010002D6);
                        headInstance->actionFunc = (EnTpActionFunc)EnTp_Die;
                        headInstance->action = 1;
                        headInstance->sub.dmgReaction = self->actor.colChkInfo.damageEffect;
                        headInstance->actor.params = 0xC;
                    }
                } else {
                    EnTp* parent;
                    EnTp* child;
                    if (applyStun) {
                        self->sub.dmgReaction = 0;
                    }
                    if (applyStun) {
                        self->actor.freezeTimer = 80;
                        Audio_PlayActorSound2(&self->actor, 0x0100030E);
                        if (isHead) {
                            Actor_SetColorFilter(&self->actor, 0, 255, 0, 80);
                        } else {
                            Actor_SetColorFilter(&self->actor, 0, 255, 0x200000, 80);
                        }
                    }
                    parent = (EnTp*)self->actor.parent;
                    while (parent != NULL) {
                        parent->collider.acFlags &= ~2;
                        if (applyStun) {
                            parent->actor.freezeTimer = 80;
                            Audio_PlayActorSound2(&self->actor, 0x0100030E);
                            if (isHead) {
                                Actor_SetColorFilter(&parent->actor, 0, 255, 0, 80);
                            } else {
                                Actor_SetColorFilter(&parent->actor, 0, 255, 0x200000, 80);
                            }
                        }
                        parent = (EnTp*)parent->actor.parent;
                    }
                    child = (EnTp*)self->actor.child;
                    while (child != NULL) {
                        child->collider.acFlags &= ~2;
                        if (applyStun) {
                            child->actor.freezeTimer = 80;
                            if (isHead) {
                                Actor_SetColorFilter(&child->actor, 0, 255, 0, 80);
                            } else {
                                Actor_SetColorFilter(&child->actor, 0, 255, 0x200000, 80);
                            }
                        }
                        child = (EnTp*)child->actor.child;
                    }
                }
            }
        }
    }
    self->actionFunc(self, play);
    if (self->actor.params < 0) {
        Actor_MoveForward(&self->actor);
        if (self->action != 9) {
            Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 15.0f, 10.0f, 5);
        }
        if ((self->actor.speedXZ != 0.0f) && (self->actor.bgCheckFlags & 8)) {
            v = self->actor.wallYaw - self->actor.world.rot.y;
            if (ABS(v) > 0x4000) {
                if (v >= 0) {
                    self->actor.world.rot.y -= 0x1F4;
                } else {
                    self->actor.world.rot.y += 0x1F4;
                }
                self->actor.shape.rot.y = self->actor.world.rot.y;
            }
        }
        self->actor.shape.rot.z += 0x800;
        if (self->actor.shape.rot.z == 0) {
            Audio_PlaySoundGeneral(0x010002D5, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
        if (self->action >= 2) {
            CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)&self->collider);
        }
    }
    if (self->actor.params != 0xB) {
        self->sub.sparkTimer--;
        self->sub.sparkTimer &= 7;
    }
    self->actor.focus.pos = self->actor.world.pos;
    if (self->sub.dmgReaction == 0xE) {
        CollisionCheck_SetAT(play, &play->colChkCtx, (Collider*)&self->collider);
    }
    if ((self->sub.sparkTimer & 7) == 0) {
        sparkPrimColor.r = self->sub.redShineAmount;
        sparkAccel.x = self->actor.velocity.x * -0.25f;
        sparkAccel.y = self->actor.velocity.y * -0.25f;
        sparkAccel.z = self->actor.velocity.z * -0.25f;
        sparkPos.x = ((Rand_ZeroOne() - 0.5f) * 25.0f) + self->actor.world.pos.x;
        sparkPos.y = ((Rand_ZeroOne() - 0.5f) * 20.0f) + self->actor.world.pos.y;
        sparkPos.z = ((Rand_ZeroOne() - 0.5f) * 25.0f) + self->actor.world.pos.z;
        EffectSsKiraKira_SpawnSmall(play, &sparkPos, &sparkVel, &sparkAccel, &sparkPrimColor, &sparkEnvColor);
    }
    if ((self->action >= 2) && (self->actor.colChkInfo.health != 0)) {
        CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)&self->collider);
    }
}
#endif
GLOBAL_ASM("asm/EnTp_Update.s")

#if 0 // EnTp_Draw: 78/98 words off (SetUnkAC family: ROM keeps `mov r6,#1` hoisted before the params test => push {r4-r6,lr}/mtx at sp+0; ~25 variants of the 1-local/operator[]/scale-loop/decl-order shapes did not move it). GLOBAL_ASM below.
extern Vec3f EnTp_sScaleTable[];
void FUN_00373bec(void* obj);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_00371fac(MTX34* mtx, void* arg1);
void Collider_UpdateSpheres(s32 index, EnTpCollider* collider, MTX34* mtx);

void EnTp_Draw(Actor* thisx, GameState* state) {
    EnTp* self = (EnTp*)thisx;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;
    s32 one = 1;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    if (self->modeVar != 2) {
        if (self->actor.params < 0 || self->actor.params == 0xC) {
            GraphicsContext_UnknownStub(play->state.gfxCtx);
            if (self->scaleTimer != 0) {
                Vec3f* sc;
                f32 sx;
                f32 sy;
                f32 sz;

                self->scaleTimer--;
                sc = &EnTp_sScaleTable[0x25 - self->scaleTimer];
                sx = sc->x;
                sy = sc->y;
                sz = sc->z;
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
        } else {
            SkeletonAnimationModel_unk_0C* anim = &self->anim;

            anim->SetAnimSpeed(*(f32*)((u8*)play + 0x7F44) * 0.5f);
            FUN_00373bec(&self->anim);
            FUN_00371fac(&mtx, (u8*)play + 0x2FC);
        }
        self->model->SetUnkAC((u8)one);
        self->model->FUN_003721e0(&mtx);
        self->model->FUN_00372170(0);
    }
    if (self->actor.params <= 0 || self->actor.params == 0xB) {
        Collider_UpdateSpheres(0, &self->collider, &mtx);
    }
}
#endif
GLOBAL_ASM("asm/EnTp_Draw.s")

void FUN_0022fa68(EnTp* self, PlayState* play);
f32 Math_CosF(f32 angle);

void FUN_00295e58(EnTp* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnTpSub* sub;

    Math_SmoothStepToF(&self->actor.world.pos.y, player->actor.world.pos.y + 30.0f, 1.0f, 0.5f, 0.0f);
    Audio_PlaySoundGeneral(0x010002D4, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                           &DAT_0054ac24.unk_0); // NA_SE_EN_TAIL_FLY - SFX_FLAG
    sub = &self->sub;
    if (self->collider.atFlags & 2) {
        self->collider.atFlags &= ~2;
        if (&player->actor == self->collider.at) {
            sub->timer = 2;
        }
    }
    if (sub->redShineAmount < 255) {
        sub->redShineAmount += 15;
    }
    if (Math_CosF(sub->unk_168) == 0.0f) {
        sub->unk_170 = 2.0f * Rand_ZeroOne();
    }
    self->actor.world.pos.y += Math_CosF(sub->unk_168) * (2.0f + sub->unk_170);
    sub->unk_168 += 0.2f;
    Math_SmoothStepToF(&self->actor.speedXZ, 2.5f, 0.1f, 0.2f, 0.0f);
    sub->timer--;
    if (sub->timer != 0) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 0x2EE, 0);
        self->actor.shape.rot.y = self->actor.world.rot.y;
    } else {
        self->action = 9;
        self->actionFunc = FUN_0022fa68;
    }
}

void FUN_002a6da8(EnTp* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnTpSub* sub;

    Math_SmoothStepToF(&self->actor.speedXZ, 2.5f, 0.1f, 0.2f, 0.0f);
    sub = &self->sub;
    Math_SmoothStepToF(&self->actor.world.pos.y, player->actor.world.pos.y + sub->bodyElemDist + 85.0f, 1.0f,
                       self->actor.speedXZ * 0.25f, 0.0f);
    Audio_PlaySoundGeneral(0x010002D4, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                           &DAT_0054ac24.unk_0); // NA_SE_EN_TAIL_FLY - SFX_FLAG
    if (self->collider.atFlags & 2) {
        self->collider.atFlags &= ~2;
        if (&player->actor == self->collider.at) {
            sub->timer2 = 1;
        }
    }
    if (sub->redShineAmount != 0) {
        sub->redShineAmount -= 15;
    }
    if (Math_CosF(sub->unk_168) == 0.0f) {
        sub->unk_170 = Rand_ZeroOne() * 4.0f;
    }
    self->actor.world.pos.y += Math_CosF(sub->unk_168) * ((self->actor.speedXZ * 0.25f) + sub->unk_170);
    self->actor.world.rot.y += sub->unk_164;
    sub->unk_168 += 0.2f;
    if (sub->timer != 0) {
        sub->timer--;
    }
    Math_SmoothStepToS(&self->actor.world.rot.y,
                       Math_Atan2S(self->actor.home.pos.z - self->actor.world.pos.z,
                                   self->actor.home.pos.x - self->actor.world.pos.x),
                       1, 0x2EE, 0);
    if (sub->timer == 0) {
        self->action = 7;
        sub->timer = 300;
        self->actionFunc = FUN_00295e58;
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EffectSsDeadDb_Spawn(PlayState* play, Vector3f* pos, Vector3f* velocity, Vector3f* accel, s16 scale, s16 scaleStep, s16 primR,
                          s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s32 unk, s32 life, s32 playSfx);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);

void EnTp_Die(EnTp* self, PlayState* play) {
    PlayState* play2 = play;
    s16 i;
    EnTp* actor;
    Vector3f sp78(0.0f, 0.5f, 0.0f);
    Vector3f sp6C(0.0f, 0.0f, 0.0f);

    self->sub.timer--;
    if (self->sub.timer <= 0) {
        if (self->actor.params == 0xC) {
            sp6C.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + self->actor.world.pos.x;
            sp6C.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + self->actor.world.pos.z;
            sp6C.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + self->actor.world.pos.y;
            EffectSsDeadDb_Spawn(play2, &sp6C, &sp78, &sp78, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
            sp6C.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + self->actor.world.pos.x;
            sp6C.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + self->actor.world.pos.z;
            sp6C.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + self->actor.world.pos.y;
            EffectSsDeadDb_Spawn(play, &sp6C, &sp78, &sp78, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 11, 1);
            Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0x50);
        } else {
            for (i = 0; i < 1; i++) {
                actor = (EnTp*)Actor_Spawn(&play->actorCtx, play, 0x35, self->actor.world.pos.x,
                                           self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0xA, 1);
                if (actor != NULL) {
                    Actor_SetScale(&actor->actor, self->actor.scale.z * 0.5f);
                    actor->sub.redShineAmount = self->sub.redShineAmount;
                }
            }
        }
        if (self->actor.child != NULL) {
            self->actor.child->parent = NULL;
            self->actor.child->params = 0xB;
            self->actor.child->colChkInfo.health = 0;
        }
        self->modeVar = 2;
        Actor_Kill(&self->actor);
    }
}

void EnTp_Fragment_Fade(EnTp* self, PlayState* play) {
    FUN_0036b96c(&self->actor);
    self->sub.fadeAlpha -= 20;
    if (self->sub.fadeAlpha < 20) {
        self->sub.fadeAlpha = 0;
        Actor_Kill(&self->actor);
    }
}

void FUN_0038049c(EnTp* self) {
    self->actor.speedXZ = 0.0f;
    self->action = 4;
    self->modeVar = 0;
    self->actor.shape.rot.x = 0xC000;
    self->sub.timer = 0x5A;
    self->sub.timer2 = 0;
    self->actionFunc = FUN_0026e1e0;
}


#if 0 // EnTp_Tail_FollowHead: 1 word off (0x8: ROM `add r5,r4,#0x400`, ours `add r5,r0,#0x400`; ~45 variants of sub/self/thisx aliasing did not change it). GLOBAL_ASM below.
void EnTp_Tail_FollowHead(Actor* thisx, PlayState* play) {
    EnTp* self = (EnTp*)thisx;
    EnTpSub* sub = &self->sub;
    EnTp* head;
    s16 yaw;
    s16 angle;
    s32 r;

    Actor* parent;
    if (thisx->params == 0xB) {
        parent = thisx->parent;
        self->action = 1;
        if (parent == NULL) {
            sub->timer = 3;
            self->actionFunc = (EnTpActionFunc)EnTp_Die;
        }
        return;
    }
    parent = thisx->parent;
    if (self->modeVar != 0) {
        self->actor.flags |= 1;
    }
    head = self->headInstance;
    if (head->modeVar != 0) {
        self->actor.speedXZ = sub->redShineAmount = self->actor.velocity.y = sub->unk_168 = 0.0f;
        if (self->actor.world.pos.y < head->actor.home.pos.y) {
            self->actor.flags &= ~1;
        }
        self->actor.world.pos = parent->prevPos;
        return;
    }
    Math_SmoothStepToF(&self->actor.world.pos.y, parent->world.pos.y - 4.0f, 1.0f, 1.0f, 0.0f);
    yaw = (s16)(self->headInstance->actor.shape.rot.y + 0x4000);
    angle = (s16)((self->headInstance->sub.timer2 + sub->timer) * 0x7D0);
    if (angle > 0) {
        r = (s32)((f32)angle * 2.0f / 3.0f + 0.5f);
    } else {
        r = (s32)((f32)angle * 2.0f / 3.0f - 0.5f);
    }
    self->actor.world.pos.x = self->actor.home.pos.x + sub->bodyElemDist * (Math_SinS((s16)r) * Math_SinS(yaw));
    if (angle > 0) {
        r = (s32)((f32)angle * 2.0f / 3.0f + 0.5f);
    } else {
        r = (s32)((f32)angle * 2.0f / 3.0f - 0.5f);
    }
    self->actor.world.pos.z = self->actor.home.pos.z + sub->bodyElemDist * (Math_SinS((s16)r) * Math_CosS(yaw));
}
#endif
GLOBAL_ASM("asm/EnTp_Tail_FollowHead.s")

}
