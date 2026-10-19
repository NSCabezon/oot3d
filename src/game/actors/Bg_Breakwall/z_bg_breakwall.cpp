extern "C" {
#include "z_bg_breakwall.hpp"

#define FLAGS 0x00400010
#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

// Local prototypes for helpers not declared in include/ (3DS signatures)
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Collider_InitQuad(PlayState* play, BgBreakwallQuad* quad);
s32 Collider_SetQuad(PlayState* play, BgBreakwallQuad* dest, Actor* actor, void* src);
void ActorShape_Init(ActorShape* shape, f32 yOffset, ActorShadowFunc shadowDraw, f32 shadowScale);

void BgBreakwall_Init(Actor* thisx, PlayState* play);
void BgBreakwall_Destroy(BgBreakwall* self, PlayState* play);
void BgBreakwall_Update(BgBreakwall* self, PlayState* play);

GLOBAL_ASM("data/z_Bg_Breakwall.data.s")

ActorInit Bg_Breakwall_InitVars = {
    ACTOR_BG_BREAKWALL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgBreakwall),
    (ActorFunc)BgBreakwall_Init,
    (ActorFunc)BgBreakwall_Destroy,
    (ActorFunc)BgBreakwall_Update,
    NULL,
};

// 3DS data symbols (EQU file): dList/collision table, quad init, init chain
GLOBAL_ASM("../src/game/actors/Bg_Breakwall/bw_symbols.s")
extern BombableWallInfo bwBombableWallInfo[];
extern u8 bwQuadInit[];
extern InitChainEntry bwInitChain[];
extern Vec3f bwColQuadList[][4];
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 bankIndex);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_0035479c(BgBreakwallQuad* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d); // Collider_SetQuadVertices
void FUN_0015c004(BgBreakwall* self, PlayState* play); // N64: BgBreakwall_WaitForObject
void FUN_002768c4(BgBreakwall* self, PlayState* play); // N64: BgBreakwall_Draw
void FUN_003f471c(BgBreakwall* self, PlayState* play); // N64: BgBreakwall_Wait
void FUN_00104da8(PlayState* play, BgBreakwall* self, Vec3f* pos, f32 velocity, f32 scaleY, f32 scaleXZ, s32 count,
                  f32 accel); // N64: BgBreakwall_SpawnFragments

void BgBreakwall_Init(Actor* thisx, PlayState* play) {
    BgBreakwall* self = (BgBreakwall*)thisx;
    s32 wallType = PARAMS_GET_U(self->dyna.actor.params, 13, 2);

    Actor_ProcessInitChain(&self->dyna.actor, bwInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    self->dyna.actor.unk_19A = 1;
    self->colType = bwBombableWallInfo[wallType].colType;

    if (self->colType == 1) {
        self->dyna.actor.world.rot.x = 0x4000;
    }

    if (bwBombableWallInfo[wallType].dListIdx >= 0) {
        if (Flags_GetSwitch(play, PARAMS_GET_U(thisx->params, 0, 6))) {
            Actor_Kill(&self->dyna.actor);
            return;
        }

        ActorShape_Init(&self->dyna.actor.shape, 0.0f, NULL, 0.0f);
        Collider_InitQuad(play, &self->collider);
        Collider_SetQuad(play, &self->collider, &self->dyna.actor, bwQuadInit);
    } else {
        thisx->world.pos.y -= 40.0f;
    }

    thisx->unk_19B = 2;

    if (wallType >= 2) {
        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, 0x19);
    } else {
        self->requiredObjectSlot = Object_GetIndex(&play->objectCtx, 0x18A);
    }

    if (self->requiredObjectSlot < 0) {
        Actor_Kill(&self->dyna.actor);
    } else {
        self->actionFunc = (BgBreakwallActionFunc)FUN_0015c004;
    }
}

void BgBreakwall_Destroy(BgBreakwall* self, PlayState* play) {
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(&self->dyna.actor, &self->skelAnimModel, 0);
}

void BgBreakwall_Update(BgBreakwall* self, PlayState* play) {
    self->actionFunc(self, play);
}

#if 0 // draft, 17/85 words differ (fastchk). Model-index ternary must stay inline in the call args (as a local,
      // armcc folds x==1?1:x==2?2:tbl into (x==1||x==2)?x:tbl). Remaining diff: armcc reloads params after the
      // objBankIndex strb; ROM keeps one ldrh and schedules the strb inside the ternary. A `u16 params` local
      // stops the reload but then the ternary lands directly in r3 instead of r0 + mov r3,r0. ~25 variants tried.
      // LavaCover action 0x001EA8C8 is unlabeled (raw address). Still GLOBAL_ASM in functions_140000s.cpp.
// N64: BgBreakwall_WaitForObject. Name pinned to the ld symbol.
void FUN_0015c004(BgBreakwall* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->requiredObjectSlot)) {
        CollisionHeader* colHeader = NULL;
        s32 variant = (self->dyna.actor.params >> 6) & 3;

        self->dyna.actor.objBankIndex = self->requiredObjectSlot;
        s32 wallType = (u8)PARAMS_GET_U(self->dyna.actor.params, 13, 2);
        FUN_00372f38(&self->dyna.actor, play, &self->skelAnimModel,
                                                 (variant == 1)   ? 1
                                                 : (variant == 2) ? 2
                                                                  : bwBombableWallInfo[wallType].dListIdx,
                                                 NULL);
        // N64: Actor_SetObjectDependency(play, actor). Empty on 3DS (see Bg_Haka_Megane).
        ((void (*)(PlayState*, Actor*))GraphicsContext_UnknownStub)(play, &self->dyna.actor);
        self->dyna.actor.flags &= ~0x10;
        self->dyna.actor.draw = (ActorFunc)FUN_002768c4;
        ReadCollisionHeaderFromZsi(
            play->GetObjectStatus(self->dyna.actor.objBankIndex)->zarInfo.FUN_003532c0((u8)bwBombableWallInfo[wallType].colIdx),
            &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);

        if (wallType == 3) {
            self->actionFunc = (BgBreakwallActionFunc)0x001EA8C8; // N64: BgBreakwall_LavaCoverMove (unlabeled)
            self->dyna.actor.flags |= 0x10;
        } else {
            self->actionFunc = FUN_003f471c;
        }
    }
}

#endif

// N64 Matrix_MultVec3f wrapper; (dst, mtx, src) argument order gives the ROM loop scheduling.
static inline void BgBreakwall_MultVec3f(Vec3f* dst, MTX34* mtx, Vec3f* src) {
    VEC3TransformAsm((Vector3f*)dst, mtx, (Vector3f*)src);
}

// N64: BgBreakwall_Draw. Name pinned to the ld symbol.
void FUN_002768c4(BgBreakwall* self, PlayState* play) {
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->FUN_003721e0(&self->dyna.actor.modelMtx);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_00372170(0);

        if (self->colType >= 0) {
            Vec3f colQuad[4];
            Vec3f* src = &bwColQuadList[self->colType][0];
            Vec3f* dst = &colQuad[0];
            s32 i;

            for (i = 0; i < 4; i++) {
                BgBreakwall_MultVec3f(dst++, &self->dyna.actor.modelMtx, src++);
            }

            FUN_0035479c(&self->collider, &colQuad[0], &colQuad[1], &colQuad[2], &colQuad[3]);
            CollisionCheck_SetAC(play, &play->colChkCtx, (Collider*)&self->collider);
        }
    }
}

// N64: BgBreakwall_Wait. Name pinned to the ld symbol.
void FUN_003f471c(BgBreakwall* self, PlayState* play) {
    if (self->collider.unk_00[0x11] & 2) { // collider.base.acFlags & AC_HIT
        Vec3f effectPos;
        s32 wallType = PARAMS_GET_U(self->dyna.actor.params, 13, 2);

        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
        effectPos.y = effectPos.z = effectPos.x = 0.0f;

        if (self->dyna.actor.world.rot.x == 0) {
            effectPos.y = 55.0f;
        } else {
            effectPos.z = 25.0f;
            effectPos.y = -10.0f;
        }

        FUN_00104da8(play, self, &effectPos, 0.0f, 6.4f, 5.0f, 1, 2.0f);
        Flags_SetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 0, 6));

        if (wallType == 2) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x1000159); // NA_SE_EV_EXPLOSION
            self->breakTimer = 60;
        } else {
            Audio_PlayActorSound2(&self->dyna.actor, 0x1000167); // NA_SE_EV_WALL_BROKEN
            self->breakTimer = 90;
        }

        if ((wallType == 0) && !Flags_GetEventChkInf(0xB0)) {
            Flags_SetEventChkInf(0xB0);
            play->FUN_0037573c(play->sceneZARInfo.GetQDBByIndex(0));
            gSaveContext.cutsceneTrigger = 1;
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x1000487); // NA_SE_SY_CORRECT_CHIME
            FUN_0036e980(play, NULL, 0x31);
        }

        if (self->dyna.actor.params < 0) {
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x100048C); // NA_SE_SY_TRE_BOX_APPEAR
        }

        self->actionFunc = (BgBreakwallActionFunc)0x0038227C; // 3DS-only follow-up action (unlabeled)
        self->dyna.actor.draw = NULL;
    }
}
}
