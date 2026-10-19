extern "C" {
#include "z_en_fr.hpp"
#include "model_manager.hpp"
#include "mtx34_identity.hpp"

#define FLAGS 0x02000019

void EnFr_Init(Actor* thisx, PlayState* play);
void EnFr_Destroy(Actor* thisx, PlayState* play);
void EnFr_Update(Actor* actor, GameState* state);
void EnFr_SetReward(EnFr* self, PlayState* play);
void FUN_0016c790(EnFr* self, PlayState* play);
void FUN_0014d56c(EnFr* self, PlayState* play);
void FUN_001c23a4(EnFr* self, PlayState* play);
extern u8 En_Fr_sOcarinaNotes[]; // 0x005238EF, EQU in data_syms.s
extern u8* gUnk_0054C8C0;        // 0x0054C8C0 (pointer variable), EQU in src/game/shared_syms.s
extern u8 En_Fr_sJumpOrder[];    // 0x005238FC, EQU in data_syms.s (FROG_* order for ChildSong jumps)
void FUN_0019062c(EnFr* self, PlayState* play);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);
void FUN_00230a00(EnFr* self, PlayState* play);
void FUN_00190550(EnFr* self, PlayState* play);
void FUN_003811dc(EnFr* self, PlayState* play);
void FUN_001905d4(EnFr* self, PlayState* play);
void FUN_003725e0(PlayState* play);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* node);
s32 GetMessageMode(PlayState* play);
s32 FUN_0034eaf8(void* collider);
void FUN_00370778(PlayState* play);
s32 FUN_002386c4(PlayState* play);
void FUN_00372244(void* arg0, s16 arg1, u32 sfxId); // Sfx_PlaySfxCentered
void FUN_003523dc(s32 instrument); // AudioOcarina_SetInstrument
extern s8 En_Fr_sUnk5238D0; // 0x005238D0, EQU in data_syms.s
extern s32 En_Fr_sSongToFrog[]; // 0x00523914, EQU in data_syms.s
extern u16 En_Fr_sFrogSongFlags[]; // 0x00523904, EQU in data_syms.s
typedef struct LogSpotToFromWater {
    /* 0x0 */ f32 xzDist;
    /* 0x4 */ f32 yaw;
    /* 0x8 */ f32 yDist;
} LogSpotToFromWater;
extern LogSpotToFromWater En_Fr_sLogSpotToFromWater[]; // 0x00523978, EQU in data_syms.s
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_003cebe4(EnFr* self, PlayState* play);
void FUN_003b36a8(EnFr* self, PlayState* play);

GLOBAL_ASM("data/z_En_Fr.data.s")

GLOBAL_ASM("../src/game/actors/En_Fr/data_syms.s")

typedef struct EnFrPointers {
    u8 flags;
    EnFr* frogs[5];
} EnFrPointers;

// sEnFrPointers (0x00599110) has no scatter symbol: absolute EQU in data_syms.s.
extern EnFrPointers En_Fr_sEnFrPointers;
#define sEnFrPointers En_Fr_sEnFrPointers
extern u16 gSaveContext_F00[]; // 0x00588858 = gSaveContext+0xF00 (repo SaveContext puts eventChkInf at 0xEEC)

void FUN_0037073c(PlayState* play, u16 action); // Message_StartOcarina
void FUN_001135a0(EnFr* self, PlayState* play);
void FUN_00230a54(EnFr* self, PlayState* play);
void En_Fr_DecrementBlinkTimer(EnFr* self); // 0x0012FFC4, unlabeled code (data_syms.s EQU)

ActorInit En_Fr_InitVars = {
    ACTOR_EN_FR,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_FR,
    sizeof(EnFr),
    (ActorFunc)EnFr_Init,
    (ActorFunc)EnFr_Destroy,
    (ActorFunc)EnFr_Update,
    NULL,
};

void FUN_00112fcc(Actor* thisx, PlayState* play) {
}

void FUN_002919d8(EnFr* self, PlayState* play) {
    self->unk_C28++;
    self->actionFunc(self, play);
}

void FUN_0026f2dc(EnFr* self, PlayState* play) {
    FUN_00367c7c(play, self->actor.textId, &self->actor);
    self->actionFunc = FUN_001905d4;
}

void FUN_0026f230(EnFr* self, PlayState* play) {
    s32 i;

    self->unk_E4C = (s8)(self->unk_E4C + 1);
    if (self->unk_E4C > En_Fr_sUnk5238D0) {
        self->actor.textId = 0x40AC;
        for (i = 0; i < 5; i++) {
            EnFr* frog = sEnFrPointers.frogs[i];
            frog->isButterflyDrawn = false;
        }
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
        FUN_003523dc(0);
        FUN_00370778(play);
        self->actionFunc = FUN_0026f2dc;
    }
}

void FUN_0016c790(EnFr* self, PlayState* play) {
    s32 i;

    if (self->unusedButterflyActor != NULL) {
        self->unusedButterflyActor->freezeTimer = 0xF;
    }
    for (i = 0; i < 5; i++) {
        EnFr* frog = sEnFrPointers.frogs[i];

        if ((frog == NULL) || (frog->isDeactivating != 1)) {
            return;
        }
    }
    for (i = 0; i < 5; i++) {
        EnFr* frog = sEnFrPointers.frogs[i];

        if (frog == NULL) {
            return;
        }
        frog->isDeactivating = 0;
    }
    *(u16*)((u8*)play + 0x2B7E) = 4;
    Audio_PlayActorSound2(&self->actor, 0x0100023A);
    if (self->getItemId == 0) {
        self->actionFunc = FUN_0019062c;
    } else {
        self->actionFunc = FUN_00230a00;
        GiveItem(&self->actor, play, self->getItemId, 30.0f, 100.0f);
    }
}

// 0 words in C (fastchk) but NOT enabled: ROM function is 276 B = 260 B of code + 16 B unlabeled tail after the pool
// (`mov r1,r0; ldr r0,[r0,#0x18]; ldr r1,[r1,#0x1c]; bx lr`). Needs cfg_split at 0x00190730 (orchestrator), then
// enable_draft.py. Float compare = `d = x*x+z*z; yd = py - sy;` BEFORE `if (d > 900.0f) return;`.
void FUN_0019062c(EnFr* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (player->stateFlags2 & 0x2000000) {
        if (*(u16*)((u8*)play + 0x2B7E) == 4) {
            *(u16*)((u8*)play + 0x2B7E) = 0;
        }
        OnePointCutscene_Init(play, 4110, ~0x62, &self->actor, 0);
        FUN_00370778(play);
        player->actor.world.pos.x = self->actor.world.pos.x;
        player->actor.world.pos.y = self->actor.world.pos.y;
        player->actor.world.pos.z = self->actor.world.pos.z;
        {
            s16 yaw = self->actor.world.rot.y;

            player->actor.shape.rot.y = yaw;
            player->actor.world.rot.y = yaw;
            *(s16*)((u8*)player + 0x2220) = yaw;
        }
        self->actionFunc = FUN_0014d56c;
        self->getItemId = 0;
    } else {
        f32 xd = player->actor.world.pos.x - self->actor.world.pos.x;
        f32 zd = player->actor.world.pos.z - self->actor.world.pos.z;
        f32 d = xd * xd + zd * zd;
        f32 yd = player->actor.world.pos.y - self->actor.world.pos.y;
        if (d > 900.0f) { return; }
        if (yd >= 0.0f) {
            *(Actor**)((u8*)player + 0x1740) = &self->actor;
        }
    }
}

void FUN_001135a0(EnFr* self, PlayState* play) {
    if (sEnFrPointers.flags == 12) {
        self->actor.world.rot.y = (s16)(En_Fr_sLogSpotToFromWater[self->actor.params].yaw * (0x8000 / M_PI));
        Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f,
                                  (f32)Animation_GetLastFrame(&self->skelAnime, 0), 2, 0.0f);
        self->unk_C41 = 0;
        self->actionFunc = FUN_003cebe4;
    } else if (self->unk_C35 != 0) {
        Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f,
                                  (f32)Animation_GetLastFrame(&self->skelAnime, 0), 2, 0.0f);
        self->unk_C41 = 0;
        self->actionFunc = FUN_003b36a8;
    }
}

void FUN_003811dc(EnFr* self, PlayState* play) {
    s32 i;
    s32 frogIdx;
    s32 idx;
    u8 note;
    u16* ev;
    u8* notes;

    if (self->unk_C42 != 0) {
        self->unk_C42--;
        return;
    }
    self->unk_C42 = 90;
    self->unk_C3C = 0;
    FUN_0037073c(play, 0x2F);
    idx = self->unk_C3C;
    ev = gSaveContext_F00;
    if (ev[3] & 1) {
        idx = (s32)Rand_ZeroFloat(60.0f) % 5;
        notes = En_Fr_sOcarinaNotes;
    } else {
        notes = gUnk_0054C8C0;
    }
    note = notes[idx];
    self->unk_C3D = note;
    frogIdx = 0;
    if (gSaveContext.masterQuestFlag != 0) {
        switch (note) {
            case 4:
                frogIdx = 1;
                break;
            case 2:
                frogIdx = 2;
                break;
            case 1:
                frogIdx = 3;
                break;
            case 0:
                frogIdx = 4;
                break;
            case 3:
                frogIdx = 0;
                break;
        }
    } else {
        switch (note) {
            case 2:
                frogIdx = 1;
                break;
            case 4:
                frogIdx = 2;
                break;
            case 0:
                frogIdx = 3;
                break;
            case 1:
                frogIdx = 4;
                break;
            case 3:
                frogIdx = 0;
                break;
        }
    }
    for (i = 0; i < 5; i++) {
        EnFr* frog = sEnFrPointers.frogs[i];
        if (i != frogIdx) {
            frog->isButterflyDrawn = 0;
        }
        if (i == frogIdx) {
            frog->isButterflyDrawn = 1;
        }
    }
    self->actionFunc = FUN_001c23a4;
}

void FUN_003b36a8(EnFr* self, PlayState* play) {
    f32 yDistToLogSpot;

    u8 st;

    if (self->unk_C41 == 0) goto s0;
    if (self->unk_C41 == 1) {
        if (self->skelAnime.curFrame >= 9.0f) {
            self->skelAnime.playSpeed = 0.0f;
            st = 2;
            goto set;
        }
    }
    goto lnd;
set:
    self->unk_C41 = st;
lnd:
    f32 yLogSpot = self->unk_C4C;
    yDistToLogSpot = self->actor.world.pos.y - yLogSpot;
    if ((self->actor.velocity.y < 0.0f) && (yDistToLogSpot <= 0.0f)) {
        self->actor.world.pos.y = self->unk_C4C;
        self->actor.velocity.y = 0.0f;
        self->unk_C35 = 0;
        self->actor.gravity = 0.0f;
        Animation_Change_00375c08(&self->skelAnime, 1, 1.0f, 0.0f,
                                  (f32)Animation_GetLastFrame(&self->skelAnime, 1), 0, 0.0f);
        self->actionFunc = FUN_001135a0;
        return;
    }
    goto el;
s0:
    if (self->skelAnime.curFrame >= 4.5f) {
        self->actor.gravity = -10.0f;
        self->actor.velocity.y = 25.0f;
        if (self->unk_C3F != 0) {
            self->unk_C3F = 0;
            Audio_PlayActorSound2(&self->actor, 0x01000296);
        }
        st = 1;
        goto set;
    }
    goto lnd;
el:
    if ((self->actor.velocity.y <= 0.0f) && (yDistToLogSpot < 40.0f)) {
        self->skelAnime.playSpeed = 1.0f;
    }
}

void EnFr_SetReward(EnFr* self, PlayState* play) {
    u16 songIndex;
    u16* ev;
    u16* tbl;
    EnFr* s2 = self;

    sEnFrPointers.flags = 12;
    self->actionFunc = FUN_0016c790;
    self->getItemId = GI_NONE;
    songIndex = self->songIndex;
    ev = gSaveContext_F00;
    if (songIndex <= 4) {
        tbl = En_Fr_sFrogSongFlags;
        if (!(ev[3] & tbl[songIndex])) {
            ev[3] |= tbl[songIndex];
            s2->getItemId = GI_RUPEE_PURPLE;
        } else {
            self->getItemId = GI_RUPEE_BLUE;
        }
    } else if (songIndex == 5) {
        tbl = En_Fr_sFrogSongFlags;
        if (!(ev[3] & tbl[5])) {
            ev[3] |= tbl[5];
            self->getItemId = GI_HEART_PIECE;
        } else {
            self->getItemId = GI_RUPEE_BLUE;
        }
    } else if (songIndex == 6) {
        tbl = En_Fr_sFrogSongFlags;
        if (!(ev[3] & tbl[6])) {
            ev[3] |= tbl[6];
            self->getItemId = GI_HEART_PIECE;
        } else {
            self->getItemId = GI_RUPEE_PURPLE;
        }
    }
}

void FUN_00393e60(EnFr* self, PlayState* play) {
    s32 i;
    EnFr* frog = sEnFrPointers.frogs[En_Fr_sSongToFrog[self->songIndex]];

    if (frog->isActive == 0) {
        self->actor.textId = 0x40A9;
        for (i = 0; i < 5; i++) {
            EnFr* frog2 = sEnFrPointers.frogs[i];
            frog2->isButterflyDrawn = false;
        }
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
        FUN_003523dc(0);
        FUN_00370778(play);
        self->actionFunc = FUN_0026f2dc;
    }
}

void EnFr_Destroy(Actor* thisx, PlayState* play) {
    EnFr* self = (EnFr*)thisx;

    LightContext_RemoveLight(play, &play->unk_A66[0xA70 - 0xA66], self->lightNode);
    SkelAnime_Free2(&self->skelAnime);
    SkelAnime_Free2(&self->skelAnimeButterfly);
    ((FaceAnimations*)((u8*)self + 0xC80))->Destroy(); // ROM tail: pop; falls into next fn (tail-reorder)
}


void FUN_00230a00(EnFr* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actor.parent = NULL;
        self->actionFunc = (void (*)(EnFr*, PlayState*))0x0026F1F4; // debt: unlabeled code (no symbol)
        return;
    }
    GiveItem(&self->actor, play, self->getItemId, 30.0f, 100.0f);
}

void FUN_001905d4(EnFr* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5) {
        if (Message_ShouldAdvance(play)) {
            self->unk_C42 = 150;
            FUN_003725e0(play);
            self->actionFunc = EnFr_SetReward;
        }
    }
}

void FUN_002b6ac0(EnFr* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 5) {
        if (Message_ShouldAdvance(play)) {
            FUN_003725e0(play);
            self->unk_C42 = 3;
            self->actionFunc = FUN_003811dc;
        }
    }
}

void FUN_0039bbcc(EnFr* self, PlayState* play) {
    s32 frogIndex;

    for (frogIndex = 0; frogIndex < 5; frogIndex++) {
        EnFr* frog = sEnFrPointers.frogs[frogIndex];
        if (frog != NULL && frog->actionFunc == FUN_001135a0) {
            continue;
        } else {
            return;
        }
    }
    FUN_0037073c(play, 0x30);
    self->actionFunc = FUN_00230a54;
}

void FUN_00389714(EnFr* self, PlayState* play);
s32 FUN_00340b34(EnFr* self, s32 idx);

void FUN_00230a54(EnFr* self, PlayState* play) {
    s32 i;

    self->songIndex = 7;
    switch (*(u16*)((u8*)play + 0x2B7E)) {
        case 7:
            self->songIndex = 0;
            break;
        case 6:
            self->songIndex = 1;
            break;
        case 5:
            self->songIndex = 2;
            break;
        case 8:
            self->songIndex = 3;
            break;
        case 9:
            self->songIndex = 4;
            break;
        case 10:
            self->songIndex = 5;
            break;
        case 4:
            FUN_003725e0(play);
            self->getItemId = 0;
            Audio_PlaySoundGeneral(0x010004AC, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            FUN_003523dc(0);
            sEnFrPointers.flags = 12;
            for (i = 0; i < 5; i++) {
                EnFr* frog = sEnFrPointers.frogs[i];
                frog->isButterflyDrawn = false;
            }
            self->actionFunc = FUN_0016c790;
            break;
        case 1:
            switch (gSaveContext.masterQuestFlag) {
                default:
                    switch (*(u8*)((u8*)play + 0x2BA0)) {
                        case 2:
                            FUN_00340b34(self, 2);
                            break;
                        case 3:
                            FUN_00340b34(self, 0);
                            break;
                        case 4:
                            FUN_00340b34(self, 1);
                            break;
                        case 0:
                            FUN_00340b34(self, 4);
                            break;
                        case 1:
                            FUN_00340b34(self, 3);
                            break;
                    }
                    break;
                case 0:
                    switch (*(u8*)((u8*)play + 0x2BA0)) {
                        case 4:
                            FUN_00340b34(self, 2);
                            break;
                        case 3:
                            FUN_00340b34(self, 0);
                            break;
                        case 2:
                            FUN_00340b34(self, 1);
                            break;
                        case 1:
                            FUN_00340b34(self, 4);
                            break;
                        case 0:
                            FUN_00340b34(self, 3);
                            break;
                    }
                    break;
            }
            break;
    }
    if (self->songIndex != 7) {
        self->unk_C28 = 0;
        self->actionFunc = FUN_00389714;
    }
}

// FUN_003cebe4 = N64 EnFr_JumpingUp (state machine on unk_C41). goto layout puts the state-0 block after the
// landing code; `SkelAnime* sa` alias stops if-conversion of the state-1 block; dead `EnFr* s2` alias on the
// landing velocity test fixes the prologue table-literal hoist (R11 near-miss generator).
void FUN_0034251c(EnFr* self);
void FUN_003b3898(EnFr* self, PlayState* play);
void FUN_001b09ac(EnFr* self, PlayState* play);

void FUN_003cebe4(EnFr* self, PlayState* play) {
    f32 yTarget = self->unk_C4C + En_Fr_sLogSpotToFromWater[self->actor.params].yDist;

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.world.rot.y, 2, 10000, 100);

    if (self->unk_C41 == 0) goto s0;
    if (self->unk_C41 == 1) {
        if (self->skelAnime.curFrame >= 6.0f) {
            self->unk_C41 = 2;
            SkelAnime* sa = &self->skelAnime;
            sa->playSpeed = 0.0f;
        }
    }
lnd:
    EnFr* s2 = self;
    if (s2->actor.velocity.y < 0.0f) {
        if (self->actor.world.pos.y < yTarget) {
            Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f,
                                      (f32)Animation_GetLastFrame(&self->skelAnime, 2), 0, 0.0f);
            self->actionFunc = FUN_003b3898;
            self->actor.draw = (self->actor.params == 4) ? (ActorFunc)FUN_001b09ac : NULL;
            self->isDeactivating = 1;
            FUN_0034251c(self);
        }
    }
    return;
s0:
    if (self->skelAnime.curFrame >= 3.0f) {
        self->actor.speedXZ = 6.0f;
        self->actor.gravity = -10.0f;
        self->actor.velocity.y = 25.0f;
        self->unk_C41 = 1;
    }
    goto lnd;
}

s32 FUN_00340b34(EnFr* self, s32 idx);

void FUN_00389714(EnFr* self, PlayState* play) {
    s32 i;

    if (self->unk_C28 < 0x30) {
        if ((self->unk_C28 & 3) == 0) {
            FUN_00340b34(self, En_Fr_sJumpOrder[(self->unk_C28 >> 2) & 7]);
        }
        return;
    }
    u32 sfx = 0x01000487;
    void (*next)(EnFr*, PlayState*) = FUN_0026f2dc;

    if (self->songIndex == 5) {
        self->actor.textId = 0x40AA;
        for (i = 0; i < 5; i++) {
            EnFr* frog = sEnFrPointers.frogs[i];
            frog->isButterflyDrawn = false;
        }
        FUN_00372244(&play->unk_5FCC, 0x1E, sfx);
    tail:
        FUN_003523dc(0);
        FUN_00370778(play);
        self->actionFunc = next;
        return;
    }
    if (!(gSaveContext_F00[3] & En_Fr_sFrogSongFlags[self->songIndex])) {
        EnFr* frog = sEnFrPointers.frogs[En_Fr_sSongToFrog[self->songIndex]];

        FUN_00372244(&play->unk_5FCC, 0x1E, sfx);
        if (frog->actionFunc != FUN_001135a0) {
            self->unk_C28 = 0x30;
            return;
        }
        frog->unk_C35 = 1;
        frog->isActive = 1;
        Audio_PlayActorSound2(&frog->actor, 0x01000223);
        self->actionFunc = FUN_00393e60;
    } else {
        self->actor.textId = 0x40A9;
        for (i = 0; i < 5; i++) {
            EnFr* frog = sEnFrPointers.frogs[i];
            frog->isButterflyDrawn = false;
        }
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
        goto tail;
    }
}

void Matrix_RotateY(MTX34* mtx, f32 angle, s32 mode);
void FUN_003b3800(EnFr* self, PlayState* play);

// N64 Matrix_MultVec3f argument order (src, dest); keeps the ROM's mtx/src/dest register setup order.
static inline void EnFr_MultVec3f(Vec3f* src, Vec3f* dest, MTX34* mtx) {
    VEC3TransformAsm((Vector3f*)dest, mtx, (Vector3f*)src);
}

void FUN_00102ccc(EnFr* self, PlayState* play) {
    Vec3f vec1;
    Vec3f vec2;

    if (self->unk_C41 == 0) goto s0;
    if (self->unk_C41 == 1) {
        if (Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
            self->unk_C41 = 2;
            sEnFrPointers.flags++;
            self->skelAnime.playSpeed = 0.0f;
        }
    }
lnd:
    vec1.x = vec1.y = 0.0f;
    vec1.z = self->unk_C54;
    Matrix_RotateY(&self->actor.modelMtx, (f32)(self->actor.world.rot.y + 0x8000) * (1.0f / 0x8000) * M_PI, 0);
    EnFr_MultVec3f(&vec1, &vec2, &self->actor.modelMtx);
    self->actor.world.pos.x = self->posLogSpotX + vec2.x;
    self->actor.world.pos.z = self->posLogSpotZ + vec2.z;
    if (self->skelAnime.curFrame >= 3.0f) {
        Math_ApproachF(&self->unk_C54, 0.0f, 1.0f, 10.0f);
    }

    vec2.y = self->actor.world.pos.y - self->unk_C4C;
    if ((self->actor.velocity.y < 0.0f) && (vec2.y <= 0.0f)) {
        { EnFr* s2 = self; s2->actor.world.pos.y = self->unk_C4C; }
        self->actor.velocity.y = 0.0f;
        self->actor.gravity = 0.0f;
        self->actionFunc = FUN_003b3800;
        self->unk_C58 = 0.0f;
    }
    if ((self->actor.velocity.y <= 0.0f) && (vec2.y < 40.0f)) {
        self->skelAnime.playSpeed = 1.0f;
    }
    return;
s0:
    if (Animation_OnFrameImpl(&self->skelAnime, 3.0f, 1.0f)) {
        self->actor.speedXZ = 0.0f;
        self->actor.gravity = -10.0f;
        self->actor.velocity.y = 47.0f;
        self->unk_C41 = 1;
        sEnFrPointers.flags++;
    }
    goto lnd;
}

void FUN_00113698(EnFr* self) {
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    } else if (self->eyeTexIndex) {
        self->eyeTexIndex = 0;
        self->blinkTimer = (s16)(Rand_ZeroFloat(60.0f) + 20.0f);
        self->blinkFunc = En_Fr_DecrementBlinkTimer;
    } else {
        self->eyeTexIndex = 1;
        self->blinkTimer = 1;
    }
}

void FUN_00190550(EnFr* self, PlayState* play) {
    if (sEnFrPointers.flags == 11) {
        u16* ev = gSaveContext_F00;
        if ((ev[3] & 0x2) && (ev[3] & 0x4) && (ev[3] & 0x10) && (ev[3] & 0x8) && (ev[3] & 0x20) && (ev[3] & 0x40)) {
            self->actionFunc = FUN_002b6ac0;
            self->songIndex = 6;
            FUN_00367c7c(play, 0x40AB, &self->actor);
        } else {
            self->songIndex = 0;
            self->actionFunc = FUN_0039bbcc;
        }
    }
}

extern u16 gSaveContext_F00[]; // 0x00588858 = gSaveContext+0xF00 (repo SaveContext puts eventChkInf at 0xEEC: mismatch)
extern u8 En_Fr_sTimerJumpingOutOfWater[];
extern u8 En_Fr_sPitchTbl0[];
extern u8 En_Fr_sPitchTbl1[];
extern u8 En_Fr_sPitchTbl2[];
extern u8 En_Fr_sPitchTbl3[];
void FUN_001b09ac(EnFr* self, PlayState* play);
void FUN_00102ccc(EnFr* self, PlayState* play);
void FUN_0035e5b4(PosRot* pos, u32 sfxId, s8 pitch);
s16 Math_SmoothStepToS(s16* pValue, s16 target, s16 scale, s16 step, s16 minStep);

// FUN_002386c4 lives in z_en_fr_mm.cpp (own TU: in this file armcc inlines it into FUN_0014d56c).

void FUN_003b3898(EnFr* self, PlayState* play) {
    if (sEnFrPointers.flags == *(u8*)((s32)self->actor.params + (s32)En_Fr_sTimerJumpingOutOfWater)) {
        Animation_Change_00375c08(&self->skelAnime, 0, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 0), 2, 0.0f);
        self->unk_C41 = 0;
        self->actor.draw = (ActorFunc)FUN_001b09ac;
        self->actionFunc = FUN_00102ccc;
    }
}

// = N64 EnFr_OrientOnLogSpot (3DS drops the flags++). The SkelAnime* local inside the rotYRemaining test keeps
// the float compare as a real branch (without it armcc if-converts it into vldreq/vcmpeq).
void FUN_003b3800(EnFr* self, PlayState* play) {
    s16 rotYRemaining = Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 10000, 100);

    self->actor.world.rot.y = self->actor.shape.rot.y;

    if (rotYRemaining == 0) {
        SkelAnime* sa = &self->skelAnime;

        if (sa->curFrame == sa->endFrame) {
            self->actionFunc = FUN_001135a0;
            Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 2), 0,
                                      0.0f);
        }
    }
}

// ROM places the save==0 arm after the shared join (goto layout).
s32 FUN_00340b34(EnFr* self, s32 idx) {
    EnFr* frog = sEnFrPointers.frogs[idx];
    u8* table;
    u8 pitch;

    if ((frog != NULL) && (frog->unk_C35 == 0)) {
        if (gSaveContext.masterQuestFlag == 0) {
            goto nonmq;
        }
        if (frog->unk_C3A == 3) {
            table = En_Fr_sPitchTbl3;
        } else {
            table = En_Fr_sPitchTbl2;
        }
join:
        pitch = table[idx];
        if (self->songIndex == 6) {
            frog->unk_C3F = 1;
        }
        frog->unk_C35 = 1;
        FUN_0035e5b4(&frog->actor.world, 0x01000208, (s8)pitch);
        return 1;
nonmq:
        if (frog->unk_C3A == 3) {
            table = En_Fr_sPitchTbl1;
        } else {
            table = En_Fr_sPitchTbl0;
        }
        goto join;
    }
    return 0;
}

void FUN_0014d56c(EnFr* self, PlayState* play) {
    if (GetMessageMode(play) == 0xC) {
        FUN_00370778(play);
        sEnFrPointers.flags = 1;
        self->actionFunc = FUN_00190550;
    } else {
        if (FUN_002386c4(play) != 0) {
            sEnFrPointers.flags = 1;
            self->actionFunc = FUN_00190550;
        }
    }
}

void EnFr_Init(Actor* thisx, PlayState* play) {
    EnFr* self = (EnFr*)thisx;

    if (thisx->params == 0) {
        thisx->destroy = NULL;
        thisx->draw = NULL;
        thisx->update = (ActorFunc)FUN_002919d8;
        self->actor.flags &= ~0x11;
        Actor_ChangeType(play, &play->actorCtx, thisx, 6);
        self->actor.textId = 0x40AC;
        self->actionFunc = FUN_0019062c;
    }
}

GLOBAL_ASM("asm/EnFr_Update.s")

// DRAFT (NOT compiled): 17 words off in fastchk, ONLY the inlined identity-MTX34 init (VFP copy numbering of the
// 1.0/0.0 ctor args; the same include/mtx34_identity.hpp ctor matches the out-of-line MTX34Identity). Rest matches.
#if 0
// N64 EnFr_OrientUnderwater. Matrix_RotateY(MTXMODE_NEW) starts from the shared identity MTX34 static
// (include/mtx34_identity.hpp: guard 0x0055A210 / object 0x005BE29C, pinned in data/oot3d.ld).
void FUN_0034251c(EnFr* self) {
    Vec3f vec1;
    Vec3f vec2;
    MTX34 mtx;

    vec1.x = vec1.y = 0.0f;
    vec1.z = self->unk_C54 = En_Fr_sLogSpotToFromWater[self->actor.params].xzDist;
    MTX34CopyAsm(&mtx, (MTX34*)&MTX34_Identity());
    Matrix_RotateY(&mtx, En_Fr_sLogSpotToFromWater[self->actor.params].yaw, 0);
    EnFr_MultVec3f(&vec1, &vec2, &mtx);
    self->actor.world.pos.x = self->posLogSpotX + vec2.x;
    self->actor.world.pos.z = self->posLogSpotZ + vec2.z;
    self->actor.world.pos.y = self->unk_C4C + En_Fr_sLogSpotToFromWater[self->actor.params].yDist;
    self->actor.world.rot.y = self->actor.shape.rot.y =
        (s16)(En_Fr_sLogSpotToFromWater[self->actor.params].yaw * (0x8000 / M_PI)) + 0x8000;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.speedXZ = 0.0f;
}
#endif
GLOBAL_ASM("asm/FUN_0034251c.s")

}
