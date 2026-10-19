extern "C" {
#include "z_en_cs.hpp"
#include "mtx34_identity.hpp"

#define FLAGS 0x00000009
#define RAD_TO_BINANG(rad) ((s16)((rad) * 10430.378f))

void EnCs_Init(Actor* actor, PlayState* play);
void EnCs_Destroy(Actor* actor, PlayState* play);
void EnCs_Update(Actor* actor, PlayState* play);
void EnCs_Draw(Actor* actor, GameState* state);

void FUN_0035021c(void* model);
s32 FUN_00370734(SkelAnime* skelAnime);
void EnCs_Talk(EnCs* self, PlayState* play);
s32 EnCs_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnCs_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnCs_Walk(EnCs* self, PlayState* play);
void EnCs_Wait(EnCs* self, PlayState* play);
f32* EnCs_CurFramePtr(SkelAnime* skelAnime); // own TU (z_en_cs_anim.cpp), inlined by armlink
#define EN_CS_CURFRAME() ((s32)*EnCs_CurFramePtr(&self->skelAnime))
void EnCs_SetAnimFromIndex(EnCs* self, s32 animIndex, s32* currentAnimIndex);
void FUN_0034c664(Actor* actor, void* interactInfo, s32 arg3, s32 arg4);
void FUN_00353020(SkelAnime* skelAnime, void* animation, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);

typedef struct {
    /* 0x0 */ u8 count;
    /* 0x4 */ Vec3s* points;
} EnCsPath; // size = 0x8

typedef struct {
    /* 0x0 */ void* animation;
    /* 0x4 */ f32 frameCount;
    /* 0x8 */ u8 mode;
    /* 0xC */ f32 morphFrames;
} EnCsAnimInfo; // size = 0x10

extern EnCsAnimInfo EnCs_sAnimationInfo[];
extern ColliderCylinderInit EnCs_sCylinderInit;
extern CollisionCheckInfoInit2 EnCs_sColChkInfoInit2;
extern DamageTable EnCs_sDamageTable;
extern s32 EnCs_sEyeBlinkFrames[];
void FUN_00376a60(s16 amount);
void FUN_00345fcc(PlayState* play);
void FUN_00376a78(PlayState* play, s32 itemId);
SkeletonAnimationModel* FUN_0036a924(Actor* actor, PlayState* play, s16 objectSlot, s32 arg3);

GLOBAL_ASM("data/z_En_Cs.data_00521B74.s")
GLOBAL_ASM("data/z_En_Cs.data_00521B98.s")
GLOBAL_ASM("../src/game/actors/En_Cs/data_syms.s")

ActorInit En_Cs_InitVars = {
    ACTOR_EN_CS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_CS,
    sizeof(EnCs),
    (ActorFunc)EnCs_Init,
    (ActorFunc)EnCs_Destroy,
    (ActorFunc)EnCs_Update,
    (ActorFunc)EnCs_Draw,
};

#if 0
// Draft: 36 of 157 words differ (checked with fastchk). Remaining diff = inlined MTX34_Identity() guarded-static init VFP numbering
// (same blocker class as EnMm_Init / End_Title) + play(r5)/dst(r7) callee-saved swap that comes with it, and the walkSpeed/actionFunc store order.
void EnCs_Init(Actor* thisx, PlayState* play) {
    EnCs* self = (EnCs*)thisx;

    if (gSaveContext.nightFlag != 0) {
        Actor_Kill(thisx);
        return;
    }

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 19.0f);
    FUN_00372f38(thisx, play, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 15);
    self->skelAnime.unk_75[0] = 0;
    SkelAnime_SetFaceAnimations(&self->face, &self->skelAnime, 0, -1, -1);
    self->model = FUN_0036a924(thisx, play, 1, 0x5A);
    {
        MTX34* dst = &self->spookyMaskMtx;
        MTX34CopyAsm(dst, (MTX34*)&MTX34_Identity());
    }

    Collider_InitCylinder(play, (ColliderCylinder*)self->collider);
    Collider_SetCylinder(play, (ColliderCylinder*)self->collider, thisx, &EnCs_sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, &EnCs_sDamageTable, &EnCs_sColChkInfoInit2);
    Actor_UpdateBgCheckInfo(play, thisx, 0.0f, 0.0f, 0.0f, 4);

    FUN_00353020(&self->skelAnime, EnCs_sAnimationInfo[0].animation, 1.0f, 0.0f,
                 (f32)Animation_GetLastFrame(&self->skelAnime, *(s32*)EnCs_sAnimationInfo[0].animation),
                 EnCs_sAnimationInfo[0].mode, EnCs_sAnimationInfo[0].morphFrames);

    self->actor.gravity = -1.0f;
    self->actor.targetMode = 6;
    self->path = self->actor.params & 0xFF;
    self->unk_20C = 0;
    self->currentAnimIndex = -1;
    self->talkState = 0;
    EnCs_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
    self->actionFunc = EnCs_Walk;
    self->walkSpeed = 1.0f;
}
#endif

GLOBAL_ASM("asm/EnCs_Init.s")

void EnCs_Destroy(Actor* thisx, PlayState* play) {
    EnCs* self = (EnCs*)thisx;

    FUN_0035021c(self->model);
    FUN_003508b8(thisx, 0);
    SkelAnime_Free2(&self->skelAnime);
    self->face.Destroy();
    Collider_DestroyCylinder(play, (ColliderCylinder*)self->collider);
}

#if 0
// Draft: 57 of 266 words differ (checked with fastchk; EnCs_CurFramePtr accessor TU makes the add #0x3c sites match only after link).
// Structure matches (inlined GetTalkState/GetTextId/HandleTalking, 7-entry jump table via full case list). Open: ROM hoists
// `add r7, play, #0x2000` (player base) to the top of HandleTalking and loads the player late via [r7,#0xac]; ours loads the
// pointer through the 0x20ac literal in the offer-talk branch (~25 variants: play2/player locals/ActorContext*), and the
// 2-branch constant (`mov r0,#1` vs shared r7=1).
static inline s32 EnCs_GetTalkState(EnCs* self, PlayState* play) {
    s32 talkState = 1;

    switch (Message_GetState(&play->msgCtx)) {
        case 4:
            if (Message_ShouldAdvance(play)) {
                if (FUN_00369f3c(play) == 0) {
                    self->actor.textId = 0x2026;
                    EnCs_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
                    talkState = 2;
                } else {
                    self->actor.textId = 0x2024;
                    EnCs_SetAnimFromIndex(self, 1, &self->currentAnimIndex);
                    talkState = 2;
                }
            }
            break;
        case 6:
            if (Message_ShouldAdvance(play)) {
                if (self->actor.textId == 0x2026) {
                    FUN_00345fcc(play);
                    FUN_00376a78(play, 0x2C);
                    gSaveContext.itemGetInf[3] |= 0x400;
                    FUN_00376a60(30);
                    self->actor.textId = 0x2027;
                    talkState = 2;
                } else {
                    talkState = 0;
                }
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
            break;
    }

    return talkState;
}

static inline s32 EnCs_GetTextId(EnCs* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 textId = Text_GetFaceReaction(play, 0xF);

    if (gSaveContext.itemGetInf[3] & 0x400) {
        if (textId == 0) {
            textId = 0x2028;
        }
    } else if (*(u8*)((u8*)player + 0x1B7) == 3) {
        textId = 0x2023;
    } else {
        if (textId == 0) {
            textId = 0x2022;
        }
    }

    return textId;
}

static inline void EnCs_HandleTalking(EnCs* self, PlayState* play) {
    s16 sp2A;
    s16 sp28;

    if (self->talkState == 2) {
        Message_ContinueTextbox(play, self->actor.textId);
        self->talkState = 1;
    } else if (self->talkState == 1) {
        self->talkState = EnCs_GetTalkState(self, play);
    } else if (Actor_ProcessTalkRequest(&self->actor, play)) {
        if ((self->actor.textId == 0x2022) || ((self->actor.textId != 0x2022) && (self->actor.textId != 0x2028))) {
            EnCs_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
        }

        if ((self->actor.textId == 0x2023) || (self->actor.textId == 0x2028)) {
            EnCs_SetAnimFromIndex(self, 1, &self->currentAnimIndex);
        }

        if (self->actor.textId == 0x2023) {
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
        }

        self->talkState = 1;
    } else {
        Actor_GetScreenPos(play, &self->actor, &sp2A, &sp28);

        if ((sp2A >= 0) && (sp2A <= 320) && (sp28 >= 0) && (sp28 <= 240) &&
            FUN_0036bb28(&self->actor, play, 100.0f)) {
            self->actor.textId = EnCs_GetTextId(self, play);
        }
    }
}

void EnCs_Update(Actor* thisx, PlayState* play) {
    EnCs* self = (EnCs*)thisx;
    s32 frameChanged = 1;
    s32 frame = EN_CS_CURFRAME();

    if (self->prevFrame == frame) {
        frameChanged = 0;
    }
    self->prevFrame = frame;

    if (frameChanged) {
        if (self->currentAnimIndex == 0) {
            if ((EN_CS_CURFRAME() == 9) || (EN_CS_CURFRAME() == 23)) {
                Audio_PlayActorSound2(&self->actor, 0x01000246);
            }
        } else if (self->currentAnimIndex == 1) {
            if ((EN_CS_CURFRAME() == 10) || (EN_CS_CURFRAME() == 25)) {
                Audio_PlayActorSound2(&self->actor, 0x01000246);
            }
        } else if ((self->currentAnimIndex == 2) && (EN_CS_CURFRAME() == 20)) {
            Audio_PlayActorSound2(&self->actor, 0x01000246);
        }
    }

    Collider_UpdateCylinder(&self->actor, (ColliderCylinder*)self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)self->collider);

    self->actionFunc(self, play);

    EnCs_HandleTalking(self, play);

    self->eyeBlinkTimer--;

    if (self->eyeBlinkTimer < 0) {
        self->eyeIndex++;

        if (self->eyeIndex >= 3) {
            self->eyeIndex = 0;
        }

        self->eyeBlinkTimer = (EnCs_sEyeBlinkFrames[self->eyeIndex] > 0)
                                  ? (s32)(0.5f + (EnCs_sEyeBlinkFrames[self->eyeIndex] * 3.0f) * 0.5f)
                                  : (s32)((EnCs_sEyeBlinkFrames[self->eyeIndex] * 3.0f) * 0.5f - 0.5f);
    }
}
#endif

GLOBAL_ASM("asm/EnCs_Update.s")

void EnCs_Draw(Actor* thisx, GameState* state) {
    EnCs* self = (EnCs*)thisx;

    { FaceAnimations* fa = &self->face; fa->FUN_0035e3a4(0, self->eyeIndex); }
    self->face.FUN_0035e330();
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnCs_OverrideLimbDraw, EnCs_PostLimbDraw, thisx, 0);

    if (gSaveContext.itemGetInf[3] & 0x400) {
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&self->spookyMaskMtx);
        self->model->FUN_00372170(0);
    }
}

void Matrix_RotateX(MTX34* mtx, s32 mode, f32 angle);
f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);

s32 EnCs_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnCs* self = (EnCs*)thisx;
    f32 angle, sinAngle, cosAngle, a, b;

    if (self->flag & 1) {
        f32 k = 1.0f / 32768.0f;
        f32 pi = M_PI;
        f32 zero = 0.0f;
        switch (limbIndex) {
        case 7:
            angle = (f32)self->headRot.y * k * pi;
            Matrix_RotateX(mtx, 1, angle);
            angle = (f32)self->headRot.x * k * pi;
            if (angle != zero) {
                sinAngle = __hardfp_sinf(angle);
                cosAngle = __hardfp_cosf(angle);
                a = ((f32*)mtx)[0]; b = ((f32*)mtx)[2];
                ((f32*)mtx)[0] = a * cosAngle - b * sinAngle;
                ((f32*)mtx)[2] = a * sinAngle + b * cosAngle;
                a = ((f32*)mtx)[4]; b = ((f32*)mtx)[6];
                ((f32*)mtx)[4] = a * cosAngle - b * sinAngle;
                ((f32*)mtx)[6] = a * sinAngle + b * cosAngle;
                a = ((f32*)mtx)[8]; b = ((f32*)mtx)[10];
                ((f32*)mtx)[8] = a * cosAngle - b * sinAngle;
                ((f32*)mtx)[10] = a * sinAngle + b * cosAngle;
            }
            break;
        case 14:
            angle = (f32)self->torsoRot.y * k * pi;
            Matrix_RotateX(mtx, 1, angle);
            angle = (f32)self->torsoRot.x * k * pi;
            if (angle != zero) {
                sinAngle = __hardfp_sinf(angle);
                cosAngle = __hardfp_cosf(angle);
                a = ((f32*)mtx)[0]; b = ((f32*)mtx)[1];
                ((f32*)mtx)[0] = a * cosAngle + b * sinAngle;
                ((f32*)mtx)[1] = b * cosAngle - a * sinAngle;
                a = ((f32*)mtx)[4]; b = ((f32*)mtx)[5];
                ((f32*)mtx)[4] = a * cosAngle + b * sinAngle;
                ((f32*)mtx)[5] = b * cosAngle - a * sinAngle;
                a = ((f32*)mtx)[8]; b = ((f32*)mtx)[9];
                ((f32*)mtx)[8] = a * cosAngle + b * sinAngle;
                ((f32*)mtx)[9] = b * cosAngle - a * sinAngle;
            }
            break;
        }
    }

    return 0;
}

#if 0
// Draft, 6 words: static Vector3f init VFP order (ROM x->s0,y->s1,z->s2; ours x->s2,y->s0,z->s1). ~75 ctor/param/decl-order variants tried, best 4w. Guard 0x521B98 / object 0x599010 needs a pin (orchestrator).
static inline void EnCs_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnCs_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f D_809E2970(500.0f, 800.0f, 0.0f);
    EnCs* self = (EnCs*)thisx;

    if (limbIndex == 14) {
        EnCs_MultVec3f(&D_809E2970, (Vector3f*)&self->actor.focus.pos, mtx);
        self->spookyMaskMtx = *mtx;
    }
}
#endif

GLOBAL_ASM("asm/EnCs_PostLimbDraw.s")

void EnCs_SetAnimFromIndex(EnCs* self, s32 index, s32* currentIndex) {
    f32 morphFrames;

    if ((*currentIndex < 0) || (index == *currentIndex)) {
        morphFrames = 0.0f;
    } else {
        morphFrames = EnCs_sAnimationInfo[index].morphFrames;
    }

    if (EnCs_sAnimationInfo[index].frameCount >= 0.0f) {
        FUN_00353020(&self->skelAnime, EnCs_sAnimationInfo[index].animation, EnCs_sAnimationInfo[index].frameCount, 0.0f,
                     (f32)Animation_GetLastFrame(&self->skelAnime, *(s32*)EnCs_sAnimationInfo[index].animation),
                     EnCs_sAnimationInfo[index].mode, morphFrames);
    } else {
        FUN_00353020(&self->skelAnime, EnCs_sAnimationInfo[index].animation, EnCs_sAnimationInfo[index].frameCount,
                     (f32)Animation_GetLastFrame(&self->skelAnime, *(s32*)EnCs_sAnimationInfo[index].animation), 0.0f,
                     EnCs_sAnimationInfo[index].mode, morphFrames);
    }

    *currentIndex = index;
}

void EnCs_Talk(EnCs* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (FUN_00370734(&self->skelAnime)) {
        EnCs_SetAnimFromIndex(self, self->currentAnimIndex, &self->currentAnimIndex);
    }

    self->flag |= 1;
    self->trackPos.x = player->actor.focus.pos.x;
    self->trackPos.y = player->actor.focus.pos.y;
    self->trackPos.z = player->actor.focus.pos.z;
    FUN_0034c664(&self->actor, &self->unk_268, 0, 4);

    if (self->talkState == 0) {
        EnCs_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
        self->actionFunc = EnCs_Walk;
        self->flag &= ~1;
    }
}

void EnCs_Wait(EnCs* self, PlayState* play) {
    s32 animIndex;

    if (self->talkState != 0) {
        self->actionFunc = EnCs_Talk;
        return;
    }

    if (FUN_00370734(&self->skelAnime)) {
        animIndex = self->currentAnimIndex;

        if (self->talkState == 0) {
            if (self->animLoopCount > 0) {
                self->animLoopCount--;
            } else {
                animIndex = 0;
                self->actionFunc = EnCs_Walk;
            }
        }

        EnCs_SetAnimFromIndex(self, animIndex, &self->currentAnimIndex);
    }
}

static inline s32 EnCs_GetwaypointCount(EnCsPath* pathList, s32 pathIndex) {
    EnCsPath* path = &pathList[pathIndex];

    return path->count;
}

static inline s32 EnCs_GetPathPoint(EnCsPath* pathList, Vec3f* dest, s32 pathIndex, s32 waypoint) {
    EnCsPath* path = pathList;
    Vec3s* pathPos;

    path += pathIndex;
    pathPos = &path->points[waypoint];

    dest->x = pathPos->x;
    dest->y = pathPos->y;
    dest->z = pathPos->z;

    return 0;
}

void EnCs_Walk(EnCs* self, PlayState* play) {
    s32 rnd;
    s32 animIndex;

    if (self->talkState != 0) {
        self->actionFunc = EnCs_Talk;
        return;
    }

    if (FUN_00370734(&self->skelAnime)) {
        animIndex = self->currentAnimIndex;

        if (self->talkState == 0) {
            if (gSaveContext.itemGetInf[3] & 0x400) {
                rnd = Rand_ZeroOne() * 10.0f;
            } else {
                rnd = Rand_ZeroOne() * 5.0f;
            }

            if (rnd == 0) {
                if (gSaveContext.itemGetInf[3] & 0x400) {
                    animIndex = 2.0f * Rand_ZeroOne();
                    animIndex = (animIndex == 0) ? 2 : 1;
                } else {
                    animIndex = 2;
                }

                self->actionFunc = EnCs_Wait;
            } else {
                animIndex = 0;
            }
        }

        EnCs_SetAnimFromIndex(self, animIndex, &self->currentAnimIndex);
    }

    if (self->talkState == 0) {
        if (((EN_CS_CURFRAME() >= 8) && (EN_CS_CURFRAME() <= 15)) ||
            ((EN_CS_CURFRAME() >= 23) && (EN_CS_CURFRAME() <= 29)) || (EN_CS_CURFRAME() == 0)) {
            self->walkSpeed = 0.0f;
        } else {
            self->walkSpeed = 1.0f;
        }

        {
            f32 xDiff;
            f32 zDiff;
            Vec3f pathPos;
            s32 waypointCount;
            s16 walkAngle1;
            s16 walkAngle2;

            { PlayState* play2 = play;
            EnCs_GetPathPoint((EnCsPath*)play2->setupPathList, &pathPos, self->path, self->waypoint); }
            xDiff = pathPos.x - self->actor.world.pos.x;
            zDiff = pathPos.z - self->actor.world.pos.z;
            walkAngle1 = RAD_TO_BINANG(Math_FAtan2F(xDiff, zDiff));
            self->walkAngle = walkAngle1;
            self->walkDist = sqrtf((xDiff * xDiff) + (zDiff * zDiff));

            while (self->walkDist <= 10.44f) {
                self->waypoint++;
                waypointCount = EnCs_GetwaypointCount((EnCsPath*)play->setupPathList, self->path) - 1;

                if ((self->waypoint < 0) || (self->waypoint > waypointCount)) {
                    self->waypoint = 0;
                }

                EnCs_GetPathPoint((EnCsPath*)play->setupPathList, &pathPos, self->path, self->waypoint);
                xDiff = pathPos.x - self->actor.world.pos.x;
                zDiff = pathPos.z - self->actor.world.pos.z;
                walkAngle2 = RAD_TO_BINANG(Math_FAtan2F(xDiff, zDiff));
                self->walkAngle = walkAngle2;
                self->walkDist = sqrtf((xDiff * xDiff) + (zDiff * zDiff));
            }

            Math_SmoothStepToS(&self->actor.shape.rot.y, self->walkAngle, 1, 2500, 0);
            self->actor.world.rot.y = self->actor.shape.rot.y;
            self->actor.speedXZ = self->walkSpeed;
            Actor_MoveForward(&self->actor);
            Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        }
    }
}

}
