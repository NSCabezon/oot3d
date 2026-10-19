extern "C" {
#include "z_bg_mori_hineri.hpp"

#define FLAGS 0x00400030
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
void FUN_0032b13c(PlayState* play, s32 arg1);
void FUN_00357248(SkeletonAnimationModel** model);
SkeletonAnimationModel* FUN_0036a924(Actor* actor, PlayState* play, s16 objectSlot, s32 arg3);
void FUN_00154dac(BgMoriHineri* self, PlayState* play);
void FUN_00174e04(BgMoriHineri* self, PlayState* play);
void FUN_00372244(void* arg0, s16 arg1, u32 sfxId);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);
void OnePointCutscene_EndCutscene(PlayState* play, s32 camId);
void FUN_0029d22c(Actor* thisx, GameState* state);
#define CAMERA_CHILD_CAM_ID(cam) (*(s16*)((u8*)(cam) + 0x196))
#define CAMERA_SETTING(cam) (*(u16*)((u8*)(cam) + 0x18A))
#define CAM_SET_CS_TWISTED_HALLWAY 0x26
void FUN_0035ae08(Actor* actor, u32 sfxId);
s16 Camera_ChangeSetting(Camera* camera, s16 setting);
void FUN_001280fc(BgMoriHineri* self, PlayState* play);
void FUN_001e9198(BgMoriHineri* self, PlayState* play);
void FUN_003e7f14(BgMoriHineri* self, PlayState* play);
void FUN_00174cb0(BgMoriHineri* self, PlayState* play);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0037266c(SkeletonAnimationModel* model, s32 idx);

/* sSubCamId: no scatter symbol exists; defined as an absolute symbol (EQU 0x00513328) in sBgMoriHineriSubCamId.s */
extern s16 sBgMoriHineriSubCamId;
#define CAM_ID_NONE (-1)
GLOBAL_ASM("../src/game/actors/Bg_Mori_Hineri/sBgMoriHineriSubCamId.s")

void BgMoriHineri_Init(Actor* thisx, GameState* state);
void BgMoriHineri_Destroy(Actor* thisx, GameState* state);
void BgMoriHineri_Update(Actor* thisx, GameState* state);

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

ActorInit Bg_Mori_Hineri_InitVars = {
    ACTOR_BG_MORI_HINERI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgMoriHineri),
    (ActorFunc)BgMoriHineri_Init,
    (ActorFunc)BgMoriHineri_Destroy,
    (ActorFunc)BgMoriHineri_Update,
    NULL,
};

void BgMoriHineri_Init(Actor* thisx, GameState* state) {
    BgMoriHineri* self = (BgMoriHineri*)thisx;
    PlayState* play = (PlayState*)state;
    s32 switchFlagParam;
    s32 t6;
    s16 params;

    Actor_ProcessInitChain(thisx, sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);

    params = thisx->params;
    switchFlagParam = PARAMS_GET_U(params, 0, 6);
    t6 = params & 0x4000;
    if (t6 != 0) {
        self->switchFlag = switchFlagParam;
    } else {
        self->switchFlag = PARAMS_GET_U(params, 8, 6);
        self->switchFlag = (Flags_GetSwitch(play, self->switchFlag)) ? 1 : 0;
    }
    {
        u16 tw = (u16)thisx->params;
        thisx->params = (tw & 0x8000) >> 14;
    }
    if (Flags_GetSwitch(play, switchFlagParam)) {
        if (thisx->params == 0) {
            thisx->params = 1;
        } else if (thisx->params == 2) {
            thisx->params = 3;
        }
    }
    self->boxObjectSlot = -1;
    if (thisx->params == 0) {
        self->moriHineriObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI1);
        self->altObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI1A);
        if (t6 == 0) {
            self->boxObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_BOX);
        }
    } else if (thisx->params == 1) {
        self->moriHineriObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI1A);
        self->altObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI1);
    } else if (thisx->params == 2) {
        self->moriHineriObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI2);
        self->altObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI2A);
    } else {
        self->moriHineriObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI2A);
        self->altObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI2);
    }
    if (t6 != 0) {
        self->dyna.actor.params += 4;
    }
    if (self->moriHineriObjectSlot >= 0) {
        self->actionFunc = FUN_00154dac;
    } else {
        Actor_Kill(&self->dyna.actor);
    }
    if (*(u8*)((u8*)play + 0x3235) == 0) {
        FUN_0032b13c(play, 0xC);
    }
}


void FUN_00154dac(BgMoriHineri* self, PlayState* play) {
    CollisionHeader* colHeader;
    s32 obj0, idx0, obj1, idx1;

    if (Object_IsLoaded(&play->objectCtx, self->moriHineriObjectSlot) &&
        Object_IsLoaded(&play->objectCtx, self->altObjectSlot) &&
        ((self->boxObjectSlot < 0) || Object_IsLoaded(&play->objectCtx, self->boxObjectSlot))) {
        self->dyna.actor.objBankIndex = self->moriHineriObjectSlot;
        if (self->dyna.actor.params >= 4) {
            self->dyna.actor.params -= 4;
            if (self->dyna.actor.params == 0) {
                self->moriHineriObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI1A);
                idx0 = 0; obj0 = OBJECT_MORI_HINERI1; idx1 = 0; obj1 = OBJECT_MORI_HINERI1A;
            } else if (self->dyna.actor.params == 1) {
                self->moriHineriObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI1);
                idx0 = 0; obj0 = OBJECT_MORI_HINERI1A; idx1 = 0; obj1 = OBJECT_MORI_HINERI1;
            } else if (self->dyna.actor.params == 2) {
                self->moriHineriObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI2A);
                idx0 = 0; obj0 = OBJECT_MORI_HINERI2; idx1 = 0; obj1 = OBJECT_MORI_HINERI2A;
            } else {
                self->moriHineriObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_HINERI2);
                idx0 = 0; obj0 = OBJECT_MORI_HINERI2A; idx1 = 0; obj1 = OBJECT_MORI_HINERI2;
            }
            if (self->moriHineriObjectSlot < 0) {
                Actor_Kill(&self->dyna.actor);
            } else {
                self->actionFunc = FUN_00174e04;
            }
            FUN_00372f38(&self->dyna.actor, play, NULL);
            self->skelAnimModel0 = FUN_0036a924(&self->dyna.actor, play, obj0, idx0);
            self->skelAnimModel1 = FUN_0036a924(&self->dyna.actor, play, obj1, idx1);
            self->unk_250 = 0;
        } else {
            // N64: Actor_SetObjectDependency(play, actor). Empty on 3DS; armlink inlines it to `mov r0, r0`
            // (same idiom as Bg_Haka_Megane).
            ((void (*)(PlayState*, Actor*))GraphicsContext_UnknownStub)(play, &self->dyna.actor);
            colHeader = NULL;
            self->dyna.actor.draw = (ActorFunc)FUN_0029d22c;
            if (self->dyna.actor.params == 0) {
                self->actionFunc = FUN_001280fc;
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, &play->state, 0), &colHeader);
                idx0 = 0; obj0 = OBJECT_MORI_HINERI1; idx1 = 0; obj1 = OBJECT_MORI_HINERI1A;
            } else if (self->dyna.actor.params == 1) {
                self->actionFunc = FUN_001e9198;
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, &play->state, 0), &colHeader);
                idx0 = 0; obj0 = OBJECT_MORI_HINERI1A; idx1 = 0; obj1 = OBJECT_MORI_HINERI1;
            } else if (self->dyna.actor.params == 2) {
                self->actionFunc = FUN_003e7f14;
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, &play->state, 0), &colHeader);
                idx0 = 0; obj0 = OBJECT_MORI_HINERI2; idx1 = 0; obj1 = OBJECT_MORI_HINERI2A;
            } else {
                self->actionFunc = FUN_001280fc;
                ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, &play->state, 0), &colHeader);
                idx0 = 0; obj0 = OBJECT_MORI_HINERI2A; idx1 = 0; obj1 = OBJECT_MORI_HINERI2;
            }
            FUN_00372f38(&self->dyna.actor, play, NULL);
            self->skelAnimModel0 = FUN_0036a924(&self->dyna.actor, play, obj0, idx0);
            self->skelAnimModel1 = FUN_0036a924(&self->dyna.actor, play, obj1, idx1);
            self->unk_250 = 0;
            self->skelAnimModel2 = FUN_0036a924(&self->dyna.actor, play, 1, 0x21);
            if (self->boxObjectSlot != -1) {
                f32 frame;
                self->dyna.actor.objBankIndex = self->boxObjectSlot;
                FUN_00353c9c(&self->dyna.actor, play, &self->skelAnime, 1, 2, NULL, NULL, 0);
                self->dyna.actor.objBankIndex = self->moriHineriObjectSlot;
                frame = Flags_GetTreasure(play, 0xE) ? (f32)Animation_GetLastFrame(&self->skelAnime, 2) : 0.0f;
                Animation_Change_00375c08(&self->skelAnime, 2, 1.0f, frame, frame, 2, 0.0f);
                FUN_0037266c(self->skelAnime.unk_28, 2);
                FUN_0037266c(self->skelAnime.unk_28, 0);
                FUN_0036932c(self->skelAnime.unk_28, 3);
                FUN_0036932c(self->skelAnime.unk_28, 1);
            }
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
        }
    }
}

void FUN_003e7f14(BgMoriHineri* self, PlayState* play) {
}

void FUN_001e9198(BgMoriHineri* self, PlayState* play) {
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOX, self->dyna.actor.world.pos.x + 147.0f,
                self->dyna.actor.world.pos.y + -245.0f, self->dyna.actor.world.pos.z + -453.0f, 0, 0x4000, 0, 0x27EE, 1);
    self->actionFunc = FUN_003e7f14;
}


void FUN_001280fc(BgMoriHineri* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;
    Player* player = GET_PLAYER(play);
    f32 f0 = 1100.0f - (player->actor.world.pos.z - thisx->world.pos.z);
    thisx->shape.rot.z = CLAMP(f0, 0.0f, 1000.0f) * 16.384f;
    Camera_ChangeSetting(play->cameraPtrs[0], 4);
    if (thisx->params != 0) {
        thisx->shape.rot.z = -thisx->shape.rot.z;
    }
}

void FUN_00174cb0(BgMoriHineri* self, PlayState* play) {
    s8 objectSlot;

    if (play->activeCamera == sBgMoriHineriSubCamId) {
        if (sBgMoriHineriSubCamId != 0) {
            objectSlot = self->dyna.actor.objBankIndex;
            self->dyna.actor.objBankIndex = self->moriHineriObjectSlot;
            self->moriHineriObjectSlot = objectSlot;
            self->dyna.actor.params ^= 1;
            self->unk_250 ^= 1;
            sBgMoriHineriSubCamId = 0;
            FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
        } else {
            self->dyna.actor.draw = NULL;
            self->actionFunc = FUN_00174e04;
            sBgMoriHineriSubCamId = CAM_ID_NONE;
        }
    }
    if ((sBgMoriHineriSubCamId > 0) && ((play->cameraPtrs[play->activeCamera]->eye.z - self->dyna.actor.world.pos.z) < 1100.0f)) {
        FUN_0035ae08(&self->dyna.actor, 0x01000204);
    }
}

void FUN_00174e04(BgMoriHineri* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;
    s16 mainCamChildId;

    if ((Flags_GetSwitch(play, self->switchFlag) && (self->dyna.actor.params == 0 || self->dyna.actor.params == 2)) ||
        (!Flags_GetSwitch(play, self->switchFlag) && (self->dyna.actor.params == 1 || self->dyna.actor.params == 3))) {
        thisx->draw = (ActorFunc)FUN_0029d22c;
        self->actionFunc = FUN_00174cb0;

        mainCamChildId = CAMERA_CHILD_CAM_ID(play->cameraPtrs[0]);
        if ((mainCamChildId != 0) && (CAMERA_SETTING(play->cameraPtrs[mainCamChildId]) == CAM_SET_CS_TWISTED_HALLWAY)) {
            OnePointCutscene_EndCutscene(play, mainCamChildId);
        }
        OnePointCutscene_Init(play, 0xCBC, 0x28, thisx, 0);
        sBgMoriHineriSubCamId = OnePointCutscene_Init(play, 0xCBD, 0x28, thisx, 0);
    }
}

void BgMoriHineri_Destroy(Actor* thisx, GameState* state) {
    BgMoriHineri* self = (BgMoriHineri*)thisx;
    PlayState* play = (PlayState*)state;

    SkelAnime_Free2(&self->skelAnime);
    FUN_00357248(&self->skelAnimModel2);
    FUN_00357248(&self->skelAnimModel0);
    FUN_00357248(&self->skelAnimModel1);
    FUN_00350f34(&self->dyna.actor, NULL);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
}

void BgMoriHineri_Update(Actor* thisx, GameState* state) {
    BgMoriHineri* self = (BgMoriHineri*)thisx;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}
}
