extern "C" {
#include "z_demo_go.hpp"

#define FLAGS 0x00000010

typedef void (*DemoGoActionFunc)(DemoGo* self, PlayState* play);
typedef void (*DemoGoDrawFunc)(DemoGo* self, PlayState* play);

void DemoGo_Init(Actor* thisx, PlayState* play);
void DemoGo_Destroy(Actor* thisx, PlayState* play);
void DemoGo_Update(Actor* thisx, PlayState* play);
void DemoGo_Draw(Actor* thisx, PlayState* play);

// Action / draw functions living in functions_*.cpp (asm), or unlabeled code (raw address).
void caseD_1_002a0db8(DemoGo* self, PlayState* play);
void caseD_1_0027d748(DemoGo* self, PlayState* play);
void caseD_2_0027d61c(DemoGo* self, PlayState* play);
void caseD_5_003a34dc(DemoGo* self, PlayState* play);
void caseD_6_003a0720(DemoGo* self, PlayState* play);

ActorInit Demo_Go_InitVars = {
    ACTOR_DEMO_GO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OF1D_MAP,
    sizeof(DemoGo),
    (ActorFunc)DemoGo_Init,
    (ActorFunc)DemoGo_Destroy,
    (ActorFunc)DemoGo_Update,
    (ActorFunc)DemoGo_Draw,
};

// Function tables. Unlabeled targets are raw addresses. Draw table at 0x0051BB60, action table at 0x0051BB68.
static struct {
    DemoGoDrawFunc draw[2];
    DemoGoActionFunc action[7];
} sFuncTables = {
    {
        (DemoGoDrawFunc)0x0027D618,
        caseD_1_002a0db8,
    },
    {
        (DemoGoActionFunc)0x0027D878,
        caseD_1_0027d748,
        caseD_2_0027d61c,
        (DemoGoActionFunc)0x0027D7F0,
        (DemoGoActionFunc)0x002AEBE4,
        caseD_5_003a34dc,
        caseD_6_003a0720,
    },
};

void* ZAR_GetCMBByIndex(void* zar, s32 idx);
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skelAnime, void* cmb, void* p, s32 a, s32 b, s32 c, s32 d);
void FUN_00353020(SkelAnime* skelAnime, void* animation, s32 mode, f32 speed, f32 start, f32 end, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void SkelAnime_SetFaceAnimations(FaceAnimations* faceAnims, SkelAnime* skelAnime, s32 p3, s32 p4, s32 p5);

// Object bank lookup: NULL if the slot is out of range or not loaded (same idiom as En_Kakasi2).
static inline u8* DemoGo_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void DemoGo_Init(Actor* thisx, PlayState* play) {
    DemoGo* self = (DemoGo*)thisx;
    SkelAnime* skel;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    skel = &self->skelAnime;
    {
        void* zar = DemoGo_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10;
        void* cmb = ZAR_GetCMBByIndex(zar, 0);
        void* p = self->actor.unk_178;

        FUN_00353e78(zar, play, skel, cmb, p, -1, 0, 0, 0);
    }
    SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_228, skel, 0, 1, -1);
    FUN_00353020(skel, (void*)0x004D868C, 2, 1.0f, 0.0f, (f32)Animation_GetLastFrame(skel, 10), 0.0f);
    self->action = 0;
}

void DemoGo_Destroy(Actor* thisx, PlayState* play) {
    DemoGo* self = (DemoGo*)thisx;

    SkelAnime_Free(&self->skelAnime, play);
    ((FaceAnimations*)self->unk_228)->Destroy();
}

// Update draft (its first 0x28 bytes match, but the asm block at 0x001F0D78..0x001F0DE0 also holds the
// Demo_Ik function at 0x001F0DA0, which the C version would drop and shift everything after it).
#if 0
void DemoGo_Update(Actor* thisx, PlayState* play) {
    DemoGo* self = (DemoGo*)thisx;

    if ((u32)self->action >= 7) {
        return;
    }
    if (sFuncTables.action[self->action] == NULL) {
        return;
    }
    sFuncTables.action[self->action](self, play);
}
#endif
// Update stays asm: its block also contains Demo_Ik code at 0x001F0DA0 (unpinned, no cfg entry).
GLOBAL_ASM("../split/asm/DemoGo_Update.s")
GLOBAL_ASM("../split/asm/FUN_001f0da0.s")

// caseD_2_0027d61c (action 2, N64 func_8097D01C): NOT MATCHED, stays GLOBAL_ASM in functions_240000s.cpp.
// Best draft below: 33/75 words. Dust/sfx tail matches; diffs = play/csCtx in r6/r5 (ROM r5/r6), cue channel
// in r0 (ROM r1), UpdateBgCheckInfo arg scheduling. (GameState* state) + `PlayState* play = play2` local inside
// SpawnDust gave the right r5/r6 split (30 words) but still wrong cue regs.
#if 0
void FUN_0032b060(DemoGo* self, PlayState* play); // N64 func_8097CB0C

s32 DemoGo_GetCueChannel(DemoGo* self) {
    s32 ret;

    switch (self->actor.params) {
        case 0:
            ret = 3;
            break;
        case 1:
            ret = 4;
            break;
        default:
            ret = 5;
            break;
    }
    return ret;
}

static inline void DemoGo_UpdateBgCheckInfo(DemoGo* self, PlayState* play) {
    Actor_UpdateBgCheckInfo(play, &self->actor, 75.0f, 30.0f, 30.0f, 5);
}

static inline void DemoGo_SpawnDust(DemoGo* self, PlayState* play) {
    Vec3f vec = self->actor.world.pos;

    FUN_0037378c(play, &vec, kREG(11) + 100.0f, kREG(12) + 0xA, kREG(13) + 0x12C, kREG(14), 0);
    Audio_PlaySoundGeneral(0x010002EB, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                           &DAT_0054ac24.unk_0);
}

static inline void DemoGo_CheckFall(DemoGo* self, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    CsCmdActorAction* cue;

    if (play->GetCutsceneState()) {
        cue = csCtx->npcActions[DemoGo_GetCueChannel(self)];
        if (cue != NULL && csCtx->frames >= cue->endFrame) {
            DemoGo_SpawnDust(self, play);
            self->action = 3;
        }
    }
}

// Action 2 (N64 func_8097D01C). Moved from functions_240000s.cpp.
void caseD_2_0027d61c(DemoGo* self, PlayState* play) {
    FUN_0032b060(self, play);
    DemoGo_UpdateBgCheckInfo(self, play);
    DemoGo_CheckFall(self, play);
}
#endif

// caseD_5_003a34dc (action 5, N64 func_8097D0D0): NOT MATCHED, stays GLOBAL_ASM in functions_3A0000s.cpp.
// First draft only (103/120 words, 464 vs 480 B). ROM keeps csCtx (play+0x2298) in r6 across GetCutsceneState,
// &shape (r7) and &world (r0) as pointer locals (N64 func_8097CCE0 style), and has e320f000 nops after
// `bl FUN_00370734` setup and after both `cmp r0,#0` of FUN_0036e5e0 (Animation_OnFrame-like, s1 = 1.0f).
#if 0
s32 FUN_0036e5e0(SkelAnime* skelAnime, f32 frame, f32 rate);

static inline s32 DemoGo_GetCueChannel5(DemoGo* self) {
    s32 ret;

    switch (self->actor.params) {
        case 0:
            ret = 3;
            break;
        case 1:
            ret = 4;
            break;
        default:
            ret = 5;
            break;
    }
    return ret;
}

// Action 5 (N64 func_8097D0D0).
void caseD_5_003a34dc(DemoGo* self, PlayState* play) {
    Actor* thisx = &self->actor;
    CsCmdActorAction* cue;
    s32 rotYDelta;
    s32 newRotY;
    s32 thisRotY;

    if (play->GetCutsceneState()) {
        cue = play->csCtx.npcActions[DemoGo_GetCueChannel5(self)];
        if (cue != NULL) {
            thisRotY = thisx->world.rot.y;
            rotYDelta = cue->rot.y - thisRotY;
            if ((rotYDelta > -(kREG(16) + 0x96)) && (rotYDelta < kREG(16) + 0x96)) {
                newRotY = cue->rot.y;
            } else if (rotYDelta > 0) {
                newRotY = (thisRotY + kREG(16)) + 0x96;
            } else {
                newRotY = (thisRotY - kREG(16)) - 0x96;
            }
            thisx->shape.rot.y = newRotY;
            thisx->world.rot.y = newRotY;
        }
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(play, &self->actor, 75.0f, 30.0f, 30.0f, 5);
    FUN_00370734(&self->skelAnime);
    {
        s16* something = (s16*)&self->unk_3F6;
        s16* other = &self->eyeIdx;

        if (DECR(*something) == 0) {
            *something = Rand_S16Offset_003702c8(60, 60);
        }
        *other = *something;
        if (*other >= 3) {
            *other = 0;
        }
    }
    if (FUN_0036e5e0(&self->skelAnime, 12.0f, 1.0f) || FUN_0036e5e0(&self->skelAnime, 25.0f, 1.0f)) {
        Audio_PlaySoundGeneral(0x01000328, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                               &DAT_0054ac24.unk_0);
    }
    *(f32*)self->unk_400 += 1.0f;
    if (*(f32*)self->unk_400 >= 8.0f) {
        self->action = 6;
    }
}
#endif

// Draw mode 1 (N64 func_8097D29C). Moved from functions_2A0000s.cpp.
void caseD_1_002a0db8(DemoGo* self, PlayState* play) {
    s16* eye = &self->eyeIdx;
    SkelAnime* skel = &self->skelAnime;
    FaceAnimations* face = (FaceAnimations*)self->unk_228;

    face->FUN_0035e3a4(0, *eye);
    ((FaceAnimations*)self->unk_228)->FUN_0035e330();
    SkelAnime_DrawOpa(skel, &self->actor.modelMtx, NULL, NULL, &self->actor, 0);
}

void DemoGo_Draw(Actor* thisx, PlayState* play) {
    DemoGo* self = (DemoGo*)thisx;

    if ((u32)self->drawConfig >= 2) {
        return;
    }
    if (sFuncTables.draw[self->drawConfig] == NULL) {
        return;
    }
    sFuncTables.draw[self->drawConfig](self, play);
}


}
