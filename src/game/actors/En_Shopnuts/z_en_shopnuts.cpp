extern "C" {
#include "z_en_shopnuts.hpp"
#include "mtx34_trans.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000005

void EnShopnuts_Init(Actor* thisx, PlayState* play);
void EnShopnuts_Destroy(Actor* thisx, PlayState* play);
void EnShopnuts_Update(Actor* thisx, PlayState* play);
void EnShopnuts_Draw(Actor* thisx, PlayState* play);
s32 EnShopnuts_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnShopnuts_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
void EnShopnuts_SetupWait(EnShopnuts* self);
void EnShopnuts_Wait(EnShopnuts* self, PlayState* play);
void EnShopnuts_Stand(EnShopnuts* self, PlayState* play);
void EnShopnuts_LookAround(EnShopnuts* self, PlayState* play);
void EnShopnuts_ThrowNut(EnShopnuts* self, PlayState* play);
void EnShopnuts_Burrow(EnShopnuts* self, PlayState* play);
void EnShopnuts_SpawnSalesman(EnShopnuts* self, PlayState* play);
void FUN_00258c08(EnShopnuts* self); // N64: EnShopnuts_SetupPeek (own TU: z_en_shopnuts_setup.cpp)

// N64 Actor_SetDropFlag (the header's Actor_SetDropFlag is not this symbol).
void FUN_00375fd0(Actor* actor, void* elem, s32 flag);

void Animation_PlayOnceSetSpeed(SkelAnime* skelAnime, s32 idx, f32 speed);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
void Animation_PlayOnce(SkelAnime* skelAnime, s32 idx);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void FUN_00370084(s16* pValue, s16 target, s16 scale, s16 step); // N64 Math_ApproachS
s32 FUN_003731e0(SkelAnime* skelAnime); // SkelAnime update, returns "animation finished"
f32* EnShopnuts_CurFramePtr(SkelAnime* skelAnime); // own TU: En_Shopnuts/z_en_shopnuts_anim.cpp
void FUN_00353e78(void* zar, PlayState* play, SkelAnime* skel, void* cmb, void* p, s32 b, void* jointTable,
                  void* morphTable, s32 limbCount);
void* ZAR_GetCMBByIndex(void* zar, s32 index);
// 0x00588858 = gSaveContext+0xF00: armcc's shared base for the nearby flag arrays (itemGetInf @+0xF08,
// infTable @+0xF10). Same EQU-based debt as En_Fr.
extern u16 gSaveContext_F00[];

extern s32 EnShopnuts_sAnimIndices[];
extern CollisionCheckInfoInit EnShopnuts_sColChkInfoInit;
extern ColliderCylinderInit EnShopnuts_sCylinderInit;
extern InitChainEntry EnShopnuts_sInitChain[];

GLOBAL_ASM("data/z_En_Shopnuts.data.s")
GLOBAL_ASM("../src/game/actors/En_Shopnuts/data_syms.s")

ActorInit En_Shopnuts_InitVars = {
    ACTOR_EN_SHOPNUTS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SHOPNUTS,
    sizeof(EnShopnuts),
    (ActorFunc)EnShopnuts_Init,
    (ActorFunc)EnShopnuts_Destroy,
    (ActorFunc)EnShopnuts_Update,
    (ActorFunc)EnShopnuts_Draw,
};

GLOBAL_ASM("asm/EnShopnuts_Init.s")

// EnShopnuts_Init: NEAR MISS draft (2026-10-16, DeepSeek) — parked as GLOBAL_ASM.
//
// Approach: twin of the matched EnDns_Init (same 3DS shape: inlined object-bank lookup,
// GetModelManager(), ZAR/SkelAnime init, collider setup, save-flag gating, inlined SetupWait).
// First draft reached 22/133 words differing. Remaining diffs:
//   1. 0xFC / 0x110: the ROM builds &EnShopnuts_sCylinderInit / &EnShopnuts_sColChkInfoInit as
//      `add r3, r6, #0x20` / `add r2, r6, #0x18` (r6 = &EnShopnuts_sAnimIndices, 0x0052EAFC), i.e.
//      armcc knew the compile-time offset between the data symbols. With the current
//      data_syms.s EQU layout armcc emits separate absolute literals. Needs the pair in ONE
//      section / a shared data base -> data-symbol infrastructure outside the actor dir.
//   2. 0x12C-0x174: register swap — ROM has params in r1 + flags base (0x00588858) in r0; the build
//      has them the other way round.
//   3. 0x1B8/0x1BC: `bic r1,r0,#1; strb r1` vs `bic r0,r0,#1; strb r0` — same RMW-destination
//      issue as EnShopnuts_Update (see its note).
//   4. 0x1C-0x24 + the guard/atexit block: fastchk does not apply `armlink --inline` to the empty
//      `__aeabi_atexit` / `__cxa_guard_release` (ROM has `mov r0,r0`), which also shifts the
//      literal-pool offsets; only a real ws_check build resolves those. Same class as En_Si_Init
//      ("needs real shared data symbols / linker pinning outside the actor dir").
#if 0
// 3DS inlines the object-bank lookup (EnDns_GetObjBank twin).
static inline u8* EnShopnuts_GetObjBank(PlayState* play, u32 idx) {
    if (idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void EnShopnuts_Init(Actor* thisx, PlayState* play) {
    EnShopnuts* self = (EnShopnuts*)thisx;
    u8* zar;
    s16 type;
    f32 zero;

    Actor_ProcessInitChain(&self->actor, EnShopnuts_sInitChain);

    zero = 0.0f;
    ActorShape_Init(&self->actor.shape, zero, ActorShadow_DrawCircle, 35.0f);

    zar = EnShopnuts_GetObjBank(play, (u8)self->actor.objBankIndex) + 0x10;
    GetModelManager();
    FUN_00353e78(zar, play, &self->skelAnime, ZAR_GetCMBByIndex(zar, 0), self->actor.unk_178,
                 EnShopnuts_sAnimIndices[5], self->jointTable, self->morphTable, 0xF);

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnShopnuts_sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &EnShopnuts_sColChkInfoInit);
    Collider_UpdateCylinder(&self->actor, &self->collider);

    // DNS_TYPE_HEART_PIECE / DNS_TYPE_DEKU_STICK_UPGRADE / DNS_TYPE_DEKU_NUT_UPGRADE
    // (itemGetInf[0] bit 0x800, infTable[25] bits 4 / 8).
    type = self->actor.params;
    if (((type == 2) && (gSaveContext_F00[4] & 0x800)) || ((type == 9) && (gSaveContext_F00[0x21] & 4)) ||
        ((type == 0xA) && (gSaveContext_F00[0x21] & 8))) {
        Actor_Kill(&self->actor);
    } else {
        // N64 EnShopnuts_SetupIdle, inlined here (it also exists as a standalone function
        // for its other callers, see z_en_shopnuts_setup.cpp).
        Animation_PlayOnceSetSpeed(&self->skelAnime, EnShopnuts_sAnimIndices[4], zero);
        self->animFlagAndTimer = Rand_S16Offset_003702c8(100, 50);
        self->collider.dim.height = zero;
        self->collider.base.acFlags &= ~AC_ON;
        self->actionFunc = EnShopnuts_Wait;
        self->skelAnime.unk_75[0] = 0;
        self->actor.unk_19A = 1;
    }
}
#endif

void EnShopnuts_Destroy(Actor* thisx, PlayState* play) {
    EnShopnuts* self = (EnShopnuts*)thisx;

    SkelAnime_Free2(&self->skelAnime);
    Collider_DestroyCylinder(play, &self->collider);
}

// N64 EnShopnuts_ColliderCheck + EnShopnuts_SetupSpawnSalesman are inlined by the 3DS build
// (no ROM symbols) — see the near-miss draft below.
//
// NEAR MISS (2026-10-16, DeepSeek): 4 of 99 words differ, offsets 0x2c/0x30 and 0x78/0x7c.
// Everything else (prologue, literal pool, branch layout, both inlined setup tails, rg/float
// registers) matches byte-for-byte. The two RMW clears emit
//     bic r0, r0, #K ; strb r0, [r4, #0x1bd]
// where the ROM has
//     bic r1, r0, #K ; strb r1, [r4, #0x1bd]
// (same bases, same instruction count). Tried and rejected (each one build):
//   - `EnShopnuts* s2 = self;` alias at body top / inside the `if` (En_Fd_Fire recipe, CLAUDE.md:208)
//     -> 8 words (load base becomes the alias reg, `bic r1,r1`)
//   - `x = x & ~K` instead of `x &= ~K` -> no change
//   - `u8 flags = self->collider.base.acFlags; x = flags & ~K;` -> 8 words
//   - helper taking `Actor* thisx` + cast inside (CLAUDE.md:84, Bg_Bombwall shape): fixes the
//     first `bic r1,r0` block exactly, but costs sb/sl + a `mov r5,r4` self copy and pushes
//     `play` into r6 -> 71-79 words
//   - single fall-through body with `else if (unk_02 == 0) return;` -> 76 words, literals not hoisted
//   - `(Actor* thisx, PlayState* play2)` + `PlayState* play = play2;` -> no change
//   - `&= (u8)~AC_HIT` -> no change
// N64 EnShopnuts_SetupSpawnSalesman, inlined at both 3DS call sites (no ROM symbol).
static void EnShopnuts_SetupSpawnSalesman(EnShopnuts* self) {
    f32 morph = -3.0f;

    Animation_MorphToPlayOnce(&self->skelAnime, EnShopnuts_sAnimIndices[2], morph);
    Audio_PlayActorSound2(&self->actor, 0x010002F0);
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnShopnuts_SpawnSalesman;
}

// N64 EnShopnuts_ColliderCheck, inlined into Update (no ROM symbol).
static void EnShopnuts_ColliderCheck(EnShopnuts* self, PlayState* play) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        FUN_00375fd0(&self->actor, &self->collider.info, 1);
        EnShopnuts_SetupSpawnSalesman(self);
    } else if (play->actorCtx.unk_02 != 0) {
        EnShopnuts_SetupSpawnSalesman(self);
    }
}

void EnShopnuts_Update(Actor* thisx, PlayState* play) {
    EnShopnuts* self = (EnShopnuts*)thisx;
    EnShopnuts* s2 = self;
    EnShopnuts_ColliderCheck(s2, play);

    self->actionFunc(self, play);

    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, self->collider.dim.radius, self->collider.dim.height, 4);

    if (self->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }

    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);

    if (self->actionFunc == EnShopnuts_Wait) {
        Actor_SetFocus(&self->actor, *EnShopnuts_CurFramePtr(&self->skelAnime));
    } else if (self->actionFunc == EnShopnuts_Burrow) {
        Actor_SetFocus(&self->actor, 20.0f - ((*EnShopnuts_CurFramePtr(&self->skelAnime) * 20.0f) /
                                              (f32)Animation_GetLastFrame(&self->skelAnime,
                                                                          EnShopnuts_sAnimIndices[1])));
    } else {
        Actor_SetFocus(&self->actor, 20.0f);
    }
}



void EnShopnuts_Draw(Actor* thisx, PlayState* play) {
    EnShopnuts* self = (EnShopnuts*)thisx;

    SkelAnime_DrawOpa(&self->skelAnime, &self->actor.modelMtx, (OverrideLimbDrawOpa)EnShopnuts_OverrideLimbDraw,
                      (PostLimbDrawOpa)EnShopnuts_PostLimbDraw, &self->actor, 0);
}

// N64 EnShopnuts_SetupBurrow, inlined at every 3DS call site (no ROM symbol).
static void EnShopnuts_SetupBurrow(EnShopnuts* self) {
    f32 morph = -5.0f;

    Animation_MorphToPlayOnce(&self->skelAnime, EnShopnuts_sAnimIndices[1], morph);
    Audio_PlayActorSound2(&self->actor, 0x010002ED);
    self->actionFunc = EnShopnuts_Burrow;
}

void EnShopnuts_Burrow(EnShopnuts* self, PlayState* play) {
    f32 clamped;

    if (FUN_003731e0(&self->skelAnime)) {
        EnShopnuts_SetupWait(self);
    } else {
        if (*EnShopnuts_CurFramePtr(&self->skelAnime) > 4.0f) {
            clamped = 4.0f;
        } else {
            clamped = *EnShopnuts_CurFramePtr(&self->skelAnime);
        }
        self->collider.dim.height = (4.0f - clamped) * 10.0f;
    }

    if (Animation_OnFrameImpl(&self->skelAnime, 4.0f, 1.0f)) {
        self->collider.base.acFlags &= ~AC_ON;
    }
}

void EnShopnuts_LookAround(EnShopnuts* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);

    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }

    if ((self->actor.xzDistToPlayer < 120.0f) || (self->animFlagAndTimer == 0)) {
        EnShopnuts_SetupBurrow(self);
    }
}

// FUN_00378e60 (limb override; the 3DS moves the N64 PostLimbDraw nose-scale logic here).
s32 EnShopnuts_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnShopnuts* self = (EnShopnuts*)thisx;
    f32 z;
    f32 x;
    f32 y;
    f32 curFrame;
    MTX34 scale;

    if (limbIndex == 0xE) {
        EnShopnutsActionFunc fn = EnShopnuts_ThrowNut;

        if (self->actionFunc != fn) {
            return 0;
        }
        curFrame = *EnShopnuts_CurFramePtr(&self->skelAnime);
        if (curFrame <= 6.0f) {
            x = 1.0f - (curFrame * 0.0833f);
            z = 1.0f + (curFrame * 0.1167f);
            y = z;
        } else if (curFrame <= 7.0f) {
            curFrame -= 6.0f;
            x = 0.5f + curFrame;
            z = 1.7f - (curFrame * 0.7f);
            y = 1.7f - (curFrame * 0.7f);
        } else if (curFrame <= 10.0f) {
            x = 1.5f - ((curFrame - 7.0f) * 0.1667f);
            z = 1.0f;
            y = z;
        } else {
            return 0;
        }

        f32 d0 = y;

        scale.data[0][1] = 0.0f;
        scale.data[0][2] = 0.0f;
        scale.data[0][3] = 0.0f;
        scale.data[1][0] = 0.0f;
        scale.data[1][2] = 0.0f;
        scale.data[1][3] = 0.0f;
        scale.data[2][0] = 0.0f;
        scale.data[2][1] = 0.0f;
        scale.data[2][3] = 0.0f;
        scale.data[0][0] = d0;
        scale.data[1][1] = x;
        scale.data[2][2] = z;
        FUN_0036c174(mtx, mtx, &scale);
    }

    return 0;
}

void EnShopnuts_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
}

void EnShopnuts_SpawnSalesman(EnShopnuts* self, PlayState* play) {
    Actor* spawned;

    if (FUN_003731e0(&self->skelAnime)) {
        spawned = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_DNS, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, self->actor.shape.rot.x, self->actor.shape.rot.y,
                              self->actor.shape.rot.z, SHOPNUTS_GET_TYPE(&self->actor), 1);
        spawned->room = self->actor.room;
        Actor_Kill(&self->actor);
    } else {
        FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    }
}

void EnShopnuts_Stand(EnShopnuts* self, PlayState* play) {
    FUN_003731e0(&self->skelAnime);

    if (Animation_OnFrameImpl(&self->skelAnime, 0.0f, 1.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }

    if (!(self->animFlagAndTimer & 0x1000)) {
        FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    }

    if ((self->actor.xzDistToPlayer < 120.0f) || (self->animFlagAndTimer == 0x1000)) {
        EnShopnuts_SetupBurrow(self);
    } else if (self->animFlagAndTimer == 0) {
        Animation_PlayOnce(&self->skelAnime, EnShopnuts_sAnimIndices[0]);
        self->actionFunc = EnShopnuts_ThrowNut;
    }
}

void EnShopnuts_ThrowNut(EnShopnuts* self, PlayState* play) {
    Vec3f spawnPos;

    FUN_00370084(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);

    if (self->actor.xzDistToPlayer < 120.0f) {
        EnShopnuts_SetupBurrow(self);
    } else if (FUN_003731e0(&self->skelAnime)) {
        FUN_00258c08(self);
    } else if (Animation_OnFrameImpl(&self->skelAnime, 6.0f, 1.0f)) {
        spawnPos.x = self->actor.world.pos.x + (Math_SinS(self->actor.shape.rot.y) * 35.0f);
        spawnPos.y = self->actor.world.pos.y + 12.0f;
        spawnPos.z = self->actor.world.pos.z + (Math_CosS(self->actor.shape.rot.y) * 35.0f);

        // params 2 = EN_NUTSBALL_TYPE_SHOPNUTS.
        if (Actor_Spawn(&play->actorCtx, play, ACTOR_EN_NUTSBALL, spawnPos.x, spawnPos.y, spawnPos.z,
                        self->actor.shape.rot.x, self->actor.shape.rot.y, self->actor.shape.rot.z, 2,
                        1) != NULL) {
            Audio_PlayActorSound2(&self->actor, 0x010002EE);
        }
    }
}

GLOBAL_ASM("asm/EnShopnuts_Wait.s")

}
