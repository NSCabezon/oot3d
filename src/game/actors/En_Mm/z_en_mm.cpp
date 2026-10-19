extern "C" {
#include "z_en_mm.hpp"
#include "mtx34_identity.hpp"

#define FLAGS 0x00000019

// N64 func_80AADA70 (3DS adds the credits-scene case).
static inline s32 EnMm_IsDay(PlayState* play) {
    s32 isDay = 0;

    if (play->sceneNum == 0x63 && gSaveContext.cutsceneIndex == 0xFFF4) {
        return 1;
    }
    if (gSaveContext.dayTime > 0x3555 && gSaveContext.dayTime <= 0xD556) {
        isDay = 1;
    }
    return isDay;
}

void EnMm_Init(Actor* actor, GameState* state);
void EnMm_Destroy(Actor* actor, GameState* state);
void EnMm_Update(Actor* actor, GameState* state);
void EnMm_Draw(Actor* actor, GameState* state);

void FUN_00157108(EnMm* self, PlayState* play);
void FUN_001e943c(EnMm* self, PlayState* play);
void FUN_0036bcc8(PlayState* play, Actor* actor, Vec3s* headRot, Vec3s* torsoRot, Vec3f focusPos, s32 yawLimit);
void FUN_00156b98(EnMm* self, PlayState* play);
void FUN_0035021c(void* a);
void FUN_0034e01c(void* arg, void* pos);
s32 FUN_003731e0(SkelAnime* skelAnime);
void FUN_00353aa4(EnMm* self, s32 index, s32* currentIndex);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

typedef struct EnMmAnimInfo {
    /* 0x00 */ s32 animIdx;
    /* 0x04 */ f32 playSpeed;
    /* 0x08 */ u8 mode;
    /* 0x0C */ f32 morphFrames;
} EnMmAnimInfo; // size = 0x10

extern EnMmAnimInfo EnMm_sAnimationInfo[];
extern ColliderCylinderInit EnMm_sCylinderInit;
extern CollisionCheckInfoInit2 EnMm_sColChkInfoInit;
extern DamageTable EnMm_sDamageTable;
extern u32 EnMm_sHeadOffsetGuard;
extern u32 EnMm_sData[]; // 0x0052C754: [0] = sInitChain, [1] = guard of EnMm_PostLimbDraw's function-local static
int __cxa_guard_acquire(unsigned int* guard_object);
void __cxa_guard_release(unsigned int* guard_object);
SkeletonAnimationModel* FUN_0036a924(Actor* actor, PlayState* play, s16 objectSlot, s32 arg3);
void FUN_00288b84(EnMm* self, PlayState* play);
s32 Player_InCsMode(PlayState* play);
s32 Collider_DestroyCylinder(PlayState* play, ColliderCylinder* collider);
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 EnMm_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnMm_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);

GLOBAL_ASM("data/z_En_Mm.data.s")
GLOBAL_ASM("../src/game/actors/En_Mm/data_syms.s")

ActorInit En_Mm_InitVars = {
    ACTOR_EN_MM,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MM,
    sizeof(EnMm),
    (ActorFunc)EnMm_Init,
    (ActorFunc)EnMm_Destroy,
    (ActorFunc)EnMm_Update,
    (ActorFunc)EnMm_Draw,
};

// EnMm_Init: 23 words off with the shared MTX34Init ctor, all in the inlined identity-MTX34 guard block (VFP register
// numbering; same open issue as End_Title FUN_00250540 / En_Fr FUN_0034251c). The rest matches. With a local copy of
// the ctor taking params in cell order [1,4,5,2,0,3,6,7,8,9,11,10] (row-major cells x00..x23; stores in row-major
// order) only 4 words remain (x02/x10 get s8/s0 instead of s0/s8). Search script: see STATUS.md.
// NOTE: sInitChain @0x52C754 is still in the asm data chunk (EnMm_sData[0]); when Init becomes C, define it as a
// static, split the chunk (pin_static.py split 0x52C754 --size 8) and turn PostLimbDraw's hand guard into a real
// function-local static (guard 0x52C758 follows sInitChain in the same TU .data section: pool base+4).
#if 0
void EnMm_Init(Actor* actor, GameState* state) {
    EnMm* self = (EnMm*)actor;
    PlayState* play = (PlayState*)state;

    Actor_ProcessInitChain(actor, (InitChainEntry*)&EnMm_sData[0]);
    ActorShape_Init(&actor->shape, 0.0f, ActorShadow_DrawCircle, 21.0f);
    FUN_00372f38(actor, play, &self->model, 0, 0);
    FUN_00353c9c(actor, play, &self->skelAnime, 0, 1, self->unk_228, (u8*)self + 0x534, 15);
    SkelAnime_SetFaceAnimations((FaceAnimations*)&self->unk_8E0, &self->skelAnime, -1, 0, -1);
    self->unk_AAC = FUN_0036a924(actor, play, 1, 0x59);
    {
        MTX34* dst = &self->unk_AB0;
        MTX34CopyAsm(dst, (MTX34*)&MTX34_Identity());
    }

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, actor, &EnMm_sCylinderInit);
    CollisionCheck_SetInfo2(&actor->colChkInfo, &EnMm_sDamageTable, &EnMm_sColChkInfoInit);

    Actor_UpdateBgCheckInfo(play, actor, 0.0f, 0.0f, 0.0f, 4);
    Animation_Change_00375c08(&self->skelAnime, EnMm_sAnimationInfo[0].animIdx, 1.0f, 0.0f,
                              (f32)Animation_GetLastFrame(&self->skelAnime, EnMm_sAnimationInfo[0].animIdx),
                              EnMm_sAnimationInfo[0].mode, EnMm_sAnimationInfo[0].morphFrames);

    self->unk_8A8 = actor->params & 0xFF;
    self->unk_8AC = 2;
    actor->gravity = -1.0f;
    self->unk_8A4 = 0;
    self->unk_8D4 = 0;
    self->unk_8BC = 3.0f;
    self->unk_8C0 = actor->objBankIndex;
    actor->targetMode = 2;

    if (EnMm_IsDay(play) == 1) {
        self->unk_8A0 = 1;
        FUN_00353aa4(self, 0, &self->unk_8D4);
        self->actionFunc = FUN_00157108;
    } else {
        self->unk_8A0 = 0;
        FUN_00353aa4(self, 2, &self->unk_8D4);
        self->actionFunc = FUN_00156b98;
    }

    if (play->sceneNum == 0x63 && gSaveContext.cutsceneIndex == 0xFFF4) {
        if (gSaveContext.itemGetInf[3] & 0x800) {
            actor->world.pos.x = 30.0f;
            actor->world.pos.z = -100.0f;
            FUN_00353aa4(self, 4, &self->unk_8D4);
            self->actionFunc = FUN_00288b84;
            self->unk_8A0 = 0;
        }
    }
}
#endif

GLOBAL_ASM("asm/EnMm_Init.s")

void EnMm_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnMm* self = (EnMm*)actor;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_0035021c(self->unk_AAC);
    FUN_003508b8(actor, self->model, 0);
    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)&self->unk_8E0)->Destroy();
}

// EnMm_Update: locals whose address is taken (sp26/sp24) must stay in the final else-block, else the tail call to
// CollisionCheck_SetOC is lost. The dead `EnMm* s2` alias on the HandleTextState store fixes the RMW/bic register
// choice in the inlined cases 4 and 6 (R11 near-miss generator).
void FUN_00345fcc(PlayState* play);
s32 Item_Give(PlayState* play, s32 item);
void Rupees_ChangeBy(s16 value);
// N64 func_80AADAA0
static inline s32 EnMm_HandleTextState(EnMm* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 sp1C = 1;

    switch (Message_GetState(&play->msgCtx)) {
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4:
            if (Message_ShouldAdvance(play)) {
                if (FUN_00369f3c(play) == 0) {
                    player->actor.textId = 0x202D;
                    self->flags &= ~1;
                    FUN_00353aa4(self, 6, &self->unk_8D4);
                } else {
                    player->actor.textId = 0x202C;
                    gSaveContext.infTable[0x17] |= 0x1000;
                }
                sp1C = 2;
            }
            break;
        case 5:
            if (Message_ShouldAdvance(play)) {
                FUN_00345fcc(play);
                Item_Give(play, 0x2C);
                gSaveContext.itemGetInf[3] |= 0x800;
                Rupees_ChangeBy(500);
                player->actor.textId = 0x202E;
                sp1C = 2;
            }
            break;
        case 6:
            if (Message_ShouldAdvance(play)) {
                if ((player->actor.textId == 0x202E) || (player->actor.textId == 0x202C)) {
                    self->flags |= 1;
                    FUN_00353aa4(self, 2, &self->unk_8D4);
                }
                sp1C = 0;
            }
            break;
    }
    return sp1C;
}

// N64 EnMm_GetTextId
static inline s32 EnMm_GetTextId(EnMm* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 textId = Text_GetFaceReaction(play, 0x1C);

    if (gSaveContext.itemGetInf[3] & 0x800) {
        if (textId == 0) {
            textId = 0x204D;
        }
    } else if (*((u8*)player + 0x1B7) == 4) {
        textId = (gSaveContext.infTable[0x17] & 0x1000) ? 0x202B : 0x202A;
    } else if (textId == 0) {
        textId = 0x2029;
    }
    return textId;
}

void EnMm_Update(Actor* actor, GameState* state) {
    EnMm* self = (EnMm*)actor;
    PlayState* play = (PlayState*)state;
    Player* player;

    self->actionFunc(self, play);

    player = GET_PLAYER(play);
    if (self->unk_89C == 2) {
        Message_ContinueTextbox(play, player->actor.textId);
        self->unk_89C = 1;
    } else if (self->unk_89C == 1) {
        EnMm* s2 = self;
        s2->unk_89C = EnMm_HandleTextState(self, play);
    } else {
        if (Actor_ProcessTalkRequest(&self->actor, play)) {
            self->unk_89C = 1;

            if (self->unk_8D4 != 5) {
                if ((self->actor.textId == 0x202A) || (self->actor.textId == 0x202B)) {
                    FUN_00353aa4(self, 5, &self->unk_8D4);
                    FUN_00372244((u8*)play + 0x5FCC, 0x1E, 0x0100048C);
                }
            }
        } else {
            f32 yawDiff;
            s16 sp26;
            s16 sp24;

            Actor_GetScreenPos(play, &self->actor, &sp26, &sp24);
            yawDiff = ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y));

            if ((sp26 >= 0) && (sp26 <= 0x190) && (sp24 >= 0) && (sp24 <= 0xF0) && (yawDiff <= 17152.0f) &&
                (self->unk_89C != 3) && FUN_0036bb28(&self->actor, play, 100.0f)) {
                self->actor.textId = EnMm_GetTextId(self, play);
            }
        }
    }
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}

void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);

void EnMm_Draw(Actor* actor, GameState* state) {
    EnMm* self = (EnMm*)actor;
    PlayState* play = (PlayState*)state;

    FaceAnimations* fa = (FaceAnimations*)&self->unk_8E0;
    fa->FUN_0035e3a4(1, self->unk_8A0);
    GraphicsContext_UnknownStub(play->state.gfxCtx);
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, EnMm_OverrideLimbDraw, EnMm_PostLimbDraw, actor, 0);
    ((FaceAnimations*)&self->unk_8E0)->FUN_0035e330();
    if (gSaveContext.itemGetInf[3] & 0x800) {
        if (Object_GetIndex(&play->objectCtx, 0x15) >= 0) {
            ((SkeletonAnimationModel*)self->unk_AAC)->SetUnkAC(1);
            ((SkeletonAnimationModel*)self->unk_AAC)->FUN_003721e0(&self->unk_AB0);
            ((SkeletonAnimationModel*)self->unk_AAC)->FUN_00372170(0);
        }
    }
}

s32 EnMm_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnMm* self = (EnMm*)thisx;

    if (self->flags & 1) {
        if (limbIndex == 7) {
            FUN_0034e01c(mtx, &self->unk_8CA);
        } else if (limbIndex == 0xE) {
            FUN_0034e01c(mtx, &self->unk_8C4);
        }
    }
    return 0;
}

// Function-local `static Vector3f headOffset(200, 800, 0)` of the original. Object: .bss, pinned at 0x0059B478 in
// data/oot3d.ld (scripts/pin_static.py bss 0x59B478 12 "z_en_mm.o(+BSS)" --label z_en_mm). Guard: word at
// 0x0052C758 (EnMm_sData[1], inside the asm data chunk), hand-written because armcc only addresses it as
// "sInitChain + 4" when sInitChain is referenced by C code (EnMm_Init is still asm).
static Vec3f sHeadOffset;

static inline void EnMm_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnMm_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnMm* self = (EnMm*)thisx;
    Actor* actor = &self->actor;

    if (!(EnMm_sData[1] & 1) && __cxa_guard_acquire(&EnMm_sHeadOffsetGuard)) {
        f32 x = 200.0f;
        f32 y = 800.0f;
        f32 z = 0.0f;
        sHeadOffset.x = x;
        sHeadOffset.y = y;
        sHeadOffset.z = z;
        __cxa_guard_release(&EnMm_sHeadOffsetGuard);
    }
    if (limbIndex == 0xE) {
        EnMm_MultVec3f((Vector3f*)&sHeadOffset, (Vector3f*)&actor->focus.pos, mtx);
        self->unk_AB0 = *mtx;
    }
}

// N64 func_80AAE224
void FUN_001e93d4(EnMm* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        self->actor.speedXZ = 0.0f;
        self->actionFunc = FUN_00157108;
        self->unk_8A4 = 0;
        self->unk_8A0 = 0;
        self->flags |= 1;
        self->unk_89C = 0;
        FUN_00353aa4(self, 2, &self->unk_8D4);
    }
}

void FUN_001e943c(EnMm* self, PlayState* play) {
    if (FUN_003731e0(&self->skelAnime)) {
        self->actionFunc = FUN_00156b98;
        self->unk_8D8 = 0;
        if (*(u16*)((u8*)&gSaveContext + 0xF0E) & 0x800) {
            FUN_00353aa4(self, 4, &self->unk_8D4);
            self->unk_8A0 = 0;
        } else {
            FUN_00353aa4(self, 0, &self->unk_8D4);
            self->unk_8A0 = 1;
        }
        self->unk_8A4 = 1;
    }
}

// N64 EnMm_ChangeAnim
void FUN_00353aa4(EnMm* self, s32 index, s32* currentIndex) {
    f32 morphFrames;

    if ((*currentIndex < 0) || (index == *currentIndex)) {
        morphFrames = 0.0f;
    } else {
        morphFrames = EnMm_sAnimationInfo[index].morphFrames;
    }
    if (EnMm_sAnimationInfo[index].playSpeed >= 0.0f) {
        Animation_Change_00375c08(&self->skelAnime, EnMm_sAnimationInfo[index].animIdx,
                                  EnMm_sAnimationInfo[index].playSpeed, 0.0f,
                                  (f32)Animation_GetLastFrame(&self->skelAnime, EnMm_sAnimationInfo[index].animIdx),
                                  EnMm_sAnimationInfo[index].mode, morphFrames);
    } else {
        Animation_Change_00375c08(&self->skelAnime, EnMm_sAnimationInfo[index].animIdx,
                                  EnMm_sAnimationInfo[index].playSpeed,
                                  (f32)Animation_GetLastFrame(&self->skelAnime, EnMm_sAnimationInfo[index].animIdx),
                                  0.0f, EnMm_sAnimationInfo[index].mode, morphFrames);
    }
    *currentIndex = index;
}

// N64 func_80AAE598
void FUN_00157108(EnMm* self, PlayState* play) {
    FUN_0036bcc8(play, &self->actor, &self->unk_8C4, &self->unk_8CA, self->actor.focus.pos, 0x4300);
    FUN_003731e0(&self->skelAnime);

    if (EnMm_IsDay(play) && self->unk_89C == 0) {
        self->unk_89C = 3;
        self->actionFunc = FUN_001e943c;
        self->flags &= ~1;
        FUN_00353aa4(self, 3, &self->unk_8D4);
    }
}

void FUN_00288b84(EnMm* self, PlayState* play) {
    if (!Player_InCsMode(play)) {
        FUN_003731e0(&self->skelAnime);
        self->unk_8BC = 10.0f;
        self->skelAnime.playSpeed = 2.0f;
        self->actor.speedXZ = 10.0f;
        self->actor.world.rot.y = self->actor.shape.rot.y;
        self->actor.shape.rot.y += 0x800;
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    }
}

}
