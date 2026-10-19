extern "C" {
#include "z_en_cow.hpp"

#define FLAGS 0x00000009

void EnCow_Init(Actor* actor, GameState* state);
void EnCow_Destroy(Actor* actor, PlayState* play);
void Collider_DestroyJntSph(PlayState* play, void* collider);
void EnCow_Update(Actor* actor, GameState* state);
void EnCow_Draw(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_En_Cow.data.s")
GLOBAL_ASM("../src/game/actors/En_Cow/data_syms.s")

extern ColliderCylinderInit EnCow_sCylinderInit;
extern char EnCow_sJntSphInit[];
extern char EnCow_sBodyChewAnim[];
extern char EnCow_sTailIdleAnim[];

void ActorShape_Init(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
void FUN_00353e78(ZARInfo* zar, PlayState* play, SkelAnime* skel, void* cmb, void* arg4, s32 arg5, Vec3s* jointTable,
                  Vec3s* morphTable, s32 limbCount);
void FUN_003490e0(SkelAnime* skelAnime, void* anim); // Animation_PlayLoop
void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
void FUN_0036932c(void* model, s32 arg);
void FUN_00249044(Actor* actor, PlayState* play);
void FUN_00249048(EnCow* self, PlayState* play);

s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, s32 mode, f32 speed, f32 start, f32 end, f32 morph);
s32 FUN_00370734(SkelAnime* skelAnime);
void FUN_0033ddbc(Vec3f* dst, Vec3f* src);
void FUN_0014ff48(PlayState* play, s32 limbIndex, MTX34* mtx, EnCow* self);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
s32 FUN_0016d584(PlayState* play, s32 limbIndex, MTX34* mtx, EnCow* self);
void FUN_00314cfc(EnCow* self, PlayState* play);
void FUN_003725e0(PlayState* play);
s32 FUN_00369a48(Actor* actor, PlayState* play);
s32 Inventory_HasEmptyBottle(void);
struct PS { char pad[0x2B7E]; u16 ocarinaMode; };
void FUN_003f1244(EnCow* self, PlayState* play);
void FUN_003f062c(EnCow* self, PlayState* play);
void FUN_0010f308(EnCow* self, PlayState* play);
void FUN_003ee340(EnCow* self, PlayState* play);
void FUN_00183a84(EnCow* self, PlayState* play);
void FUN_0015d0fc(EnCow* self, PlayState* play);
void FUN_003ebed4(EnCow* self, PlayState* play);


ActorInit En_Cow_InitVars = {
    ACTOR_EN_COW,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_COW,
    sizeof(EnCow),
    (ActorFunc)EnCow_Init,
    (ActorFunc)EnCow_Destroy,
    (ActorFunc)EnCow_Update,
    (ActorFunc)EnCow_Draw,
};


void EnCow_Destroy(Actor* thisx, PlayState* play) {
    EnCow* self = (EnCow*)thisx;
    s16 type = thisx->params;

    if ((type == 0) || (type == 2)) {
        Collider_DestroyCylinder(play, (ColliderCylinder*)((u8*)self + 0x1A4));
        Collider_DestroyCylinder(play, (ColliderCylinder*)((u8*)self + 0x1FC));
        Collider_DestroyJntSph(play, (u8*)self + 0x254);
    }
    SkelAnime_Free2((SkelAnime*)((u8*)self + 0x314));
}




void EnCow_Draw(Actor* thisx, PlayState* play) {
    EnCow* self = (EnCow*)thisx;
    Actor* child = self->actor.child;

    if (child != NULL) {
        { Vec3f* dst = (Vec3f*)((u8*)self + 0x49C); FUN_0033ddbc(dst, (Vec3f*)((u8*)child + 0x49C)); }
        { Vec3f* dst = (Vec3f*)((u8*)self + 0x4D0); FUN_0033ddbc(dst, (Vec3f*)((u8*)child + 0x4D0)); }
        { Vec3f* dst = (Vec3f*)((u8*)self + 0x504); FUN_0033ddbc(dst, (Vec3f*)((u8*)child + 0x504)); }
        { Vec3f* dst = (Vec3f*)((u8*)self + 0x538); FUN_0033ddbc(dst, (Vec3f*)((u8*)child + 0x538)); }
        { Vec3f* dst = (Vec3f*)((u8*)self + 0x56C); FUN_0033ddbc(dst, (Vec3f*)((u8*)child + 0x56C)); }
    }
    Actor_SetScale(thisx, 0.01f);
    SkelAnime_DrawOpa((SkelAnime*)((u8*)self + 0x314), &self->actor.modelMtx, (OverrideLimbDrawOpa)FUN_0016d584,
                      (PostLimbDrawOpa)FUN_0014ff48, thisx, 0);
}

#if 0 // draft: 9-float copy, struct copies give 32 B (ROM 24 B vldm), see STATUS.md
void FUN_0033ddbc(Vec3f* dst, Vec3f* src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}
#endif
GLOBAL_ASM("asm/FUN_0033ddbc.s")

void FUN_00249044(Actor* actor, PlayState* play) {
}

void FUN_003ebed4(EnCow* self, PlayState* play) {
    if (Actor_ProcessTalkRequest(&self->actor, play)) {
        self->actionFunc = FUN_003f062c;
    } else {
        self->actor.flags |= 0x10000;
        FUN_0036bb28(&self->actor, play, 170.0f);
        self->actor.textId = 0x2006;
    }
    FUN_00314cfc(self, play);
}

void FUN_003f062c(EnCow* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        if (Inventory_HasEmptyBottle()) {
            Message_ContinueTextbox(play, 0x2007);
            self->actionFunc = FUN_0010f308;
        } else {
            Message_ContinueTextbox(play, 0x2013);
            self->actionFunc = FUN_0015d0fc;
        }
    }
}

void FUN_0010f308(EnCow* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        self->actor.flags &= ~0x10000;
        FUN_003725e0(play);
        self->actionFunc = FUN_003ee340;
        GiveItem(&self->actor, play, GI_MILK, 100000.0f, 1100.0f);
    }
}

void FUN_003ee340(EnCow* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = FUN_00183a84;
    } else {
        GiveItem(&self->actor, play, GI_MILK, 100000.0f, 1100.0f);
    }
}

#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

typedef struct EnCowTailState {
    /* 0x00 */ char pad_00[0xA8];
    /* 0xA8 */ s16 headRotX;
    /* 0xAA */ s16 headRotY;
    /* 0xAC */ char pad_AC[0x2];
    /* 0xAE */ u16 flags;
    /* 0xB0 */ u16 timer;
    /* 0xB2 */ u16 breathTimer;
} EnCowTailState; // this + 0x700

f32 sinf(f32 x);
f32 cosf(f32 x);

static inline void EnCow_MtxRotX(MTX34* m, f32 angle) {
    f32 s;
    f32 c;
    f32 y;
    f32 z;

    if (angle != 0.0f) {
        s = sinf(angle);
        c = cosf(angle);
        y = m->data[0][1];
        z = m->data[0][2];
        m->data[0][1] = (y * c) + (z * s);
        m->data[0][2] = (z * c) - (y * s);
        y = m->data[1][1];
        z = m->data[1][2];
        m->data[1][1] = (y * c) + (z * s);
        m->data[1][2] = (z * c) - (y * s);
        y = m->data[2][1];
        z = m->data[2][2];
        m->data[2][1] = (y * c) + (z * s);
        m->data[2][2] = (z * c) - (y * s);
    }
}

static inline void EnCow_MtxRotY(MTX34* m, f32 angle) {
    f32 s;
    f32 c;
    f32 x;
    f32 z;

    if (angle != 0.0f) {
        s = sinf(angle);
        c = cosf(angle);
        x = m->data[0][0];
        z = m->data[0][2];
        m->data[0][0] = (x * c) - (z * s);
        m->data[0][2] = (x * s) + (z * c);
        x = m->data[1][0];
        z = m->data[1][2];
        m->data[1][0] = (x * c) - (z * s);
        m->data[1][2] = (x * s) + (z * c);
        x = m->data[2][0];
        z = m->data[2][2];
        m->data[2][0] = (x * c) - (z * s);
        m->data[2][2] = (x * s) + (z * c);
    }
}

s32 FUN_0016d584(PlayState* play, s32 limbIndex, MTX34* mtx, EnCow* self) {
    if (limbIndex == 1) {
        EnCowTailState* st = (EnCowTailState*)((u8*)self + 0x700);

        EnCow_MtxRotX(mtx, st->headRotX * 0.000095873802f);
        EnCow_MtxRotY(mtx, st->headRotY * 0.000095873802f);
    }
    return 0;
}

// PostLimbDraw: guarded function-local static pinned by pin_static.py (guard 0x521A1C, object 0x598FCC)
// N64 Matrix_MultVec3f shape: (src, dest) order drives the arg scheduling of the tail call
static inline void EnCow_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void FUN_0014ff48(PlayState* play, s32 limbIndex, MTX34* mtx, EnCow* self) {
    static Vector3f sHeadFocusOffset(0.0f, 0.0f, 600.0f);

    if (limbIndex == 2) {
        EnCow_MultVec3f(&sHeadFocusOffset, (Vector3f*)&self->actor.focus.pos, mtx);
    } else if ((limbIndex == 1) || (limbIndex == 3)) {
        Collider_UpdateSpheres(limbIndex, (u8*)self + 0x254, mtx);
    }
}


void FUN_003f0504(EnCow* self, PlayState* play) {
    EnCowTailState* st = (EnCowTailState*)((u8*)self + 0x700);
    Actor* thisx = &self->actor;

    if (st->timer != 0) {
        st->timer--;
    } else {
        st->timer = (s16)FRAMES((u16)(Rand_ZeroFloat(200.0f) + 40.0f));
        Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x314), 2, 2, 1.0f, *(f32*)((u8*)self + 0x350), (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x314), 2), 1.0f);
    }

    if ((self->actor.xzDistToPlayer < 150.0f) &&
        (ABS((s16)(thisx->yawTowardsPlayer - thisx->shape.rot.y)) > 25000)) {
        if (!(st->flags & 2)) {
            st->flags |= 2;
            if (*(s32*)((u8*)self + 0x344) == 2) {
                st->timer = 0;
            }
        }
    }
}

static inline void EnCow_RotateY(Vec3f* vec, s16 rotY) {
    f32 x = vec->x;
    f32 z = vec->z;
    f32 xCalc = (Math_CosS(rotY) * x) + (Math_SinS(rotY) * z);
    f32 zCalc = (Math_CosS(rotY) * z) - (Math_SinS(rotY) * x);
    vec->z = zCalc;
    vec->x = xCalc;
}
void FUN_00349008(EnCow* self) {
    Vec3f vec;

    vec.y = 0.0f;
    vec.x = 0.0f;
    vec.z = 30.0f;

    EnCow_RotateY(&vec, self->actor.shape.rot.y);

    *(f32*)((u8*)self + 0x1F0) = self->actor.world.pos.x + vec.x;
    *(f32*)((u8*)self + 0x1F4) = self->actor.world.pos.y;
    *(f32*)((u8*)self + 0x1F8) = self->actor.world.pos.z + vec.z;

    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = -20.0f;

    EnCow_RotateY(&vec, self->actor.shape.rot.y);

    *(f32*)((u8*)self + 0x248) = self->actor.world.pos.x + vec.x;
    *(f32*)((u8*)self + 0x24C) = self->actor.world.pos.y;
    *(f32*)((u8*)self + 0x250) = self->actor.world.pos.z + vec.z;
}

void FUN_00314cfc(EnCow* self, PlayState* play) {
    EnCowTailState* st = (EnCowTailState*)((u8*)self + 0x700);

    if (st->timer != 0) {
        st->timer--;
    } else {
        st->timer = (s16)FRAMES((u16)(Rand_ZeroFloat(500.0f) + 40.0f));
        Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x314), 0, 2, 1.0f, *(f32*)((u8*)self + 0x350),
                                  (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x314), 0), 1.0f);
    }

    if ((self->actor.xzDistToPlayer < 150.0f) && !(st->flags & 2)) {
        st->flags |= 2;
        if (*(s32*)((u8*)self + 0x344) == 0) {
            st->timer = 0;
        }
    }

    st->breathTimer++;
    if (st->breathTimer > 48) {
        st->breathTimer = 0;
    }
    if (st->breathTimer < 32) {
        self->actor.scale.x = ((Math_SinS(st->breathTimer * 0x0400) * (1.0f / 100.0f)) + 1.0f) * 0.01f;
    } else {
        self->actor.scale.x = 0.01f;
    }
    if (st->breathTimer > 16) {
        self->actor.scale.y = ((Math_SinS((s16)((st->breathTimer - 16) * 0x0400)) * (1.0f / 100.0f)) + 1.0f) * 0.01f;
    } else {
        self->actor.scale.y = 0.01f;
    }
}

#if 0 // draft, 53 words off: ROM reads play+0x2B7E via `ldr r0,=0x2b7e; ldrh r0,[r0,r1]` (pool) but armcc emits add #0x2b00/ldrh #0x7e (~20 variants: raw cast, u16*, struct view, index form, thisx, flags hoist); rest of the body untested because of the shift
void FUN_003f1244(EnCow* self, PlayState* play) {
    EnCowTailState* st = (EnCowTailState*)((u8*)self + 0x700);
    u16 ocarinaMode = *(u16*)((u8*)play + 0x2B7E);

    if ((ocarinaMode == 0) || (ocarinaMode == 4)) {
        if (DREG(53) != 0) {
            if (st->flags & 4) {
                st->flags &= ~4;
                DREG(53) = 0;
            } else if ((self->actor.xzDistToPlayer < 150.0f) &&
                       (ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) < 25000)) {
                DREG(53) = 0;
                self->actionFunc = FUN_003ebed4;
                self->actor.flags |= 0x10000;
                FUN_0036bb28(&self->actor, play, 170.0f);
                self->actor.textId = 0x2006;
            } else {
                st->flags |= 4;
            }
        } else {
            st->flags &= ~4;
        }
    }
    FUN_00314cfc(self, play);
}
#endif

static inline s16 EnCow_Pitch(Vec3f* a, Vec3f* b) {
    f32 by = b->y;
    f32 ay = a->y;
    f32 dx = b->x - a->x;
    f32 dz = b->z - a->z;
    return Math_Atan2S(__sqrtf(SQ(dx) + SQ(dz)), ay - by);
}

void EnCow_Update(Actor* actor, GameState* state) {
    EnCow* self = (EnCow*)actor;
    PlayState* play = (PlayState*)state;
    Player* player = GET_PLAYER(play);
    s16 targetX;
    s16 targetY;
    EnCow* s2 = self;

    FUN_00349008(self);
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)((u8*)self + 0x1A4));
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)((u8*)self + 0x1FC));
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)((u8*)self + 0x254));
    Actor_MoveForward(actor);
    Actor_UpdateBgCheckInfo(play, actor, 0.0f, 0.0f, 0.0f, 4);

    if (FUN_00370734((SkelAnime*)((u8*)self + 0x314))) {
        if (*(s32*)((u8*)self + 0x344) == 0) {
            Audio_PlayActorSound2(actor, 0x01000236);
            Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x314), 1, 2, 1.0f, 0.0f,
                                      (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x314), 1), 1.0f);
        } else {
            Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x314), 0, 0, 1.0f, 0.0f,
                                      (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x314), 0), 1.0f);
        }
    }

    s2->actionFunc(self, play);

    if ((actor->xzDistToPlayer < 150.0f) &&
        (ABS(Math_Vec3f_Yaw(&actor->world.pos, &player->actor.world.pos)) < 0xC000)) {
        targetX = EnCow_Pitch(&actor->focus.pos, &player->actor.focus.pos);
        targetY = Math_Vec3f_Yaw(&actor->focus.pos, &player->actor.focus.pos) - actor->shape.rot.y;

        if (targetX > 0x1000) {
            targetX = 0x1000;
        } else if (targetX < -0x1000) {
            targetX = -0x1000;
        }

        if (targetY > 0x2500) {
            targetY = 0x2500;
        } else if (targetY < -0x2500) {
            targetY = -0x2500;
        }
    } else {
        targetY = 0;
        targetX = 0;
    }

    Math_SmoothStepToS((s16*)((u8*)self + 0x7A8), targetX, 10, 200, 10);
    Math_SmoothStepToS((s16*)((u8*)self + 0x7AA), targetY, 10, 200, 10);

    if (actor->params == 2) {
        if (Flags_GetSwitch(play, 0x3E)) {
            if (self->timerA != 0) {
                self->timerA--;
            }
        }
        if (self->timerA == 1) {
            Actor_Spawn(&play->actorCtx, play, 0x12D, 1455.0f, -160.0f, -1156.0f, 0, 0, 0, 0x3DC1, 1);
            Actor_Spawn(&play->actorCtx, play, 0x112, 1451.0f, -136.0f, -1233.0f, 0, 0, 4, 0x1AFD, 1);
        }

        if (Flags_GetSwitch(play, 0x3D)) {
            if (self->timerB != 0) {
                self->timerB--;
            }
        }
        if (self->timerB == 1) {
            Actor_Spawn(&play->actorCtx, play, 0x12A, 1456.0f, -60.0f, -1188.0f, 0, 0, 0, 0x1403, 1);
        }
    }
}

static inline void EnCow_SetTailPos(EnCow* self) {
    Vec3f vec;

    vec.x = 0.0f;
    vec.y = 57.0f;
    vec.z = -36.0f;
    EnCow_RotateY(&vec, self->actor.shape.rot.y);

    self->actor.world.pos.x += vec.x;
    self->actor.world.pos.y += vec.y;
    self->actor.world.pos.z += vec.z;
}

#if 0 // draft left mid-work by the G40 Sonnet pass (weekly limit hit); does not match yet
void EnCow_Init(Actor* actor, GameState* state) {
    EnCow* self = (EnCow*)actor;
    ZARInfo* zar;
    void* cmb;
    PlayState* play = (PlayState*)state;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 72.0f);
    zar = &play->GetObjectStatus(actor->objBankIndex)->zarInfo;


    EnCowTailState* st = (EnCowTailState*)((u8*)self + 0x700);
    switch (actor->params) {
        case 0:
        case 2:
            cmb = ZAR_GetCMBByIndex(zar, 0);
            FUN_00353e78(zar, play, (SkelAnime*)((u8*)self + 0x314), cmb, actor->unk_178, -1,
                         (Vec3s*)((u8*)self + 0x398), (Vec3s*)((u8*)self + 0x5A0), 10);
            FUN_003490e0((SkelAnime*)((u8*)self + 0x314), EnCow_sBodyChewAnim);

            Collider_InitCylinder(play, (ColliderCylinder*)((u8*)self + 0x1A4));
            Collider_SetCylinder(play, (ColliderCylinder*)((u8*)self + 0x1A4), actor, &EnCow_sCylinderInit);

            Collider_InitCylinder(play, (ColliderCylinder*)((u8*)self + 0x1FC));
            Collider_SetCylinder(play, (ColliderCylinder*)((u8*)self + 0x1FC), actor, &EnCow_sCylinderInit);

            Collider_InitJntSph(play, (u8*)self + 0x254);
            Collider_SetJntSph(play, (u8*)self + 0x254, actor, EnCow_sJntSphInit, (u8*)self + 0x274);

            FUN_00349008(self);

            self->actionFunc = FUN_003f1244;

            if (play->sceneNum == 0x34) {
                if ((gSaveContext.linkAge != 0) || !(gSaveContext.eventChkInf[1] & 0x4000)) {
                    Actor_Kill(actor);
                    return;
                }
                FUN_0036932c(*(void**)((u8*)self + 0x33C), 1);
            }

            Actor_SpawnAsChild(&play->actorCtx, actor, play, 0x1C6, actor->world.pos.x, actor->world.pos.y,
                               actor->world.pos.z, 0, actor->shape.rot.y, 0, 1);
            st->timer = (s16)FRAMES((u16)(Rand_ZeroFloat(1000.0f) + 40.0f));
            st->breathTimer = 0;
            actor->targetMode = 6;
            DREG(53) = 0;
            break;

        case 1:
            cmb = ZAR_GetCMBByIndex(zar, 1);
            FUN_00353e78(zar, play, (SkelAnime*)((u8*)self + 0x314), cmb, actor->unk_178, -1,
                         (Vec3s*)((u8*)self + 0x398), (Vec3s*)((u8*)self + 0x5A0), 10);
            FUN_003490e0((SkelAnime*)((u8*)self + 0x314), EnCow_sTailIdleAnim);
            actor->update = (ActorFunc)FUN_00249048;
            actor->draw = (ActorFunc)FUN_00249044;
            self->actionFunc = FUN_003f0504;
            EnCow_SetTailPos(self);
            actor->flags &= ~1;
            st->timer = (s16)FRAMES((u16)Rand_ZeroFloat(1000.0f) + 40.0f);
            (*(SkeletonAnimationModel**)((u8*)self + 0x33C))->unk_AD[0] = 0;
            break;
    }

    actor->colChkInfo.mass = 0xFF;
    if (actor->params == 2) {
        self->timerA = 90;
        self->timerB = 90;
    }
    Actor_SetScale(actor, 0.01f);
    st->flags = 0;
}
#endif
GLOBAL_ASM("asm/EnCow_Init.s")

void FUN_00249048(EnCow* self, PlayState* play) {
    if (FUN_00370734((SkelAnime*)((u8*)self + 0x314))) {
        if (*(s32*)((u8*)self + 0x344) == 2) {
            Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x314), 3, 2, 1.0f, 0.0f, (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x314), 3), 1.0f);
        } else {
            Animation_Change_00375c08((SkelAnime*)((u8*)self + 0x314), 2, 0, 1.0f, 0.0f, (f32)Animation_GetLastFrame((SkelAnime*)((u8*)self + 0x314), 2), 1.0f);
        }
    }
    self->actionFunc(self, play);
}

void FUN_00183a84(EnCow* self, PlayState* play) {
    if (FUN_00369a48(&self->actor, play)) {
        self->actor.flags &= ~0x10000;
        self->actionFunc = FUN_003f1244;
    }
}

void FUN_0015d0fc(EnCow* self, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        self->actor.flags &= ~0x10000;
        FUN_003725e0(play);
        self->actionFunc = FUN_003f1244;
    }
}


}
