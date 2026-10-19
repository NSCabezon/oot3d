extern "C" {
#include "z_bg_spot18_shutter.hpp"

#define FLAGS 0x80400030

#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

// INFTABLE_109 (N64 name): gSaveContext.infTable[16], bit 0x200 on 3DS
#define INFTABLE_109_SET (gSaveContext.infTable[16] & 0x200)

extern __pure float _fabsf(float);

void BgSpot18Shutter_Init(Actor* actor, GameState* state);
void BgSpot18Shutter_Destroy(Actor* actor, GameState* state);
void BgSpot18Shutter_Update(Actor* actor, GameState* state);
void BgSpot18Shutter_Draw(Actor* actor, GameState* state);

void BgSpot18Shutter_DoNothing(BgSpot18Shutter* self, PlayState* play); // N64: func_808B95AC
void FUN_00175458(BgSpot18Shutter* self, PlayState* play);              // N64: func_808B95B8
void FUN_00199688(BgSpot18Shutter* self, PlayState* play);              // N64: func_808B9618
void FUN_00138560(BgSpot18Shutter* self, PlayState* play);              // N64: func_808B9698
void FUN_001995ec(BgSpot18Shutter* self, PlayState* play);              // N64: func_808B971C
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);
void OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 arg2, s32 arg3);

ActorInit Bg_Spot18_Shutter_InitVars = {
    ACTOR_BG_SPOT18_SHUTTER,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT18_OBJ,
    sizeof(BgSpot18Shutter),
    (ActorFunc)BgSpot18Shutter_Init,
    (ActorFunc)BgSpot18Shutter_Destroy,
    (ActorFunc)BgSpot18Shutter_Update,
    (ActorFunc)BgSpot18Shutter_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

// The raise path writes pos.y through `actor` (not self->dyna.actor): with self-> armcc sinks the
// shared actionFunc store into the slide path and the function grows.
void BgSpot18Shutter_Init(Actor* actor, GameState* state) {
    BgSpot18Shutter* self = (BgSpot18Shutter*)actor;
    PlayState* play = (PlayState*)state;
    s32 param = PARAMS_GET_U(actor->params, 8, 1);
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    Actor_ProcessInitChain(actor, sInitChain);
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, 0);

    if (param == 0) {
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            if (INFTABLE_109_SET) {
                actor->world.pos.y += 180.0f;
                self->actionFunc = BgSpot18Shutter_DoNothing;
            } else {
                self->actionFunc = FUN_00199688;
            }
        } else {
            if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 0, 6))) {
                actor->world.pos.y += 180.0f;
                self->actionFunc = BgSpot18Shutter_DoNothing;
            } else {
                self->actionFunc = FUN_00175458;
            }
        }
    } else {
        if (INFTABLE_109_SET) {
            self->dyna.actor.world.pos.x += 125.0f * Math_CosS(self->dyna.actor.world.rot.y);
            self->dyna.actor.world.pos.z -= 125.0f * Math_SinS(self->dyna.actor.world.rot.y);
            self->actionFunc = BgSpot18Shutter_DoNothing;
        } else {
            self->actionFunc = FUN_00199688;
        }
    }

    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
}

void BgSpot18Shutter_Destroy(Actor* actor, GameState* state) {
    BgSpot18Shutter* self = (BgSpot18Shutter*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot18Shutter_DoNothing(BgSpot18Shutter* self, PlayState* play) {
}

void FUN_00175458(BgSpot18Shutter* self, PlayState* play) {
    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 0, 6))) {
        Actor_SetFocus(&self->dyna.actor, 70.0f);
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        self->actionFunc = FUN_00138560;
    }
}

// NON_MATCHING draft (Haiku stage 1): still GLOBAL_ASM in functions_180000s.cpp.
// The asm entry (212 B) also contains an unlabeled code block at 0x00199700 that other
// actors' tables point to, so it cannot simply be replaced by this C body.
#if 0
void FUN_00199688(BgSpot18Shutter* self, PlayState* play) {
    if (INFTABLE_109_SET) {
        Actor_SetFocus(&self->dyna.actor, 70.0f);
        if (PARAMS_GET_U(self->dyna.actor.params, 8, 1) == 0) {
            self->actionFunc = FUN_00138560;
        } else {
            self->actionFunc = FUN_001995ec;
            OnePointCutscene_Init(play, 4221, 140, &self->dyna.actor, 0);
        }
    }
}
#endif

void FUN_00138560(BgSpot18Shutter* self, PlayState* play) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 180.0f, 1.44f)) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001EA); // NA_SE_EV_STONEDOOR_STOP
        self->actionFunc = BgSpot18Shutter_DoNothing;
    } else {
        FUN_00373264(&self->dyna.actor, 0x01000175); // NA_SE_EV_STONE_STATUE_OPEN - SFX_FLAG
    }
}

// _fabsf through a static inline wrapper: calling _fabsf/__fabsf directly schedules the vabs differently
static __inline __pure float BgSpot18Shutter_Fabsf(float f) {
    return _fabsf(f);
}

void FUN_001995ec(BgSpot18Shutter* self, PlayState* play) {
    f32 sin = Math_SinS(self->dyna.actor.world.rot.y);
    f32 cos = Math_CosS(self->dyna.actor.world.rot.y);
    s32 flag = true;

    flag &= Math_StepToF(&self->dyna.actor.world.pos.x, self->dyna.actor.home.pos.x + (125.0f * cos),
                         BgSpot18Shutter_Fabsf(cos));
    flag &= Math_StepToF(&self->dyna.actor.world.pos.z, self->dyna.actor.home.pos.z - (125.0f * sin),
                         BgSpot18Shutter_Fabsf(sin));

    if (flag) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001EA); // NA_SE_EV_STONEDOOR_STOP
        self->actionFunc = BgSpot18Shutter_DoNothing;
    } else {
        FUN_00373264(&self->dyna.actor, 0x01000175); // NA_SE_EV_STONE_STATUE_OPEN - SFX_FLAG
    }
}

void BgSpot18Shutter_Update(Actor* actor, GameState* state) {
    BgSpot18Shutter* self = (BgSpot18Shutter*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgSpot18Shutter_Draw(Actor* actor, GameState* state) {
    BgSpot18Shutter* self = (BgSpot18Shutter*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

}
