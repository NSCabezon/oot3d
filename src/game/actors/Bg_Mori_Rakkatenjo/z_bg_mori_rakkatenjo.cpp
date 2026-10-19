extern "C" {
#include "z_bg_mori_rakkatenjo.hpp"

#define FLAGS 0x80400030

#define CAM_SET_NONE 0

// Helpers not declared in include/ yet
s32 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
s32 Object_IsLoaded(ObjectContext* objectCtx, s32 slot);
void Camera_ChangeSetting(Camera* camera, s32 setting);
void Camera_SetCameraData(Camera* camera, s32 dataSet, Actor* actor, void* a3, s32 a4, s32 a5, s32 a6);
s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s32 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s32 a, s32 b, s32 c, s32 d);
void Quake_SetCountdown(s32 quakeIndex, s32 countdown);
__attribute__((noinline)) void FUN_001cf0b4(BgMoriRakkatenjo* self); // N64: BgMoriRakkatenjo_SetupWait (tail call from Rise)
void FUN_001108d8(BgMoriRakkatenjo* self, PlayState* play); // N64: BgMoriRakkatenjo_WaitForMoriTex
void FUN_0015eee0(BgMoriRakkatenjo* self, PlayState* play); // N64: BgMoriRakkatenjo_Wait
void FUN_003bc350(BgMoriRakkatenjo* self, PlayState* play); // N64: BgMoriRakkatenjo_Fall
void FUN_00144574(BgMoriRakkatenjo* self, PlayState* play); // N64: BgMoriRakkatenjo_Rest
void FUN_003bd188(BgMoriRakkatenjo* self, PlayState* play); // N64: BgMoriRakkatenjo_Rise
void FUN_0038adb0(Actor* actor, GameState* state);          // N64: BgMoriRakkatenjo_Draw

void BgMoriRakkatenjo_Init(Actor* actor, GameState* state);
void BgMoriRakkatenjo_Destroy(Actor* actor, GameState* state);
void BgMoriRakkatenjo_Update(BgMoriRakkatenjo* self, PlayState* play);

GLOBAL_ASM("data/z_Bg_Mori_Rakkatenjo.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Mori_Rakkatenjo/data_syms.s")

// sCamSetting (0x005133A4), sInitChain (0x005133A8) and the Fall bounce table (0x005133B4) live in the data
// dump above. They have no symbols in the scatter file, so they are absolute symbols (EQU) in data_syms.s.
extern s16 BgMoriRakkatenjo_sCamSetting;
extern InitChainEntry BgMoriRakkatenjo_sInitChain[];
extern f32 BgMoriRakkatenjo_sBounceVel[];
#define sCamSetting BgMoriRakkatenjo_sCamSetting
#define sInitChain BgMoriRakkatenjo_sInitChain
#define sBounceVel BgMoriRakkatenjo_sBounceVel

ActorInit Bg_Mori_Rakkatenjo_InitVars = {
    ACTOR_BG_MORI_RAKKATENJO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriRakkatenjo),
    (ActorFunc)BgMoriRakkatenjo_Init,
    (ActorFunc)BgMoriRakkatenjo_Destroy,
    (ActorFunc)BgMoriRakkatenjo_Update,
    NULL,
};

void BgMoriRakkatenjo_Init(Actor* actor, GameState* state) {
    BgMoriRakkatenjo* self = (BgMoriRakkatenjo*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 1);
    FUN_00372f38(actor, play, &self->skelAnimModel, 7, 0);
    self->moriTexObjectSlot = Object_GetIndex(&play->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjectSlot < 0) {
        Actor_Kill(actor);
        return;
    }
    Actor_ProcessInitChain(actor, sInitChain);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 5), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    self->actionFunc = FUN_001108d8;
    sCamSetting = CAM_SET_NONE;
}

void BgMoriRakkatenjo_Destroy(Actor* actor, GameState* state) {
    BgMoriRakkatenjo* self = (BgMoriRakkatenjo*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

// fabsf through an inline wrapper: plain __fabsf schedules the velocity load after the multiply.
static __inline __pure float BgMoriRakkatenjo_Fabsf(float x) {
    return __fabsf(x);
}

static inline s32 BgMoriRakkatenjo_IsLinkUnder(BgMoriRakkatenjo* self, PlayState* play) {
    Vec3f* pos = &GET_PLAYER(play)->actor.world.pos;

    return (-3300.0f < pos->z) && (pos->z < -1840.0f) && (1791.0f < pos->x) && (pos->x < 2191.0f);
}

static inline s32 BgMoriRakkatenjo_IsLinkClose(BgMoriRakkatenjo* self, PlayState* play) {
    Vec3f* pos = &GET_PLAYER(play)->actor.world.pos;

    return (-3360.0f < pos->z) && (pos->z < -1840.0f) && (1791.0f < pos->x) && (pos->x < 2191.0f);
}

void BgMoriRakkatenjo_Update(BgMoriRakkatenjo* self, PlayState* play) {
    if (self->timer > 0) {
        self->timer--;
    }
    self->actionFunc(self, play);
    if (BgMoriRakkatenjo_IsLinkUnder(self, play)) {
        if (sCamSetting == CAM_SET_NONE) {
            sCamSetting = *(s16*)((u8*)play->cameraPtrs[0] + 0x18A); // Camera.setting (raw)
            Camera_SetCameraData(play->cameraPtrs[0], 1, &self->dyna.actor, NULL, 0, 0, 0);
            Camera_ChangeSetting(play->cameraPtrs[0], 0x27);
        }
    } else if (sCamSetting != CAM_SET_NONE) {
        Camera_ChangeSetting(play->cameraPtrs[0], 4);
        sCamSetting = CAM_SET_NONE;
    }
}

void FUN_001108d8(BgMoriRakkatenjo* self, PlayState* play) {
    if (Object_IsLoaded(&play->objectCtx, self->moriTexObjectSlot)) {
        self->timer = (self->fallCount > 0) ? 165 : 32;
        self->dyna.actor.world.pos.y = 683.0f;
        self->actionFunc = FUN_0015eee0;
        self->dyna.actor.draw = (ActorFunc)FUN_0038adb0;
    }
}

void FUN_001cf0b4(BgMoriRakkatenjo* self) {
    self->timer = (self->fallCount > 0) ? 165 : 32;
    self->dyna.actor.world.pos.y = 683.0f;
    self->actionFunc = FUN_0015eee0;
}

void FUN_0015eee0(BgMoriRakkatenjo* self, PlayState* play) {
    if (self->fallCount == 0) {
        if (BgMoriRakkatenjo_IsLinkClose(self, play) || (self->timer < 30)) {
            if (self->timer <= 0) {
                self->actionFunc = FUN_003bc350;
                self->dyna.actor.velocity.y = 0.0f;
                self->bounceCount = 0;
            }
        } else {
            self->timer = 32;
            return;
        }
    } else {
        if (BgMoriRakkatenjo_IsLinkUnder(self, play) || (self->timer < 30)) {
            if (self->timer <= 0) {
                self->actionFunc = FUN_003bc350;
                self->dyna.actor.velocity.y = 0.0f;
                self->bounceCount = 0;
            }
        } else {
            self->timer = 150;
            return;
        }
    }
    if (self->timer < 30) {
        Audio_PlaySoundGeneral(0x010001B3, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0); // NA_SE_EV_BLOCKSINK - SFX_FLAG
    }
}

void FUN_003bc350(BgMoriRakkatenjo* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;
    s32 quakeIndex;

    Actor_MoveForward(thisx);
    if ((thisx->velocity.y < 0.0f) && (thisx->world.pos.y <= 403.0f)) {
        if (self->bounceCount >= 4) {
            self->actionFunc = FUN_00144574;
            self->dyna.actor.world.pos.y = 403.0f;
            self->timer = 30;
        } else {
            if (self->bounceCount == 0) {
                self->fallCount++;
                Audio_PlaySoundGeneral(0x01000174, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0); // NA_SE_EV_STONE_BOUND
            }
            thisx->world.pos.y = 403.0f - (thisx->world.pos.y - 403.0f) * sBounceVel[self->bounceCount] / BgMoriRakkatenjo_Fabsf(thisx->velocity.y);
            thisx->velocity.y = sBounceVel[self->bounceCount];
            self->bounceCount++;

            quakeIndex = Quake_Add(play->cameraPtrs[*(s16*)((u8*)play + 0xA64)], 3);
            Quake_SetSpeed(quakeIndex, 30000);
            Quake_SetQuakeValues(quakeIndex, 5, 0, 0, 0);
            Quake_SetCountdown(quakeIndex, 5);
        }
    }
}

void FUN_00144574(BgMoriRakkatenjo* self, PlayState* play) {
    if (self->timer <= 0) {
        self->actionFunc = FUN_003bd188;
        self->dyna.actor.velocity.y = -0.06666667f;
    }
}

void FUN_003bd188(BgMoriRakkatenjo* self, PlayState* play) {
    Math_SmoothStepToF(&self->dyna.actor.velocity.y, 3.3333333f, 0.06f, 0.1f, 0.0f);
    self->dyna.actor.world.pos.y += self->dyna.actor.velocity.y;
    if (self->dyna.actor.world.pos.y >= 683.0f) {
        FUN_001cf0b4(self);
    }
}

void FUN_0038adb0(Actor* actor, GameState* state) {
    BgMoriRakkatenjo* self = (BgMoriRakkatenjo*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}

}
