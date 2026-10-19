extern "C" {
#include "z_bg_ice_shutter.hpp"

#define FLAGS 0x80400010

#define NA_SE_EV_SLIDE_DOOR_OPEN 0x0100016B
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

s32 OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 arg2, s32 arg3);

void BgIceShutter_Init(Actor* actor, GameState* state);
void BgIceShutter_Destroy(Actor* actor, GameState* state);
void BgIceShutter_Update(Actor* actor, GameState* state);
void BgIceShutter_Draw(Actor* actor, GameState* state);

void FUN_003e7e9c(BgIceShutter* self, GameState* state); // matched; N64: func_80891CF4 (wait for temp clear)
void FUN_0010b970(BgIceShutter* self, PlayState* play); // N64: func_80891D6C (wait for switch)
void FUN_003e01fc(BgIceShutter* self, PlayState* play); // N64: func_80891DD4 (slide open)

GLOBAL_ASM("data/z_Bg_Ice_Shutter.data.s")

ActorInit Bg_Ice_Shutter_InitVars = {
    ACTOR_BG_ICE_SHUTTER,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_ICE_OBJECTS,
    sizeof(BgIceShutter),
    (ActorFunc)BgIceShutter_Init,
    (ActorFunc)BgIceShutter_Destroy,
    (ActorFunc)BgIceShutter_Update,
    (ActorFunc)BgIceShutter_Draw,
};


// (sInitChain stays in data/z_Bg_Ice_Shutter.data.s: pinned by the ld script)
// sInitChain (ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP)) lives unlabeled in the data dump above -> raw address.

void BgIceShutter_Init(Actor* actor, GameState* state) {
    BgIceShutter* self = (BgIceShutter*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    s32 sp28;

    Actor_ProcessInitChain(actor, (InitChainEntry*)0x0051215C);
    DynaPolyActor_Init(&self->dyna, 0);
    sp28 = PARAMS_GET_U(actor->params, 0, 8);
    actor->params = PARAMS_GET_U(actor->params, 8, 8);
    FUN_00372f38(actor, play, &self->skelAnimModel, 6, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 3), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    if (sp28 == 2) {
        actor->shape.rot.x = -0x4000;
    }

    if (sp28 != 1) {
        if (Flags_GetClear(play, actor->room)) {
            Actor_Kill(actor);
        } else {
            self->actionFunc = (BgIceShutterActionFunc)FUN_003e7e9c;
        }
    } else {
        if (Flags_GetSwitch(play, actor->params)) {
            Actor_Kill(actor);
        } else {
            self->actionFunc = FUN_0010b970;
        }
    }

    if (sp28 == 2) {
        f32 temp_f6 = Math_SinS(actor->shape.rot.x) * 50.0f;

        actor->focus.pos.x = (temp_f6 * Math_SinS(actor->shape.rot.y)) + actor->home.pos.x;
        actor->focus.pos.y = actor->home.pos.y;
        actor->focus.pos.z = actor->home.pos.z + (temp_f6 * Math_CosS(actor->shape.rot.y));
    } else {
        Actor_SetFocus(actor, 50.0f);
    }
}


void BgIceShutter_Destroy(Actor* actor, GameState* state) {
    BgIceShutter* self = (BgIceShutter*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void FUN_003e7e9c(BgIceShutter* self, GameState* state) {
    PlayState* play = (PlayState*)state;

    if (Flags_GetTempClear(play, self->dyna.actor.room)) {
        Flags_SetClear(play, self->dyna.actor.room);
        Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 30, NA_SE_EV_SLIDE_DOOR_OPEN);
        self->actionFunc = FUN_003e01fc;
        if (self->dyna.actor.shape.rot.x == 0) {
            OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        }
    }
}


void FUN_0010b970(BgIceShutter* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->dyna.actor.params)) {
        Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 30, NA_SE_EV_SLIDE_DOOR_OPEN);
        self->actionFunc = FUN_003e01fc;
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
    }
}

void FUN_003e01fc(BgIceShutter* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;

    Math_StepToF(&thisx->speedXZ, 30.0f, 2.0f);
    if (Math_StepToF(&thisx->velocity.y, 210.0f, thisx->speedXZ)) {
        Actor_Kill(thisx);
        return;
    }

    {
        f32 sp24 = Math_SinS(thisx->shape.rot.x) * thisx->velocity.y;

        thisx->world.pos.y = thisx->home.pos.y + (thisx->velocity.y * Math_CosS(thisx->shape.rot.x));
        thisx->world.pos.x = thisx->home.pos.x + (sp24 * Math_SinS(thisx->shape.rot.y));
        thisx->world.pos.z = thisx->home.pos.z + (sp24 * Math_CosS(thisx->shape.rot.y));
    }
}

void BgIceShutter_Update(Actor* actor, GameState* state) {
    BgIceShutter* self = (BgIceShutter*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgIceShutter_Draw(Actor* actor, GameState* state) {
    BgIceShutter* self = (BgIceShutter*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
