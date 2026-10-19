extern "C" {
#include "z_en_ma1.hpp"

#define FLAGS 0x02000039

void EnMa1_Init(Actor* thisx, PlayState* play);
void EnMa1_Destroy(Actor* actor, GameState* state);
void EnMa1_Update(Actor* thisx, PlayState* play);
void EnMa1_Draw(Actor* thisx, PlayState* play);

void FUN_001522b4(EnMa1* self, PlayState* play);
void FUN_001c52cc(EnMa1* self, PlayState* play);
void FUN_003d2a14(EnMa1* self, PlayState* play);
void FUN_003d2a10(EnMa1* self, PlayState* play);
void FUN_00371680(PlayState* play, s32 arg, s32 arg2);
void FUN_0037073c(PlayState* play, s32 arg);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_0033d364(f32 distFromCamEye, u32 bgmId); // Audio_UpdateMalonSinging
__attribute__((noinline)) void FUN_00348800(EnMa1* self, s32 index); // out-of-line in ROM
void FUN_001e63ec(EnMa1* self, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);

GLOBAL_ASM("data/z_En_Ma1.data.s")

ActorInit En_Ma1_InitVars = {
    ACTOR_EN_MA1,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MA1,
    sizeof(EnMa1),
    (ActorFunc)EnMa1_Init,
    (ActorFunc)EnMa1_Destroy,
    (ActorFunc)EnMa1_Update,
    (ActorFunc)EnMa1_Draw,
};

void FUN_003c6128(EnMa1* self, PlayState* play);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
DamageTable* DamageTable_Get(s32 index);

static inline u8* EnMa1_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

// N64 EnMa1_ShouldSpawn (inlined in ROM).
static inline s32 EnMa1_ShouldSpawn(EnMa1* self, PlayState* play) {
    if ((self->actor.shape.rot.z == 3) && (gSaveContext.sceneSetupIndex == 5)) {
        return true;
    }
    if (!LINK_IS_CHILD) {
        return false;
    }
    if (((play->sceneNum == 0x21) || (play->sceneNum == 0x20)) && !(gSaveContext.eventChkInf[1] & 0x10) &&
        !(gSaveContext.infTable[8] & 0x800)) {
        return true;
    }
    if ((play->sceneNum == 0x5F) && !(gSaveContext.eventChkInf[1] & 0x10)) {
        if (gSaveContext.infTable[8] & 0x800) {
            return true;
        } else {
            gSaveContext.infTable[8] |= 0x800;
            return false;
        }
    }
    if ((play->sceneNum == 0x4C) && IS_NIGHT && (gSaveContext.eventChkInf[1] & 0x10)) {
        return true;
    }
    if (play->sceneNum != 0x63) {
        return false;
    }
    if ((self->actor.shape.rot.z == 3) && IS_DAY && (gSaveContext.eventChkInf[1] & 0x10)) {
        return true;
    }
    return false;
}

void EnMa1_Init(Actor* thisx, PlayState* play) {
    EnMa1* self = (EnMa1*)thisx;
    u8* zar;
    void* cmb;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 18.0f);
    zar = EnMa1_GetObjBank(play, (u8)thisx->objBankIndex) + 0x10;
    cmb = ZAR_GetCMBByIndex(zar, 0);
    FUN_00353e78(zar, play, &self->skelAnime, cmb, thisx->unk_178, -1, self->unk_1A4, (u8*)self + 0x580, 0x13);
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_9E0, &self->skelAnime, 0, 1, -1);
    self->interactInfo.singingDisabled = 1;
    Collider_InitCylinder(play, (ColliderCylinder*)self->unk_BB0);
    Collider_SetCylinder(play, (ColliderCylinder*)self->unk_BB0, thisx, (ColliderCylinderInit*)0x0052C13C);
    CollisionCheck_SetInfo2(&thisx->colChkInfo, DamageTable_Get(22), (CollisionCheckInfoInit2*)0x0052C174);

    if (!EnMa1_ShouldSpawn(self, play)) {
        Actor_Kill(&self->actor);
        return;
    }
    Actor_UpdateBgCheckInfo(play, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.targetMode = 6;
    self->interactInfo.talkState = 0;

    if (!(gSaveContext.eventChkInf[1] & 0x10) || CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
        self->actionFunc = FUN_003d2a14;
        FUN_00348800(self, 2);
    } else {
        self->actionFunc = FUN_003c6128;
        FUN_00348800(self, 2);
    }
}

void EnMa1_Destroy(Actor* thisx, GameState* state) {
    EnMa1* self = (EnMa1*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    ((FaceAnimations*)self->unk_9E0)->Destroy();
    Collider_DestroyCylinder((PlayState*)state, (ColliderCylinder*)self->unk_BB0);
}

void FUN_003d2a10(EnMa1* self, PlayState* play) {
}

void FUN_001c52cc(EnMa1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    *(u32*)((u8*)player + 0x1000 + 0x714) |= 0x800000;

    // Message_GetState inlined by the linker in the ROM: msgCtx state read at play+0x2B7E (u16)
    if (*(u16*)((u8*)play + 0x2B7E) == 3) {
        FUN_003716f0(play, 0x157, 0x14, 0x2a);
        u8* miscCtx = (u8*)0x00588E58;
        s16 unk_A0 = -15;

        *(s16*)(miscCtx + 0xA0) = unk_A0;
        self->actionFunc = FUN_003d2a10;
    }
}

void FUN_003d2a14(EnMa1* self, PlayState* play) {
    if (self->interactInfo.talkState != 0) {
        if (self->skelAnime.animationType != 1) {
            FUN_00348800(self, 1);
        }
    } else {
        if (self->skelAnime.animationType != 2) {
            FUN_00348800(self, 3);
        }
    }

    if (play->sceneNum == 0x5F) {
        if (gSaveContext.eventChkInf[1] & 0x10) {
            Actor_Kill(&self->actor);
            return;
        }
    } else if (gSaveContext.eventChkInf[1] & 0x10) {
        if (!CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
            return;
        }
    }

    if (self->interactInfo.talkState == 2) {
        self->actionFunc = FUN_001e63ec;
        FUN_00371680(play, 4, 0);
    }
}

void FUN_001e63ec(EnMa1* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = FUN_001522b4;
    } else {
        GiveItem(&self->actor, play, 0x47, 120.0f, 10.0f);
    }
}

void FUN_001522b4(EnMa1* self, PlayState* play) {
    if (self->interactInfo.talkState == 3) {
        self->interactInfo.talkState = 0;
        self->actionFunc = FUN_003d2a14;
        u8* gameInfo = (u8*)0x00588758;
        *(u16*)(gameInfo + 0xEE) |= 4;
        FUN_00371680(play, 2, 0);
    }
}

void FUN_00348800(EnMa1* self, s32 index) {
    EnMa1AnimInfo* info = (EnMa1AnimInfo*)0x0052C180 + index;
    f32 frameCount = (f32)Animation_GetLastFrame(&self->skelAnime, info->animIdx);

    Animation_Change_00375c08(&self->skelAnime, info->animIdx, 1.0f, 0.0f, frameCount, info->mode, info->morphFrames);
}

void FUN_0013532c(EnMa1* self, PlayState* play) {
    *(u32*)((u8*)GET_PLAYER(play) + 0x1000 + 0x714) |= 0x800000;

    if (Message_GetState(&play->msgCtx) == 7) {
        FUN_0037073c(play, 0x16);
        self->actionFunc = FUN_001c52cc;
    }
}

void FUN_003523dc(s32 arg);

void FUN_00173710(EnMa1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    *(u32*)((u8*)player + 0x1000 + 0x714) |= 0x800000;

    if (self->interactInfo.talkState == 2) {
        FUN_003523dc(2);
        FUN_0037073c(play, 9);
        self->actor.flags &= ~0x10000;
        self->actionFunc = FUN_0013532c;
    }
}

void FUN_003c6128(EnMa1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->interactInfo.talkState != 0) {
        if (self->skelAnime.animationType != 1) {
            FUN_00348800(self, 1);
        }
    } else {
        if (self->skelAnime.animationType != 2) {
            FUN_00348800(self, 3);
        }
    }

    if (gSaveContext.eventChkInf[1] & 0x40) {
        if (player->stateFlags2 & 0x1000000) {
            player->stateFlags2 |= 0x2000000;
            *(Actor**)((u8*)player + 0x1740) = &self->actor;
            self->actor.textId = 0x2061;
            FUN_00367c7c(play, 0x2061, NULL);
            self->interactInfo.talkState = 1;
            self->actor.flags |= 0x10000;
            self->actionFunc = FUN_00173710;
        } else if (self->actor.xzDistToPlayer < ((ColliderCylinder*)self->unk_BB0)->dim.radius + 30.0f) {
            player->stateFlags2 |= 0x800000;
        }
    }
}

#if 0 // 4/82 words differ (best variant): ROM hoists &gSaveContext+0xE00/+0xF00 bases above the textId compare (needs explicit base pointers + case order 2043,2047,2041,...), but loads 0x2048 literal BEFORE the base literal (ours: after). Natural gSaveContext.field form gives 62/82 (per-case literals). Opus stage 3: also tried SaveContext* local (fn/case scope), flag-index inline setters, array-decay pointers, 24 case orders, decl orders: best stays 4 (+ 2047 cross-jumps to 2041's store instead of 2049's). 2026-10-14 sweep: single-site/pair mutations (play2, thisx alias, tid types, pointer decl order, per-case re-read of the base) also stay at 4. R11 (2026-10-15): 72 natural/pointer-form x case-order x outer-order x tid-type x alias grid, `&gSaveContext.field - k` pointers (4, cleaner than raw casts), raw base per case / u8* base / single pointer, 36 decl x assign perms of tid/ev/inf, greedy+pair mutations: best 4.
u8 FUN_0034696c(PlayState* play, s32 arg1);

s16 FUN_00195b90(PlayState* play, Actor* thisx) {
    s16 talkState = 1;
    PlayState* p = play;

    switch (Message_GetState(&p->msgCtx)) {
        case 2: { // TEXT_STATE_CLOSING
            s32 tid = thisx->textId;
            u16* ev = (u16*)((u8*)&gSaveContext + 0xE00);   // eventChkInf[1] at +0xEE
            u16* inf = (u16*)((u8*)&gSaveContext + 0xF00);  // infTable[8] at +0x20
            switch (tid) {
                case 0x2043:
                    talkState = 1;
                    break;
                case 0x2047:
                    ev[0x77] |= 0x20;
                    talkState = 0;
                    break;
                case 0x2041:
                    inf[0x10] |= 0x10;
                    ev[0x77] |= 0x1;
                    talkState = 0;
                    break;
                case 0x2048:
                    inf[0x10] |= 0x20;
                    talkState = 0;
                    break;
                case 0x2049:
                    ev[0x77] |= 0x40;
                    talkState = 0;
                    break;
                case 0x2061:
                    talkState = 2;
                    break;
                default:
                    talkState = 0;
                    break;
            }
            break;
        }
        case 0:
        case 1:
        case 3:
            talkState = 1;
            break;
        case 4: // TEXT_STATE_CHOICE
        case 5: // TEXT_STATE_EVENT
            if (Message_ShouldAdvance(play)) {
                talkState = 2;
            }
            break;
        case 6: // TEXT_STATE_DONE (ROM calls FUN_0034696c(play, 0); declare: u8 FUN_0034696c(PlayState*, s32))
            if (FUN_0034696c(play, 0)) {
                talkState = 3;
            }
            break;
        case 7:
        case 8:
        case 9:
            talkState = 1;
            break;
    }
    return talkState;
}
#endif

u16 FUN_00195cd8(PlayState* play, Actor* thisx) {
    u16 textId = Text_GetFaceReaction(play, 0x17);

    if (textId != 0) {
        return textId;
    }
    if (CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
        return 0x204A;
    }
    if (gSaveContext.eventChkInf[1] & 0x40) {
        return 0x2049;
    }
    if (gSaveContext.eventChkInf[1] & 0x20) {
        if (gSaveContext.infTable[8] & 0x20) {
            return 0x2049;
        }
        return 0x2048;
    }
    if (gSaveContext.eventChkInf[1] & 0x10) {
        return 0x2047;
    }
    if (gSaveContext.eventChkInf[1] & 0x4) {
        return 0x2044;
    }
    if (gSaveContext.infTable[8] & 0x10) {
        if (gSaveContext.infTable[8] & 0x800) {
            return 0x2043;
        }
        return 0x2042;
    }
    return 0x2041;
}

s32 FUN_00233614(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnMa1* self = (EnMa1*)thisx;

    if (limbIndex == 7) {
        FUN_0034e01c(mtx, &self->interactInfo.torsoRot);
    } else if (limbIndex == 6) {
        FUN_0034e01c(mtx, &self->interactInfo.headRot);
    }
    return 0;
}

// N64 Matrix_MultVec3f(src, dest) argument order; the wrapper gives the predicated call shape.
static inline void EnMa1_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void FUN_00217274(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnMa1* self = (EnMa1*)thisx;
    Vector3f offset(800.0f, 0.0f, 0.0f);

    if (limbIndex == 7) {
        EnMa1_MultVec3f(&offset, (Vector3f*)&self->actor.focus.pos, mtx);
    }
}

void FUN_00342714(PlayState* play, Actor* actor, void* talkState, f32 range, void* getTextId, void* updateTalkState);
void FUN_0034c664(Actor* actor, void* info, s32 arg2, s32 mode); // Npc_TrackPoint
void FUN_0033d270(s32 arg);                                       // Audio_ToggleMalonSinging
s16 FUN_00195b90(PlayState* play, Actor* thisx);
u16 FUN_00195cd8(PlayState* play, Actor* thisx);

// N64 helpers (inlined in ROM; writing them as helpers gives the ROM scheduling/predication).
static inline void EnMa1_UpdateEyes(EnMa1* self) {
    if (DECR(self->interactInfo.blinkTimer) == 0) {
        self->interactInfo.eyeIndex++;
        if (self->interactInfo.eyeIndex >= 3) {
            self->interactInfo.blinkTimer = Rand_S16Offset_003702c8(30, 30);
            self->interactInfo.eyeIndex = 0;
        }
    }
}

static inline void EnMa1_UpdateSinging(EnMa1* self) {
    if (self->skelAnime.animationType == 2) {
        if (self->interactInfo.talkState == 0) {
            if (self->interactInfo.singingDisabled) {
                self->interactInfo.singingDisabled = false;
                FUN_0033d270(false);
            }
        } else {
            if (!self->interactInfo.singingDisabled) {
                self->interactInfo.singingDisabled = true;
                FUN_0033d270(true);
            }
        }
    }
}

static inline void EnMa1_UpdateTracking(EnMa1* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 trackingMode;

    if ((self->interactInfo.talkState == 0) && (self->skelAnime.animationType == 2)) {
        trackingMode = 1;
    } else {
        trackingMode = 0;
    }
    self->interactInfo.trackPos = player->actor.world.pos;
    self->interactInfo.trackPos.y -= -10.0f;
    FUN_0034c664(&self->actor, &self->interactInfo.talkState, 0, trackingMode);
}

void EnMa1_Update(Actor* thisx, PlayState* play) {
    EnMa1* self = (EnMa1*)thisx;
    s32 doTrack = 1;
    s32 culled = 0;

    if (play->sceneNum == 0x20) {
        s16 x;
        s16 y;
        Actor_GetScreenPos(play, thisx, &x, &y);
        if ((u32)(x + 0x14) > 0x1B8 || y < 0 || y > 0xF0) {
            doTrack = 0;
            culled = 1;
        }
    }

    if (!culled) {
        Collider_UpdateCylinder(thisx, (ColliderCylinder*)self->unk_BB0);
        CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)self->unk_BB0);
        FUN_003731e0(&self->skelAnime);
        EnMa1_UpdateEyes(self);

        self->actionFunc(self, play);

        if (self->actionFunc != FUN_003d2a10) {
            FUN_00342714(play, thisx, &self->interactInfo.talkState,
                         ((ColliderCylinder*)self->unk_BB0)->dim.radius + 30.0f, (void*)FUN_00195cd8,
                         (void*)FUN_00195b90);
        }
    }

    EnMa1_UpdateSinging(self);
    if (doTrack) {
        EnMa1_UpdateTracking(self, play);
    }
}

// FaceAnimations calls go through this helper (arg order).
static inline void EnMa1_SetFace(FaceAnimations* f, s32 a, s32 b) {
    f->FUN_0035e3a4(a, b);
}

void EnMa1_Draw(Actor* thisx, PlayState* play) {
    EnMa1* self = (EnMa1*)thisx;
    Camera* activeCam = play->cameraPtrs[play->activeCamera];
    f32 distFromCamEye = Math_Vec3f_DistXZ(&thisx->world.pos, &activeCam->eye);

    FUN_0033d364(distFromCamEye, 0x0100059D); // Audio_UpdateMalonSinging(dist, NA_BGM_LONLON)
    EnMa1_SetFace((FaceAnimations*)self->unk_9E0, 1, self->interactInfo.mouthIndex);
    EnMa1_SetFace((FaceAnimations*)self->unk_9E0, 0, self->interactInfo.eyeIndex);
    ((FaceAnimations*)self->unk_9E0)->FUN_0035e330();
    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, FUN_00233614, FUN_00217274, &self->actor, 0);
}
}
