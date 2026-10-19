// Separate TU: FUN_0036d188 (z_en_ik_anim.cpp) must not be inlined into the action (armcc inlines same-TU callees).
extern "C" {
#include "z_en_ik.hpp"

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 FUN_0036d188(EnIk* self, PlayState* play);
void FUN_0010b758(EnIk* self, PlayState* play);
void FUN_00181598(EnIk* self); // defined in z_en_ik.cpp
void FUN_00136fc8(EnIk* self, PlayState* play); // defined in z_en_ik.cpp
void FUN_0011c7e0(EnIk* self, PlayState* play);
void FUN_001374c0(EnIk* self, PlayState* play);

static inline f32 EnIk_AbsF(f32 x) {
    if (x >= 0.0f) {
        return x;
    }
    return -x;
}

// N64 EnIk_Idle. The two SetupWalkOrRun branches are the N64 else-if/else pair (the second compare is dead in the ROM,
// but keeps 150.0f live from entry).
void FUN_0010b758(EnIk* self, PlayState* play) {
    s32 thr = (self->unk_E0F == 0) ? 0xAAA : 0x3FFC;
    s16 yawDiff = (s16)((u16)self->actor.yawTowardsPlayer - (u16)self->actor.shape.rot.y);

    if (((yawDiff >= 0) ? yawDiff : -yawDiff) <= thr && self->actor.xzDistToPlayer < 100.0f &&
        EnIk_AbsF(self->actor.yDistToPlayer) < 150.0f) {
        if (*(u32*)((u8*)play + 0x5BF4) & 1) {
            f32 frames = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0);

            self->actor.speedXZ = 0.0f;
            self->unk_E13 = 1;
            self->unk_E0C = 6;
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0, 1.5f, 0.0f, frames, 2, -4.0f);
            self->actionFunc = (void*)FUN_0011c7e0;
        } else {
            f32 frames = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 3);

            self->actor.speedXZ = 0.0f;
            self->unk_E13 = 2;
            *(u16*)((u8*)self + 0xE18) = 0;
            self->unk_E0C = 6;
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 3, 0.0f, 0.0f, frames, 3, -6.0f);
            *((u8*)self + 0xE10) = 0;
            self->actionFunc = (void*)FUN_00136fc8;
        }
    } else if (((yawDiff >= 0) ? yawDiff : -yawDiff) <= 0x4000 && EnIk_AbsF(self->actor.yDistToPlayer) < 150.0f) {
        FUN_00181598(self);
    } else {
        FUN_00181598(self);
    }
    FUN_0036d188(self, play);
    FUN_00370734((SkelAnime*)self->unk_1A4);
}

void FUN_00196ba8(EnIk* self);
void FUN_0036627c(void* camera, s32 a, s32 b, s32 c);
void FUN_003661a8(PlayState* play, Vec3f* pos, s32 arg);

// N64 EnIk_VerticalAttack (3DS frame numbers: 15 -> 20, 21 -> 26, 17..23 -> 22..28, 10 stays)
void FUN_0011c7e0(EnIk* self, PlayState* play) {
    if (Animation_OnFrameImpl((SkelAnime*)self->unk_1A4, 20.0f, 0.6666667f)) {
        Audio_PlayActorSound2(&self->actor, 0x01000399);
    } else if (Animation_OnFrameImpl((SkelAnime*)self->unk_1A4, 26.0f, 0.6666667f)) {
        Vec3f sparksPos; // scoped here so it shares the stack slot with the SmoothStepToS stack arg

        sparksPos.x = self->actor.world.pos.x + Math_SinS(self->actor.shape.rot.y + 0x6A4) * 70.0f;
        sparksPos.z = self->actor.world.pos.z + Math_CosS(self->actor.shape.rot.y + 0x6A4) * 70.0f;
        sparksPos.y = self->actor.world.pos.y;
        Audio_PlayActorSound2(&self->actor, 0x0100041E);
        FUN_0036627c((u8*)play + 0x364, 2, 0x19, 5); // camera quake
        FUN_003661a8(play, &sparksPos, 8);           // shield particles
    }

    {
        f32 frame = *(f32*)((u8*)self + 0x1E0); // skelAnime.curFrame

        if (!(frame <= 22.0f)) {
            if (frame < 28.0f) {
                self->unk_E12 = 1;
                goto after;
            }
        }
        if (self->unk_E0F != 0 && frame < 10.0f) {
            Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 0x5DC, 0);
            self->actor.shape.rot.y = self->actor.world.rot.y;
        }
        self->unk_E12 = 0;
    }
after:
    if (FUN_003731e0((SkelAnime*)self->unk_1A4)) {
        f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 2);
        s32 t;

        // N64 SetupPullOutAxe, 3DS frame rounding (x * 3 / 2)
        if (endFrame > 0.0f) {
            t = (s32)(0.5f + (endFrame * 3.0f) * 0.5f);
        } else {
            t = (s32)((endFrame * 3.0f) * 0.5f - 0.5f);
        }
        self->unk_E0D = t;
        self->unk_E0C = 7;
        self->unk_E12 = 0;
        self->unk_E13 = 0;
        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 2, 1.0f, 0.0f, endFrame, 0, -4.0f);
        Audio_PlayActorSound2(&self->actor, 0x01000431);
        self->actionFunc = (void*)FUN_00196ba8;
    }
}

void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep, s16 primR,
                          s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unk1, s16 unk2, s32 unk3);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s16 params);
s32 FUN_00379cac(ActorContext* actorCtx, s16 id, u8 type);
void Actor_Kill(Actor* actor);

// N64 EnIk_Die (3DS: effects only every 3rd tick, 3DS frame number 39, effect param 9 -> 0xB)
void FUN_003d6f18(EnIk* self, PlayState* play) {
    if (FUN_003731e0((SkelAnime*)self->unk_1A4)) {
        if (*((u8*)self + 0xB7) == 0 && self->unk_E0D != 0) { // colChkInfo.health == 0 && animationTimer != 0
            s32 i;
            Vec3f pos;
            Vector3f sp7C(0.0f, 0.5f, 0.0f);

            self->unk_E0D--;

            if (self->unk_E0D % 3 != 0) {
                for (i = 0xC - (self->unk_E0D >> 1); i >= 0; i--) {
                    pos.x = self->actor.world.pos.x + Rand_CenteredFloat(120.0f);
                    pos.z = self->actor.world.pos.z + Rand_CenteredFloat(120.0f);
                    pos.y = self->actor.world.pos.y + 20.0f + Rand_CenteredFloat(50.0f);

                    EffectSsDeadDb_Spawn(play, &pos, (Vec3f*)&sp7C, (Vec3f*)&sp7C, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 0xB,
                                         1);
                }
            }

            if (self->unk_E0D == 0) {
                Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos, 0xB0);

                if (*(s16*)((u8*)self + 0xE1A) != 0xFF) { // switchFlag
                    Flags_SetSwitch(play, *(s16*)((u8*)self + 0xE1A));
                    if (FUN_00379cac(&play->actorCtx, self->actor.id, self->actor.type) <= 1) {
                        Player* player = GET_PLAYER(play);
                        u8* base = (u8*)player + 0x2000;

                        *(u32*)(base + 0x9B8) |= 0x3000000;
                        self->unk_E14 = 1;
                    }
                }
                Actor_Kill(&self->actor);
            }
        }
    } else if (Animation_OnFrameImpl((SkelAnime*)self->unk_1A4, 39.0f, 0.6666667f)) {
        Audio_PlayActorSound2(&self->actor, 0x01000398);
    }
}

typedef struct EnIkBodyBreak {
    /* 0x00 */ MTX34* matrices;
    /* 0x04 */ void** models;
    /* 0x08 */ s16 count;
    /* 0x0C */ s32 val;
} EnIkBodyBreak;

void SystemArena_Free(void* ptr);
void FUN_0026c4a0(Actor* self, PlayState* play, s32 arg2, void* arg3, void* entry, Vec3f* pos);

// N64 Actor_SpawnAsChild. Plain `static` (not `inline`): armcc still inlines it, but late, which keeps the ROM's
// hoisted NULL in sl (`moveq r0,sl`) and the `push {r0-r3,...}` spill of `type`.
static Actor* EnIk_SpawnAsChild(ActorContext* actorCtx, Actor* parent, PlayState* play, s16 actorId, f32 posX, f32 posY,
                                f32 posZ, s16 rotX, s16 rotY, s16 rotZ, s16 params) {
    Actor* spawnedActor = Actor_Spawn(actorCtx, play, actorId, posX, posY, posZ, rotX, rotY, rotZ, params, 1);

    if (spawnedActor == NULL) {
        return NULL;
    }
    parent->child = spawnedActor;
    spawnedActor->parent = parent;
    if (spawnedActor->room >= 0) {
        spawnedActor->room = parent->room;
    }
    return spawnedActor;
}

// N64 BodyBreak_SpawnParts (only caller FUN_003c6944). objBankIndex must be read into a local before the call.
s32 FUN_00232fac(Actor* actor, EnIkBodyBreak* bodyBreak, PlayState* play, s16 type, s32 arg5) {
    Actor* spawned;

    if (bodyBreak->val != -1) {
        return 0;
    }

    while (bodyBreak->count > 0) {
        MTX34* mtx = &bodyBreak->matrices[bodyBreak->count];
        f32 invX = 1.0f / actor->scale.x;
        f32 invY = 1.0f / actor->scale.y;
        f32 invZ = 1.0f / actor->scale.z;

        mtx->data[0][0] *= invX;
        mtx->data[1][0] *= invX;
        mtx->data[2][0] *= invX;
        mtx->data[0][1] *= invY;
        mtx->data[1][1] *= invY;
        mtx->data[2][1] *= invY;
        mtx->data[0][2] *= invZ;
        mtx->data[1][2] *= invZ;
        mtx->data[2][2] *= invZ;

        {
            s8 bank = actor->objBankIndex;

            spawned = EnIk_SpawnAsChild(&play->actorCtx, actor, play, 7,
                                        bodyBreak->matrices[bodyBreak->count].data[0][3],
                                        bodyBreak->matrices[bodyBreak->count].data[1][3],
                                        bodyBreak->matrices[bodyBreak->count].data[2][3], 0, 0, bank, type);
        }
        if (spawned != NULL) {
            FUN_0026c4a0(spawned, play, arg5, bodyBreak->models[bodyBreak->count], &bodyBreak->matrices[bodyBreak->count],
                         &actor->scale);
        }
        bodyBreak->count--;
    }

    bodyBreak->val = 0;
    if (bodyBreak->matrices != NULL) {
        SystemArena_Free(bodyBreak->matrices);
        bodyBreak->matrices = NULL;
    }
    if (bodyBreak->models != NULL) {
        SystemArena_Free(bodyBreak->models);
        bodyBreak->models = NULL;
    }
    return 1;
}

s32 FUN_003a7a2c(Actor* actor, Actor* other, f32 range, s16 angle);

// N64 EnIk_FindBreakableProp, literal N64 shape (`return prop` inside the loop, `return NULL` after it); the caller must
// test the call result directly (a `prop` local changes the loop block layout).
static inline Actor* EnIk_FindBreakableProp(PlayState* play, Actor* actor) {
    Actor* prop = play->actorCtx.actorLists[6].head;

    while (prop != NULL) {
        if ((prop == actor) || (prop->id != 0x169)) {
            prop = prop->next;
            continue;
        } else if (FUN_003a7a2c(actor, prop, 80.0f, 0x2710)) {
            return prop;
        }
        prop = prop->next;
    }
    return NULL;
}

// N64 EnIk_WalkOrRun
void FUN_003e9be4(EnIk* self, PlayState* play) {
    s16 thr;
    s16 step;
    s16 foot1;
    s16 foot2;
    s16 targetYaw;
    s16 yawDiff;

    if (self->unk_E0F == 0) {
        thr = 0xAAA;
        step = 0x320;
        foot1 = (s16)(*(f32*)((u8*)self + 0x1EC) - 0.6666667f);
        foot2 = 0xB;
    } else {
        thr = 0x3FFC;
        step = 0x4B0;
        foot1 = 2;
        foot2 = 9;
    }

    targetYaw = (s16)(self->actor.wallYaw - (u16)self->actor.shape.rot.y);

    if (self->unk_E15 == 0) {
        if ((self->actor.bgCheckFlags & 8) && ABS(targetYaw) >= 0x4000) {
            targetYaw = (self->actor.yawTowardsPlayer > 0) ? self->actor.wallYaw - 0x4000 : self->actor.wallYaw + 0x4000;
            Math_SmoothStepToS(&self->actor.world.rot.y, targetYaw, 1, step, 0);
            self->unk_E15 = 30;
            *(s16*)((u8*)self + 0xE16) = targetYaw;
        } else {
            Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, step, 0);
        }
    } else {
        Math_SmoothStepToS(&self->actor.world.rot.y, *(s16*)((u8*)self + 0xE16), 1, step, 0);
        self->unk_E15--;
        if (!(self->actor.bgCheckFlags & 8)) {
            self->unk_E15 = 0;
        }
    }

    {
        u16 rotY = self->actor.world.rot.y;

        self->actor.shape.rot.y = rotY;
        yawDiff = (s16)((u16)self->actor.yawTowardsPlayer - rotY);
    }

    if (ABS(yawDiff) <= thr && self->actor.xzDistToPlayer < 100.0f && EnIk_AbsF(self->actor.yDistToPlayer) < 150.0f) {
        if (*(u32*)((u8*)play + 0x5BF4) & 1) {
            f32 frames = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0);

            self->actor.speedXZ = 0.0f;
            self->unk_E13 = 1;
            self->unk_E0C = 6;
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0, 1.5f, 0.0f, frames, 2, -4.0f);
            self->actionFunc = (void*)FUN_0011c7e0;
        } else {
            f32 frames = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 3);

            self->actor.speedXZ = 0.0f;
            self->unk_E13 = 2;
            *(u16*)((u8*)self + 0xE18) = 0;
            self->unk_E0C = 6;
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 3, 0.0f, 0.0f, frames, 3, -6.0f);
            *((u8*)self + 0xE10) = 0;
            self->actionFunc = (void*)FUN_00136fc8;
        }
    }

    {
        if (EnIk_FindBreakableProp(play, &self->actor) != NULL) {
            f32 frames = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 3);

            self->actor.speedXZ = 0.0f;
            self->unk_E13 = 2;
            *(u16*)((u8*)self + 0xE18) = 0;
            self->unk_E0C = 6;
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 3, 0.0f, 0.0f, frames, 3, -6.0f);
            self->actionFunc = (void*)FUN_00136fc8;
            *((u8*)self + 0xE10) = 1;
        } else {
            yawDiff = (s16)((u16)self->actor.yawTowardsPlayer - (u16)self->actor.shape.rot.y);

            if (ABS(yawDiff) > 0x4000) {
                s16 t = (s16)(*(s16*)((u8*)self + 0xE18) - 1);

                *(s16*)((u8*)self + 0xE18) = t;
                if (t == 0) {
                    f32 frames = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 3);

                    self->actor.speedXZ = 0.0f;
                    self->unk_E0C = 1;
                    self->unk_E13 = 3;
                    Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 3, 0.5f, 18.0f, frames, 3, -4.0f);
                    self->actionFunc = (void*)FUN_001374c0;
                }
            } else {
                *(s16*)((u8*)self + 0xE18) = 0x3C;
            }
        }
    }

    FUN_0036d188(self, play);
    FUN_00370734((SkelAnime*)self->unk_1A4);
    if (Animation_OnFrameImpl((SkelAnime*)self->unk_1A4, (f32)foot1, 0.6666667f) ||
        Animation_OnFrameImpl((SkelAnime*)self->unk_1A4, (f32)foot2, 0.6666667f)) {
        Audio_PlayActorSound2(&self->actor, 0x01000398);
    }
}


// 3DS EffectBlure init params (N64 EffectBlureInit1). The vertex array element has an out-of-line empty constructor in
// the ROM (FUN_003fd780, aliased as _ZN12BlureInitVertexC1Ev in data_syms.s).
struct BlureInitVertex {
    BlureInitVertex();
    u8 data[0x24];
};

typedef struct {
    /* 0x000 */ s32 unk_00;
    /* 0x004 */ BlureInitVertex vtx[16];
    /* 0x244 */ u8 p1StartColor[4];
    /* 0x248 */ u8 p2StartColor[4];
    /* 0x24C */ u8 p1EndColor[4];
    /* 0x250 */ u8 p2EndColor[4];
    /* 0x254 */ s32 elemDuration;
    /* 0x258 */ s32 unkFlag;
    /* 0x25C */ u8 calcMode;
    /* 0x260 */ s32 unk_260;
    /* 0x264 */ u8 unk_264;
} EnIkBlureInit; // size = 0x268

extern ColliderCylinderInit EnIk_sCylinderInit;
extern u8 EnIk_sTrisInit[];
extern u8 EnIk_sQuadInit[];
extern u8 EnIk_sDamageTable[];
s32 Collider_InitTris(PlayState* play, void* collider);
s32 Collider_SetTris(PlayState* play, void* collider, Actor* actor, void* src, void* elements);
s32 Collider_InitQuad(PlayState* play, void* collider);
s32 Collider_SetQuad(PlayState* play, void* collider, Actor* actor, void* src);
s32 Effect_Add(PlayState* play, s32* pIndex, s32 type, u8 arg3, u8 arg4, void* initParams);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, void* jointTable,
                  void* morphTable, s32 limbCount);
void FUN_0034f724(PlayState* play);
void FUN_001008b4(EnIk* self, PlayState* play); // N64 EnIk_UpdateEnemy
void FUN_0014be68(EnIk* self); // N64 EnIk_DrawEnemy
void FUN_003e45ac(EnIk* self, PlayState* play);
void EnIk_Update(Actor* actor, GameState* state);
void EnIk_Draw(Actor* actor, GameState* state);
void EnIk_Init(EnIk* self, PlayState* play);

// Object bank lookup: NULL if the slot is out of range or not loaded.
static inline u8* EnIk_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

static inline void EnIk_SetupAction(EnIk* self, void* actionFunc) {
    self->actionFunc = actionFunc;
}

static inline void EnIk_SetupStandUp(EnIk* self) {
    f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0xB);
    f32 startFrame;

    if (self->actor.params >= 2) {
        startFrame = endFrame - 1.0f;
    } else {
        startFrame = 0.0f;
    }
    Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0xB, 0.0f, startFrame, endFrame, 2, 0.0f);
    self->actor.speedXZ = 0.0f;
    self->unk_E0C = 3;
    EnIk_SetupAction(self, (void*)FUN_003e45ac);
}

static inline void EnIk_InitImpl(Actor* thisx, PlayState* play) {
    EnIk* self = (EnIk*)thisx;
    EnIkBlureInit blureInit;

    thisx->update = (ActorFunc)FUN_001008b4;
    thisx->draw = (ActorFunc)FUN_0014be68;
    thisx->flags |= 0x400;

    Collider_InitCylinder(play, &self->bodyCollider);
    Collider_SetCylinder(play, &self->bodyCollider, thisx, &EnIk_sCylinderInit);
    Collider_InitTris(play, self->shieldCollider);
    Collider_SetTris(play, self->shieldCollider, thisx, EnIk_sTrisInit, (u8*)self + 0xF2C);
    Collider_InitQuad(play, self->axeCollider);
    Collider_SetQuad(play, self->axeCollider, thisx, EnIk_sQuadInit);

    *(void**)((u8*)thisx + 0xA0) = EnIk_sDamageTable;
    *((u8*)thisx + 0xB6) = 0xFE;
    self->unk_E10[0] = 0;
    *((u8*)thisx + 0xB7) = 30;
    thisx->gravity = -1.0f;
    *(s16*)((u8*)self + 0xE1A) = (thisx->params >> 8) & 0xFF;
    thisx->params = thisx->params & 0xFF;

    if (thisx->params == 0) {
        *((u8*)thisx + 0xB7) = 50;
        thisx->naviEnemyId = 0x34;
    } else {
        Actor_SetScale(thisx, 0.012f);
        thisx->naviEnemyId = 0x35;
        Actor_ChangeType(play, &play->actorCtx, thisx, 5);
    }

    blureInit.p1StartColor[0] = blureInit.p1StartColor[1] = blureInit.p2StartColor[0] = blureInit.p2StartColor[1] =
        blureInit.p2StartColor[2] = blureInit.p1EndColor[0] = blureInit.p1EndColor[1] = blureInit.p2EndColor[0] =
            blureInit.p2EndColor[1] = blureInit.p2EndColor[2] = 255;
    blureInit.p2StartColor[3] = 64;
    blureInit.p1StartColor[3] = 200;
    blureInit.p1StartColor[2] = blureInit.p1EndColor[2] = 150;
    blureInit.p1EndColor[3] = blureInit.p2EndColor[3] = 0;
    blureInit.elemDuration = 8;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 2;
    blureInit.unk_264 = (thisx->params == 0) ? 0x14 : 0x13;

    Effect_Add(play, (s32*)((u8*)self + 0xFE4), 1, 0, 0, &blureInit);
    EnIk_SetupStandUp(self);

    if (*(s16*)((u8*)self + 0xE1A) != 0xFF) {
        if (Flags_GetSwitch(play, *(s16*)((u8*)self + 0xE1A))) {
            Actor_Kill(thisx);
        }
    } else if (thisx->params != 0 && Flags_GetClear(play, *(s8*)((u8*)play + 0x4C30))) {
        Actor_Kill(thisx);
    }
}

static inline void EnIk_CsInit(EnIk* self, PlayState* play) {
    if (self->actor.params == 0) {
        if (!(gSaveContext.eventChkInf[3] & 0x800)) {
            self->actor.update = (ActorFunc)EnIk_Update;
            self->actor.draw = (ActorFunc)EnIk_Draw;
            Actor_SetScale(&self->actor, 0.01f);
        } else {
            f32 endFrame;

            self->actor.update = (ActorFunc)FUN_001008b4;
            self->actor.draw = (ActorFunc)FUN_0014be68;
            self->actor.flags |= 5;
            gSaveContext.eventChkInf[3] |= 0x800;
            Actor_SetScale(&self->actor, 0.012f);
            endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0xC);
            self->actor.flags |= 5;
            self->actor.speedXZ = 0.0f;
            self->unk_E0C = 4;
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0xC, 0.0f, 0.0f, endFrame, 0, 4.0f);
            self->actionFunc = (void*)FUN_0010b758;
            FUN_0034f724(play);
        }
    }
}

static inline void EnIk_InitShape(ActorShape* shape, f32 shadowScale) {
    ActorShape_Init(shape, 0.0f, ActorShadow_DrawCircle, shadowScale);
}

void EnIk_Init(EnIk* self, PlayState* play) {
    EnIk* s2 = self;
    PlayState* play2 = play;
    if (((self->actor.params & 0xFF) == 0 && (gSaveContext.eventChkInf[3] & 0x1000)) ||
        ((self->actor.params & 0xFF00) != 0 && Flags_GetSwitch(play, (self->actor.params & 0xFF00) >> 8))) {
        Actor_Kill(&self->actor);
    } else {
        EnIk_InitShape(&self->actor.shape, 30.0f);
        self->actor.unk_19A = 1;
        s2->unk_E14 = 0;
        {
            SkelAnime* skel = (SkelAnime*)self->unk_1A4;
            void* zar = EnIk_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10;
            void* cmb;

            *(void**)((u8*)self + 0xDF0) = zar;
            cmb = ZAR_GetCMBByIndex(zar, 0);
            FUN_00353e78(*(void**)((u8*)self + 0xDF0), play, skel, cmb, self->actor.unk_178, 0xA,
                         (u8*)self + 0x228, (u8*)self + 0x80C, 0x1D);
        }
        EnIk_InitImpl(&self->actor, play2);
        EnIk_CsInit(self, play);
    }
}


#if 0
// Draft (NOT compiled): FUN_001008b4 = N64 EnIk_UpdateEnemy with UpdateDamage / StartDefeatCutscene / SetupDie /
// SetupReactToAttack inlined (2020 B vs ROM 2024 B; structure matches, ~28 of the first 230 insns differ). Open: ROM
// hoists world.pos base (r8), 0xE00 base (sb), 1 (fp) and 0 (sl) at entry and keeps play in r5 with a spill at [sp,#0x10]
// only for the inlined UpdateDamage calls; ours hoists differently (greedy dead-alias search: s2/play2/thisx, 3 rounds,
// local minimum). Needs EQUs EnIk_gSfxDefaultFreqAndVolScale (0x54AC20) / EnIk_gSfxDefaultReverb (0x54AC24) in data_syms.s.
void BodyBreak_Alloc(void* bodyBreak, s32 count, PlayState* play);
void FUN_00375f90(PlayState* play, Vec3f* pos, s32 arg);
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);
void FUN_00375b70(PlayState* play, Actor* actor);
void FUN_00374bb8(PlayState* play, Actor* actor, f32 arg2, s16 arg3, f32 arg4);
u32 FUN_00374be8(PlayState* play, u32 arg1);
s32 Gameplay_InCsMode(PlayState* play);
void FUN_003d6f18(EnIk* self, PlayState* play);
void FUN_003c6944(EnIk* self, PlayState* play);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
u8 Actor_ApplyDamage(Actor* actor);
void Flags_SetSwitch(PlayState* play, s32 flag);
void Audio_PlaySoundAtPosition(PlayState* play, Vec3f* pos, s32 duration, u32 sfxId);
void Audio_PlaySoundGeneral(u32 sfxId, Vec3f* a1, u8 a2, f32* a3, f32* a4, s8* a5);
extern f32 EnIk_gSfxDefaultFreqAndVolScale[];
extern s8 EnIk_gSfxDefaultReverb[];
extern s16 Math_Atan2S(f32 x, f32 y);

static inline void EnIk_SetupSingleHorizontalAttack(EnIk* self) {
    f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 3);

    self->actor.speedXZ = 0.0f;
    self->unk_E0C = 1;
    self->unk_E13 = 3;
    Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 3, 0.5f, 18.0f, endFrame, 3, -4.0f);
    EnIk_SetupAction(self, (void*)FUN_001374c0);
}

static inline void EnIk_UpdateDamage(EnIk* self, PlayState* play) {
    u8 prevHealth;

    if (self->unk_E0C == 3 || self->unk_E0C == 2) {
        return;
    }
    {
        if (*((u8*)self + 0xF1D) & 0x80) {
            f32 frames = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 6) - 2.0f;

            if (*(f32*)((u8*)self + 0x1E0) < frames) {
                *(f32*)((u8*)self + 0x1E0) = frames;
            }
            *((u8*)self + 0xF1D) &= ~0x80;
            *((u8*)self + 0xE45) &= ~2;
        } else if (*((u8*)self + 0xE45) & 2) {
            Vec3f sparksPos = self->actor.world.pos;

            sparksPos.y += 50.0f;
            FUN_00375fd0(&self->actor, (u8*)self + 0xE4C, 1);
            *((u8*)self + 0xE11) = *((u8*)&self->actor + 0xB9);
            *((u8*)self + 0xE45) &= ~2;

            if (*((u8*)self + 0xE11) == 0) {
                return;
            }
            if (*((u8*)self + 0xE11) == 0xD || *((u8*)self + 0xE11) == 6 ||
                (self->unk_E0F == 0 && *((u8*)self + 0xE11) == 0xE)) {
                FUN_00375f90(play, &sparksPos, 8);
                return;
            }

            Actor_SetColorFilter(&self->actor, 0x400000, 0xFF, 0, 0xC);
            prevHealth = *((u8*)&self->actor + 0xB7);
            Actor_ApplyDamage(&self->actor);

            if (self->actor.params != 0) {
                if (prevHealth > 10 && *((u8*)&self->actor + 0xB7) <= 10) {
                    self->unk_E0F = 1;
                    BodyBreak_Alloc((u8*)self + 0xE20, 2, play);
                }
            } else if (*((u8*)&self->actor + 0xB7) <= 10) {
                Actor_ChangeType(play, &play->actorCtx, &self->actor, 9);
                Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 0x14, 0x010002FB);
                if (*(s16*)((u8*)self + 0xE1A) != 0xFF) {
                    Flags_SetSwitch(play, *(s16*)((u8*)self + 0xE1A));
                }
                return;
            } else if (prevHealth == 0x32) {
                Actor_ChangeType(play, &play->actorCtx, &self->actor, 5);
            }

            if (*((u8*)&self->actor + 0xB7) == 0) {
                f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 7);

                self->unk_E12 = 0;
                self->actor.speedXZ = 0.0f;
                self->unk_E0C = 2;
                Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 7, 1.0f, 0.0f, endFrame, 2, -4.0f);
                self->unk_E0D = 0x24;
                Audio_PlayActorSound2(&self->actor, 0x01000430);
                Audio_PlayActorSound2(&self->actor, 0x010003AB);
                EnIk_SetupAction(self, (void*)FUN_003d6f18);
                FUN_00375b70(play, &self->actor);
                return;
            }

            Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 0x7D0, 0);

            if (self->actor.params == 0 && Rand_ZeroOne() < 0.5f) {
                if (ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) > 0x4000) {
                    EnIk_SetupSingleHorizontalAttack(self);
                }
            }

            if (self->actor.params != 0 && self->unk_E0F != 0) {
                if (prevHealth > 10 && *((u8*)&self->actor + 0xB7) <= 10) {
                    Audio_PlayActorSound2(&self->actor, 0x0100039C);
                } else {
                    Audio_PlayActorSound2(&self->actor, 0x0100042E);
                    Audio_PlayActorSound2(&self->actor, 0x010003AB);
                }
                {
                    Actor* ac = *(Actor**)((u8*)self + 0xE3C);
                    s16 yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &ac->world.pos);
                    s16 yawDiff;

                    self->unk_E0C = 0;
                    yawDiff = yaw - self->actor.shape.rot.y;
                    if (ABS(yawDiff) <= 0x4000) {
                        f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 8);

                        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 8, 1.0f, 0.0f, endFrame, 2, -4.0f);
                        self->actor.speedXZ = -6.0f;
                    } else {
                        f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 5);

                        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 5, 1.0f, 0.0f, endFrame, 2, -4.0f);
                        self->actor.speedXZ = 6.0f;
                    }
                    self->unk_E12 = 0;
                    EnIk_SetupAction(self, (void*)FUN_003c6944);
                }
            } else {
                Audio_PlayActorSound2(&self->actor, 0x010003AA);
                Audio_PlayActorSound2(&self->actor, 0x0100042E);
                FUN_00375f90(play, &self->actor.world.pos, 8);
            }
        }
    }
}

// N64 EnIk_UpdateEnemy with EnIk_StartDefeatCutscene inlined (UpdateDamage as static inline).
void FUN_001008b4(EnIk* self, PlayState* play) {
    Actor* thisx = &self->actor;
    PlayState* play2 = play;
    EnIk* s2 = self;
    Player* player = GET_PLAYER(play);
    u8 prevTimer;

    self->unk_E0E = self->unk_E0F;
    EnIk_UpdateDamage(self, play2);

    if (self->actor.params == 0 && *((u8*)thisx + 0xB7) <= 10) {
        if (!Gameplay_InCsMode(play)) {
            u8* cue = NULL;
            f32 endFrame;

            self->actor.update = (ActorFunc)EnIk_Update;
            self->actor.draw = (ActorFunc)EnIk_Draw;
            play->FUN_0037573c(((ZARInfo*)((u8*)play + 0x118))->GetQDBByIndex(1));
            gSaveContext.cutsceneTrigger = 1;
            Actor_SetScale(&self->actor, 0.01f);
            gSaveContext.eventChkInf[3] |= 0x1000;
            endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0x10);
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0x10, 1.0f, 0.0f, endFrame, 2, 0.0f);
            self->updateIndex = 3;
            self->drawIndex = 2;
            if (play->GetCutsceneState() != 0) {
                cue = *(u8**)((u8*)play + 0x22EC);
            }
            if (cue != NULL) {
                self->actor.world.pos.x = (f32) * (s32*)(cue + 0xC);
                self->actor.world.pos.y = (f32) * (s32*)(cue + 0x10);
                self->actor.world.pos.z = (f32) * (s32*)(cue + 0x14);
                self->actor.shape.rot.y = *(u16*)(cue + 8);
                self->actor.world.rot.y = *(u16*)(cue + 8);
            }
            Audio_PlaySoundGeneral(0x01000430, &self->actor.world.pos, 4, EnIk_gSfxDefaultFreqAndVolScale,
                                   EnIk_gSfxDefaultFreqAndVolScale, EnIk_gSfxDefaultReverb);
            self->actor.shape.shadowAlpha = 0xFF;
        }
    } else {
        ((void (*)(EnIk*, PlayState*))self->actionFunc)(self, play);

        if (*((u8*)self + 0xE9C) & 2) {
            *((u8*)self + 0xE9C) &= ~2;
            if (*(Actor**)((u8*)self + 0xE90) == (Actor*)player) {
                u8* pl = (u8*)player + 0x2000;

                prevTimer = pl[0x488];
                if (((s8*)pl)[0x488] <= 0) {
                    if (FUN_00374be8(play, 0xE) != 0) {
                        s8 t = ((s8*)pl)[0x488];

                        pl[0x488] = 0;
                        if (t > -0x28) {
                            (*(void (**)(PlayState*, s32))((u8*)play + 0x5BAC))(play, -0x40);
                            s2->unk_E12 = 0;
                        }
                    }
                }
                FUN_00374bb8(play, &self->actor, 8.0f, self->actor.yawTowardsPlayer, 8.0f);
                pl[0x488] = prevTimer;
            }
        }

        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 75.0f, 30.0f, 30.0f, 0x1D);
        self->actor.focus.pos = self->actor.world.pos;
        self->actor.focus.pos.y += 45.0f;
        Collider_UpdateCylinder(&self->actor, &self->bodyCollider);
        CollisionCheck_SetOC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), (Collider*)&self->bodyCollider);

        if (*((u8*)&self->actor + 0xB7) > 0 && *(u16*)((u8*)&self->actor + 0x11A) == 0 && self->unk_E0C >= 2) {
            CollisionCheck_SetAC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), (Collider*)&self->bodyCollider);
        }
        if (*((s8*)self + 0xE12) > 0) {
            CollisionCheck_SetAT(play, (CollisionCheckContext*)((u8*)play + 0x5C78), (Collider*)self->axeCollider);
        }
        if (self->unk_E0C == 9) {
            CollisionCheck_SetAC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), (Collider*)self->shieldCollider);
        }
    }
}

#endif
}
