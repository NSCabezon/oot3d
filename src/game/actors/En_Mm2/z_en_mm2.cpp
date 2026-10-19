extern "C" {
#include "z_en_mm2.hpp"

#define FLAGS 0x00000019

typedef enum RunningManAnimIndex {
    /* 0 */ RM2_ANIM_RUN,
    /* 1 */ RM2_ANIM_SIT,
    /* 2 */ RM2_ANIM_SIT_WAIT,
    /* 3 */ RM2_ANIM_STAND,
    /* 4 */ RM2_ANIM_SPRINT,
    /* 5 */ RM2_ANIM_EXCITED,
    /* 6 */ RM2_ANIM_HAPPY
} RunningManAnimIndex;

typedef enum RunningManMouthTex {
    /* 0 */ RM2_MOUTH_CLOSED,
    /* 1 */ RM2_MOUTH_OPEN
} RunningManMouthTex;

typedef struct EnMm2AnimInfo {
    /* 0x00 */ void* animation;
    /* 0x04 */ f32 playSpeed;
    /* 0x08 */ u8 mode;
    /* 0x0C */ f32 morphFrames;
} EnMm2AnimInfo; // size = 0x10

// Data symbols (ROM .data blob pinned by data/z_En_Mm2.data.s, EQU in syms.s).
extern u8 mm2CylinderInit[];
extern u8 mm2AnimInfo[];
#define sAnimationInfo ((EnMm2AnimInfo*)mm2AnimInfo)

void EnMm2_Init(Actor* thisx, PlayState* play);
void EnMm2_Destroy(EnMm2* self, PlayState* play);
void EnMm2_Update(Actor* thisx, PlayState* play);
void EnMm2_Draw(Actor* thisx, PlayState* play);
s32 EnMm2_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void EnMm2_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);

// Unknown callees (3DS addresses), local prototypes.
void FUN_003539d8(EnMm2* self, PlayState* play); // N64: func_80AAEF70 (text id selection)
void FUN_001571d8(EnMm2* self, PlayState* play); // N64: func_80AAF57C (idle, talk)
void FUN_001788b8(EnMm2* self, PlayState* play); // N64: func_80AAF668 (marathon start)
void FUN_003cdc40(EnMm2* self, PlayState* play); // N64: func_80AAF3C0 (talk choices)
void FUN_001570ac(EnMm2* self, PlayState* play); // N64: func_80AAF2BC
void FUN_001e94b4(EnMm2* self, PlayState* play); // N64: func_80AAF330
void FUN_003f93e4(EnMm2* self, PlayState* play); // N64: func_80AAF5EC
void FUN_0034bf5c(EnMm2* self, s32 animIndex, s32* previousAnimation); // N64: EnMm2_ChangeAnim
s32 FUN_00369a48(Actor* actor, PlayState* play); // N64: Actor_TextboxIsClosing
void FUN_003725e0(PlayState* play);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);              // N64: Message_CloseTextbox
void FUN_00353c9c(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex,
                  void* jointTable, void* morphTable, s32 limbCount);
void Animation_Change_00375c08(SkelAnime* skelAnime, void* anim, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, void* anim);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f lookAtPos, s16 yawLimit);

ActorInit En_Mm2_InitVars = {
    ACTOR_EN_MM2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MM,
    sizeof(EnMm2),
    (ActorFunc)EnMm2_Init,
    (ActorFunc)EnMm2_Destroy,
    (ActorFunc)EnMm2_Update,
    (ActorFunc)EnMm2_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_STOP),
};

GLOBAL_ASM("data/z_En_Mm2.data.s")
GLOBAL_ASM("../src/game/actors/En_Mm2/syms.s")

// N64: func_80AAF224 (inlined)
static inline s32 EnMm2_TalkOrOffer(EnMm2* self, PlayState* play, EnMm2ActionFunc actionFunc) {
    Actor* actor = &self->actor;

    if (Actor_ProcessTalkRequest(actor, play)) {
        self->actionFunc = actionFunc;
        return 1;
    }
    s16 d = actor->yawTowardsPlayer - actor->shape.rot.y;
    if (ABS(d) <= 0x4300 && actor->xzDistToPlayer < 100.0f) {
        FUN_0036bb28(actor, play, 100.0f);
    }
    return 0;
}

// N64: func_80AAF2BC. Moved from functions_140000s.cpp.
void FUN_001570ac(EnMm2* self, PlayState* play) {
    if (self->unk_8B2 > 90) {
        Actor_Kill(&self->actor);
    }
    FUN_003731e0(&self->skelAnime);
    self->unk_8B2++;
    Math_SmoothStepToF(&self->actor.speedXZ, 10.0f, 0.6f, 2.0f, 0.0f);
}

// N64: func_80AAF330. Moved from functions_1C0000s.cpp.
void FUN_001e94b4(EnMm2* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        self->actionFunc = FUN_001570ac;
        FUN_0034bf5c(self, RM2_ANIM_RUN, &self->previousAnimation);
        self->mouthTexIndex = RM2_MOUTH_OPEN;
        if (!(self->unk_8B0 & 2)) {
            FUN_003725e0(play);
        }
        gSaveContext.timer2State = 0;
        gSaveContext.eventInf[1] &= ~1;
    }
}

// N64: func_80AAF5EC. Moved from functions_3E0000s.cpp.
void FUN_003f93e4(EnMm2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
        self->unk_8B0 &= ~1;
        FUN_0034bf5c(self, RM2_ANIM_STAND, &self->previousAnimation);
        self->actionFunc = FUN_001e94b4;
    }
}

// N64: func_80AAF3C0. Moved from functions_3C0000s.cpp.
void FUN_003cdc40(EnMm2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);

    switch (self->actor.textId) {
        case 0x607D:
        case 0x607E:
            if ((Message_GetState(&play->msgCtx) == 4) && Message_ShouldAdvance(play)) {
                switch (FUN_00369f3c(play)) {
                    case 0:
                        Message_ContinueTextbox(play, 0x607F);
                        self->actor.textId = 0x607F;
                        gSaveContext.eventInf[1] |= 1;
                        break;
                    case 1:
                        Message_ContinueTextbox(play, 0x6080);
                        self->actor.textId = 0x6080;
                        break;
                }

                if (self->unk_8B0 & 4) {
                    self->unk_8B0 &= ~4;
                    gSaveContext.unk_EE0++;
                }
            }
            break;

        case 0x6081:
            if ((Message_GetState(&play->msgCtx) == 5) && Message_ShouldAdvance(play)) {
                self->unk_8B0 |= 4;
                gSaveContext.unk_EE0--;
                Message_ContinueTextbox(play, 0x607E);
                self->actor.textId = 0x607E;
            }
            break;

        default:
            if (FUN_00369a48(&self->actor, play)) {
                switch (self->actor.textId) {
                    case 0x607F:
                        FUN_00371e6c(0);
                        self->actionFunc = FUN_001571d8;
                        break;
                    default:
                        self->actionFunc = FUN_001571d8;
                        break;
                }
                self->actionFunc = FUN_001571d8;
                FUN_003539d8(self, play);
            }
            break;
    }
}

// N64: func_80AAF57C. NOT matching (3 words: ROM loads yawTowardsPlayer into r1 / rot.y into r0, we swap them;
// ~25 variants tried). Kept as GLOBAL_ASM in functions_140000s.cpp.
#if 0
// N64: func_80AAF57C
void FUN_001571d8(EnMm2* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);
    FUN_003539d8(self, play);
    if (EnMm2_TalkOrOffer(self, play, FUN_003cdc40) && (self->actor.textId == 0x607D)) {
        gSaveContext.infTable[23] |= 0x8000;
    }
}
#endif

// N64: func_80AAF668
void FUN_001788b8(EnMm2* self, PlayState* play) {
    self->actor.world.rot.y = -0x3E80;
    self->actor.shape.rot.y = self->actor.world.rot.y;
    FUN_003731e0(&self->skelAnime);
    if (gSaveContext.timer2Value < (s32)gSaveContext.unk_EE0) {
        self->actor.textId = 0x6085;
    } else {
        self->actor.textId = 0x6084;
    }
    if (EnMm2_TalkOrOffer(self, play, FUN_003f93e4)) {
        self->unk_8B2 = 0;
        {
            s32 t = gSaveContext.timer2Value;
            s32 h = gSaveContext.unk_EE0;
            if (t > h) {
                t = h;
            }
            gSaveContext.unk_EE0 = t;
        }
    } else {
        if (!(gSaveContext.eventInf[1] & 1)) {
            self->unk_8B0 |= 2;
            self->unk_8B0 &= ~1;
            FUN_0034bf5c(self, RM2_ANIM_STAND, &self->previousAnimation);
            self->actionFunc = FUN_001e94b4;
        }
    }
}

void EnMm2_Init(Actor* thisx, PlayState* play) {
    EnMm2* self = (EnMm2*)thisx;


    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 21.0f);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel, 0, 0);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 1, self->jointTable, self->morphTable, 15);
    Animation_Change_00375c08(&self->skelAnime, sAnimationInfo[RM2_ANIM_SIT_WAIT].animation, 1.0f, 0.0f,
                              Animation_GetLastFrame(&self->skelAnime, sAnimationInfo[RM2_ANIM_SIT_WAIT].animation),
                              sAnimationInfo[RM2_ANIM_SIT_WAIT].mode, sAnimationInfo[RM2_ANIM_SIT_WAIT].morphFrames);
    self->previousAnimation = RM2_ANIM_SIT_WAIT;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, (ColliderCylinderInit*)mm2CylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.gravity = -1.0f;
    self->mouthTexIndex = RM2_MOUTH_CLOSED;
    self->actor.targetMode = 6; // ATTENTION_RANGE_6
    self->unk_8B0 |= 1;
    if (self->actor.params == 1) {
        ((EnMm2*)thisx)->actionFunc = FUN_001788b8;
    } else {
        FUN_003539d8(self, play);
        ((EnMm2*)thisx)->actionFunc = FUN_001571d8;
    }
    if (!LINK_IS_ADULT) {
        Actor_Kill(&self->actor);
    }
    if (self->actor.params == 1) {
        if (!(gSaveContext.infTable[23] & 0x8000) || !(gSaveContext.eventInf[1] & 1)) {
            Actor_Kill(&self->actor);
        }
    }
}



void EnMm2_Destroy(EnMm2* self, PlayState* play) {
    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(&self->actor, &self->skelAnimModel, 0);
    SkelAnime_Free2(&self->skelAnime);
    self->faceAnim.Destroy();
}

void EnMm2_Update(Actor* thisx, PlayState* play) {
    EnMm2* self = (EnMm2*)thisx;

    if (self->unk_8B0 & 1) {
        FUN_0036bcc8(play, &self->actor, &self->unk_8A4, &self->unk_8AA, self->actor.focus.pos, 0x4300);
    } else {
        Math_SmoothStepToS(&self->unk_8A4.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_8A4.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_8AA.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_8AA.y, 0, 6, 6200, 100);
    }
    self->actionFunc(self, play);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
}

void EnMm2_Draw(Actor* thisx, PlayState* play) {
    EnMm2* self = (EnMm2*)thisx;

    FaceAnimations* fa = &self->faceAnim;
    s32 mouth = self->mouthTexIndex;
    fa->FUN_0035e3a4(1, mouth);
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnMm2_OverrideLimbDraw, EnMm2_PostLimbDraw, thisx, 0);
    self->faceAnim.FUN_0035e330();
}

s32 EnMm2_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    EnMm2* self = (EnMm2*)arg;

    if (limbIndex == 7) {
        FUN_0034e01c(mtx, &self->unk_8AA);
    } else if (limbIndex == 14) {
        FUN_0034e01c(mtx, &self->unk_8A4);
    }

    return 0;
}

// N64-order wrapper (Matrix_MultVec3f(src, dest)).
static inline void MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

// Vec3f with a Set()-based ctor: gives the ROM's x->s0, y->s1, z->s2 literal registers in the guarded init.
struct EnMm2Vec3f {
    f32 x, y, z;
    EnMm2Vec3f(f32 a, f32 b, f32 c) {
        Set(a, b, c);
    }
    void Set(f32 a, f32 b, f32 c) {
        x = a;
        y = b;
        z = c;
    }
};

// Function-local static: guard 0x0052C874 shares the TU .data section with sInitChain (ROM addresses it as
// sInitChain+4); both cut from the data chunk. Object 0x0059B484 pinned in data/oot3d.ld (scripts/pin_static.py).
void EnMm2_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnMm2* self = (EnMm2*)thisx;
    static EnMm2Vec3f sMultVec(200.0f, 800.0f, 0.0f);

    if (limbIndex == 14) {
        MultVec3f((Vector3f*)&sMultVec, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

}
