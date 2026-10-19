extern "C" {
#include "z_en_ik.hpp"

#define FLAGS 0x00000010

void EnIk_Init(Actor* actor, GameState* state);
void EnIk_Destroy(Actor* actor, PlayState* play);
void EnIk_Update(Actor* actor, GameState* state);
void EnIk_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Ik.data.s")

GLOBAL_ASM("../src/game/actors/En_Ik/data_syms.s")
extern ActorFunc EnIk_sUpdateFuncs[];
extern ActorFunc EnIk_sDrawFuncs[];
extern u8 gUnk_004D86CC[];

ActorInit En_Ik_InitVars = {
    ACTOR_EN_IK,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_IK,
    sizeof(EnIk),
    (ActorFunc)EnIk_Init,
    (ActorFunc)EnIk_Destroy,
    (ActorFunc)EnIk_Update,
    (ActorFunc)EnIk_Draw,
};

// EnIk_Init is defined in z_en_ik_act.cpp (needs its own TU for the callee-register layout).

Actor* Actor_FindNearby(PlayState* play, Actor* actor, s32 actorId, s32 actorCategory, f32 maxDist);
void FUN_00373d0c(PlayState* play);
void Collider_DestroyTris(PlayState* play, void* collider);
s32 EnIk_ColliderDestroyStub(PlayState* play, void* collider); // defined in z_en_ik_anim.cpp
void FUN_003504d0(void* a, void* b);

void EnIk_Destroy(Actor* thisx, PlayState* play) {
    EnIk* self = (EnIk*)thisx;

    if (self->unk_E14 != 0) {
        Player* player = GET_PLAYER(play);
        u8* base = (u8*)player + 0x2000;
        *(u32*)(base + 0x9B8) &= ~0x3000000;
    }
    if (Actor_FindNearby(play, &self->actor, ACTOR_EN_IK, ACTORCAT_ENEMY, 8000.0f) == NULL) {
        FUN_00373d0c(play);
    }
    Collider_DestroyTris(play, self->shieldCollider);
    EnIk_ColliderDestroyStub(play, &self->bodyCollider);
    EnIk_ColliderDestroyStub(play, self->axeCollider);
    FUN_003504d0(self, self->unk_E20);
    SkelAnime_Free2((SkelAnime*)self->unk_1A4);
}

void EnIk_Update(Actor* actor, GameState* state) {
    EnIk* self = (EnIk*)actor;
    ActorFunc func;

    if (self->updateIndex < 6) {
        func = EnIk_sUpdateFuncs[self->updateIndex];
        if (func != NULL) {
            func(actor, state);
        }
    }
}

void EnIk_Draw(Actor* actor, GameState* state) {
    EnIk* self = (EnIk*)actor;
    ActorFunc func;

    if (self->drawIndex < 3) {
        func = EnIk_sDrawFuncs[self->drawIndex];
        if (func != NULL) {
            func(actor, state);
        }
    }
}


void FUN_003468fc(EnIk* self); // defined in z_en_ik_anim.cpp (separate TU: must not be inlined here)
s32 FUN_0036d188(EnIk* self, PlayState* play);

void FUN_001c67f4(EnIk* self, PlayState* play) {
    if (FUN_003731e0((SkelAnime*)self->unk_1A4)) {
        FUN_003468fc(self);
        FUN_0036d188(self, play);
    }
}

__attribute__((noreturn)) void EnIk_EmptyCall(PlayState* play, Vec3f* pos, s32 arg);
void EnIk_EmptyCall(PlayState* play, Vec3f* pos, s32 arg) {
}

void FUN_00375f88(PlayState* play, Vec3f* pos); // defined in z_en_ik_anim.cpp (separate TU: must not be inlined here)


s32 FUN_00379cac(ActorContext* actorCtx, s16 id, u8 type) {
    Actor* actor = actorCtx->actorLists[type].head;
    s32 count = 0;

    while (actor != NULL) {
        if (actor->id == id) {
            count++;
        }
        actor = actor->next;
    }
    return count;
}

s32 FUN_003a7a2c(Actor* actor, Actor* other, f32 range, s16 angle) {
    f32 dist = Math_Vec3f_DistXYZ(&actor->world.pos, &other->world.pos);
    s32 diff;

    if (dist < range) {
        diff = (s16)(Math_Vec3f_Yaw(&actor->world.pos, &other->world.pos) - actor->shape.rot.y);
        if (diff < 0) {
            diff = -diff;
        }
        if (diff < angle) {
            return 1;
        }
    }
    return 0;
}

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

void FUN_00196ba8(EnIk* self) {
    if (FUN_00370734((SkelAnime*)self->unk_1A4) == 0) {
        self->unk_E0D = (u8)(self->unk_E0D - 1);
        if (self->unk_E0D != 0) {
            return;
        }
    }
    if (self->unk_E0C == 8) {
        FUN_003468fc(self);
        return;
    }
    {
        f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 1);
        self->unk_E0C = 8;
        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 1, 1.5f, 0.0f, endFrame, 3, -4.0f);
    }
}

typedef struct EnIkAnimCtx {
    void* vtbl;
    void* self;
    void* play;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} EnIkAnimCtx;

void FUN_003624c8(void* entry, ActorShape* shape, s32 arg2);
void FUN_00347774(void* skelAnimModel, EnIkAnimCtx* ctx);
void FUN_00339d90(Actor* actor, PlayState* play);

void FUN_0026c4a0(EnIk* self, PlayState* play, s32 arg2, s32 arg3, void* entry, Vec3f* pos) {
    EnIkAnimCtx* ctx;

    *(Vec3f*)((u8*)self + 0x54) = *pos;
    FUN_003624c8(entry, &self->actor.shape, 0);
    if (self->actor.params == 5 || self->actor.params == 6 || self->actor.params == 7 || self->actor.params == 8) {
        if (arg3 == 0x11) {
            arg2 = 1;
        } else {
            arg2 = 3;
        }
        arg3 = 0;
    }
    *(s32*)((u8*)self + 0x1DC) = arg3;
    *((u8*)self + 0x1D1) = 0;
    FUN_00372f38(&self->actor, play, (SkeletonAnimationModel**)self->unk_1A4, arg2, 0);
    *(s32*)((u8*)self + 0x1A8) = arg2;
    ctx = (EnIkAnimCtx*)((u8*)self + 0x1AC);
    if (ctx != NULL) {
        ctx->self = NULL;
        ctx->play = NULL;
        ctx->unk_0C = 0;
        ctx->unk_10 = 0;
        ctx->vtbl = (void*)0x004EBFA0;
        ctx->unk_14 = 0;
    }
    *(EnIkAnimCtx**)((u8*)self + 0x1C4) = ctx;
    ctx->self = self;
    ctx->play = play;
    FUN_00347774(*(void**)self->unk_1A4, *(EnIkAnimCtx**)((u8*)self + 0x1C4));
    FUN_00339d90(&self->actor, play);
}

void FUN_003e9be4(EnIk* self, PlayState* play);

static inline void EnIk_SetupAction(EnIk* self, void* actionFunc) {
    self->actionFunc = actionFunc;
}

void FUN_00181598(EnIk* self) {
    f32 speed;

    self->unk_E0C = 5;
    if (self->unk_E0F == 0) {
        f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0xE);
        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0xE, 1.0f, 0.0f, endFrame, 0, -4.0f);
        speed = 0.9f;
    } else {
        f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 9);
        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 9, 1.0f, 0.0f, endFrame, 0, -4.0f);
        Audio_PlayActorSound2(&self->actor, 0x0100042F);
        speed = 2.5f;
    }
    self->actor.speedXZ = speed;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->unk_E15 = 0;
    EnIk_SetupAction(self, (void*)FUN_003e9be4);
}

s32 FUN_003c43bc(s16* pValue, s16 limit, s16 step); // N64 Math_StepUntilS, defined in z_en_ik_anim.cpp (not inlined)

// N64 EnIk_SetupRecoverFromHorizontalAttack
static inline void EnIk_SetupRecoverFromHorizontalAttack(EnIk* self) {
    f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 4);

    self->unk_E13 = self->unk_E12 = 0;
    self->unk_E0C = 8;
    Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 4, 1.5f, 0.0f, endFrame, 3, -4.0f);
    EnIk_SetupAction(self, (void*)FUN_001c67f4);
}

// N64 EnIk_SingleHorizontalAttack. The range test must be nested ifs sharing one `unk_E12 = 0` store (an `&&` gets
// folded into an unsigned range compare of the float bits).
void FUN_001374c0(EnIk* self, PlayState* play) {
    FUN_003c43bc(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 2000);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (*(f32*)((u8*)self + 0x1E0) > 18.0f) {
        if (*(f32*)((u8*)self + 0x1E0) < 23.0f) {
            if ((s8)self->unk_E12 < 0) {
                Audio_PlayActorSound2(&self->actor, 0x01000399);
            }
            self->unk_E12 = 1;
            goto done;
        }
    }
    self->unk_E12 = 0;
done:
    if (FUN_003731e0((SkelAnime*)self->unk_1A4)) {
        EnIk_SetupRecoverFromHorizontalAttack(self);
        FUN_0036d188(self, play);
    }
}

void FUN_00357a50(SkelAnime* skelAnime, s32 idx, s32 count, void* table, s32 flag);
void FUN_0037266c(void* model, s32 idx);
void FUN_0036932c(void* model, s32 idx);
void FUN_0016ee30(void); // post-limb draw callback
void* FUN_00357a70(s32 idx); // defined in z_en_ik_anim.cpp

void FUN_0014be68(EnIk* self) {
    void* table = FUN_00357a70(self->actor.params);

    FUN_00357a50((SkelAnime*)self->unk_1A4, 0, 4, table, 0);
    FUN_00357a50((SkelAnime*)self->unk_1A4, 1, 4, table, 0);
    FUN_00357a50((SkelAnime*)self->unk_1A4, 2, 3, (u8*)table + 0x10, 0);
    FUN_00357a50((SkelAnime*)self->unk_1A4, 3, 2, (u8*)table + 0x20, 0);
    if (self->actor.params != 0) {
        FUN_0037266c(*(void**)((u8*)self + 0x1CC), 7);
        FUN_0036932c(*(void**)((u8*)self + 0x1CC), 8);
        FUN_0036932c(*(void**)((u8*)self + 0x1CC), 6);
    } else {
        FUN_0036932c(*(void**)((u8*)self + 0x1CC), 7);
        FUN_0037266c(*(void**)((u8*)self + 0x1CC), 8);
        FUN_0037266c(*(void**)((u8*)self + 0x1CC), 6);
    }
    if (self->unk_E0E & 1) {
        FUN_0036932c(*(void**)((u8*)self + 0x1CC), 1);
        FUN_0036932c(*(void**)((u8*)self + 0x1CC), 0);
    } else {
        FUN_0037266c(*(void**)((u8*)self + 0x1CC), 1);
        FUN_0037266c(*(void**)((u8*)self + 0x1CC), 0);
    }
    SkelAnime_DrawOpa((SkelAnime*)self->unk_1A4, (MTX34*)((u8*)self + 0x148), NULL, (PostLimbDrawOpa)FUN_0016ee30,
                      &self->actor, 0);
}

void FUN_0010b758(EnIk* self, PlayState* play);
void FUN_001374c0(EnIk* self, PlayState* play);
s32 FUN_00232fac(EnIk* self, void* entry, PlayState* play, s16 arg, s32 arg5);

void FUN_003c6944(EnIk* self, PlayState* play) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);
    if (FUN_00232fac(self, &self->unk_E20, play, (s16)(self->actor.params + 4), 0) != 0) {
        *(s32*)((u8*)self + 0xE2C) = 0;
    }
    if (FUN_00370734((SkelAnime*)self->unk_1A4) == 0) {
        return;
    }
    s16 yawDiff = (s16)((u16)self->actor.yawTowardsPlayer - (u16)self->actor.shape.rot.y);
    if ((u32)(yawDiff + 0x4000) <= 0x8000) {
        f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0xC);
        self->actor.flags |= 5;
        self->actor.speedXZ = 0.0f;
        self->unk_E0C = 4;
        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0xC, 0.0f, 0.0f, endFrame, 0, 4.0f);
        self->actionFunc = (void*)FUN_0010b758;
        FUN_0036d188(self, play);
    } else {
        f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 3);
        self->actor.speedXZ = 0.0f;
        self->unk_E0C = 1;
        self->unk_E13 = 3;
        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 3, 0.5f, 18.0f, endFrame, 3, -4.0f);
        self->actionFunc = (void*)FUN_001374c0;
    }
}

void FUN_0034f724(PlayState* play);

void FUN_003e45ac(EnIk* self, PlayState* play) {
    f32 speed;
    f32 one = 1.0f;

    if (*((u8*)self + 0xE45) & 2) {
        Vec3f pos = self->actor.world.pos;

        Audio_PlayActorSound2(&self->actor, 0x010003AA);
        pos.y += 30.0f;
        FUN_00375f88(play, &pos);
        *(f32*)((u8*)self + 0x1E4) = one;
        FUN_0034f724(play);
    }
    if (Animation_OnFrameImpl((SkelAnime*)self->unk_1A4, 5.0f, 0.6666667f)) {
        Audio_PlayActorSound2(&self->actor, 0x01000432);
    }
    if (FUN_00370734((SkelAnime*)self->unk_1A4)) {
        Actor* thisx = &self->actor;

        thisx->flags |= 5;
        f32 morph = -4.0f;
        f32 start = 0.0f;
        self->unk_E0C = 5;
        if (self->unk_E0F == 0) {
            f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0xE);
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0xE, one, start, endFrame, 0, morph);
            speed = 0.9f;
        } else {
            f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 9);
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 9, one, start, endFrame, 0, morph);
            Audio_PlayActorSound2(&self->actor, 0x0100042F);
            speed = 2.5f;
        }
        self->actor.speedXZ = speed;
        self->actor.world.rot.y = self->actor.shape.rot.y;
        self->unk_E15 = 0;
        EnIk_SetupAction(self, (void*)FUN_003e9be4);
    }
}

static __inline __pure float EnIk_Fabsf(float x) {
    return __fabsf(x);
}

void FUN_00136fc8(EnIk* self, PlayState* play) {
    s16 angle = (s16)(*(u16*)((u8*)self + 0xE18) + 0x12F);
    f32 sinVal;
    f32 frame;

    *(s16*)((u8*)self + 0xE18) = angle;
    sinVal = Math_SinS(angle);
    {
        f32* scale = (f32*)((u8*)self + 0x1D4);

        if (sinVal >= 0.0f) {
            goto absDone;
        }
        sinVal = -sinVal;
absDone:
        scale[4] = sinVal;
    }
    frame = *(f32*)((u8*)self + 0x1E0);
    if (frame > 16.0f) {
        self->unk_E13 = 3;
    }
    if (frame > 6.0f) {
        if (frame < 14.0f) {
            goto window;
        }
    }
    if (frame <= 18.0f) {
        goto outside;
    }
    if (frame >= 23.0f) {
        goto outside;
    }
window:
    if (*((u8*)self + 0xE10) == 0 && self->unk_E0F != 0 && frame < 15.0f) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 0x5DC, 0);
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }
    if (*(s8*)((u8*)self + 0xE12) < 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000399);
    }
    self->unk_E12 = 1;
    goto after;
outside:
    self->unk_E12 = 0;
after:
    if (FUN_00370734((SkelAnime*)self->unk_1A4)) {
        f32 endFrame = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 4);

        self->unk_E12 = 0;
        self->unk_E13 = 0;
        self->unk_E0C = 8;
        Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 4, 1.5f, 0.0f, endFrame, 3, -4.0f);
        self->actionFunc = (void*)FUN_001c67f4;
    }
}

void FUN_0011c7e0(EnIk* self, PlayState* play);

// N64 EnIk_StopAndBlock. `play2` param + `play` alias moves the 6 (E0C) into the play reg after the flag load.
static inline f32 EnIk_AbsF(f32 x) {
    if (x >= 0.0f) {
        return x;
    }
    return -x;
}

void FUN_0013832c(EnIk* self, PlayState* play2) {
    PlayState* play = play2;

    CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)self->shieldCollider);
    if (FUN_003731e0((SkelAnime*)self->unk_1A4) != 0) {
        s16 yawDiff = (s16)((u16)self->actor.yawTowardsPlayer - (u16)self->actor.shape.rot.y);

        if (((yawDiff >= 0) ? yawDiff : -yawDiff) <= 0x4000 && self->actor.xzDistToPlayer < 100.0f &&
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
        } else {
            f32 frames = (f32)Animation_GetLastFrame((SkelAnime*)self->unk_1A4, 0xC);

            self->actor.flags |= 5;
            self->actor.speedXZ = 0.0f;
            self->unk_E0C = 4;
            Animation_Change_00375c08((SkelAnime*)self->unk_1A4, 0xC, 0.0f, 0.0f, frames, 0, 4.0f);
            self->actionFunc = (void*)FUN_0010b758;
        }
    }
}

}
