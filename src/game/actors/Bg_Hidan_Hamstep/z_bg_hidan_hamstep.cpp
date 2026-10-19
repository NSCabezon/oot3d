extern "C" {
#include "z_bg_hidan_hamstep.hpp"

#define FLAGS 0x00400000

#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))
#define CAM_ID_MAIN 0
#define NA_SE_EV_HAMMER_SWITCH 0x01000191
#define NA_SE_EV_BLOCK_BOUND 0x0100018C

s32 Collider_DestroyTris(PlayState* play, ColliderTris* collider);
void Flags_SetSwitch(PlayState* play, s32 flag);
void OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);


extern BgHidanHamstepActionFunc BgHidanHamstep_sActionFuncs[];

void BgHidanHamstep_Init(Actor* actor, GameState* state);
void BgHidanHamstep_Destroy(Actor* actor, GameState* state);
void BgHidanHamstep_Update(Actor* actor, GameState* state);
void BgHidanHamstep_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Hidan_Hamstep.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Hidan_Hamstep/data_syms.s")

// Static data lives in the data dump above (0x005118B0..); EQU symbols in data_syms.s.
extern InitChainEntry BgHidanHamstep_sInitChain[];
extern f32 BgHidanHamstep_sYPosOffsets[];
extern ColliderTrisInit BgHidanHamstep_sTrisInit;
extern f32 BgHidanHamstep_sEffectPositions[][2];
#define sInitChain BgHidanHamstep_sInitChain
#define sYPosOffsets BgHidanHamstep_sYPosOffsets
#define sTrisInit BgHidanHamstep_sTrisInit
#define sActionFuncs BgHidanHamstep_sActionFuncs
#define sEffectPositions BgHidanHamstep_sEffectPositions

s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s32 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s32 a, s32 b, s32 c, s32 d);
void Quake_SetCountdown(s32 quakeIndex, s32 countdown);

ActorInit Bg_Hidan_Hamstep_InitVars = {
    ACTOR_BG_HIDAN_HAMSTEP,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanHamstep),
    (ActorFunc)BgHidanHamstep_Init,
    (ActorFunc)BgHidanHamstep_Destroy,
    (ActorFunc)BgHidanHamstep_Update,
    (ActorFunc)BgHidanHamstep_Draw,
};

static inline void BgHidanHamstep_SetupAction(BgHidanHamstep* self, s32 action) {
    self->action = action;
    self->actionFunc = sActionFuncs[action];
}


void Collider_InitTris(PlayState* play, ColliderTris* collider);
void Collider_SetTris(PlayState* play, ColliderTris* collider, Actor* actor, void* src, ColliderTrisElement* elements);
void Collider_SetTrisVertices(ColliderTris* collider, s32 index, Vec3f* a, Vec3f* b, Vec3f* c);

// BgHidanHamstep_Init: not matched (1060 vs 1020 bytes, ~230 words differ). Draft below; ROM keeps the incoming
// play spilled at [sp,#0x58] and a second copy at [sp,#0x38] (N64 SpawnChildren inlined with `PlayState* play = play2`),
// copies thisx with `mov r4,r0` before DynaPolyActor_Init, and stores the DynaPolyInfo_Alloc result via
// `add r1,sp,#0x34; str r0,[r1]` (inlined CollisionHeader_GetVirtual-like store, two copies with nops).
// FUN_00372f38 is called with 7 args here (3 on stack): (actor, play, &skel[0], 10, &skel[1], 9, 0).
#if 0
void BgHidanHamstep_Init(Actor* actor, GameState* state) {
    BgHidanHamstep* self = (BgHidanHamstep*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    Vec3f sp48[3];
    s32 i;
    s32 i2;

    DynaPolyActor_Init(&self->dyna, 1);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);

    if (PARAMS_GET_U(self->dyna.actor.params, 0, 8) == 0) {
        Collider_InitTris(play, &self->collider);
        Collider_SetTris(play, &self->collider, &self->dyna.actor, &sTrisInit, self->colliderElements);

        for (i = 0; i < 2; i++) {
            for (i2 = 0; i2 < 3; i2++) {
                sp48[i2].x = sTrisInit.elements[i].dim.vtx[i2].x + self->dyna.actor.home.pos.x;
                sp48[i2].y = sTrisInit.elements[i].dim.vtx[i2].y + self->dyna.actor.home.pos.y;
                sp48[i2].z = sTrisInit.elements[i].dim.vtx[i2].z + self->dyna.actor.home.pos.z;
            }
            Collider_SetTrisVertices(&self->collider, i, &sp48[0], &sp48[1], &sp48[2]);
        }
    }

    FUN_00372f38(actor, play, &self->skelAnimModel[0], 10, &self->skelAnimModel[1], 9,
                                                0);

    if (PARAMS_GET_U(self->dyna.actor.params, 0, 8) == 0) {
        colHeader = (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, state, 6);
    } else {
        colHeader = (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, state, 5);
    }

    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);

    if (Flags_GetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 8))) {
        if (PARAMS_GET_U(self->dyna.actor.params, 0, 8) == 0) {
            self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + (-20.0f);
            BgHidanHamstep_SetupAction(self, 4);
        } else {
            self->dyna.actor.world.pos.y =
                sYPosOffsets[PARAMS_GET_U(self->dyna.actor.params, 0, 8) - 1] + self->dyna.actor.home.pos.y;
            BgHidanHamstep_SetupAction(self, 4);
        }
    } else if (PARAMS_GET_U(self->dyna.actor.params, 0, 8) == 0) {
        BgHidanHamstep_SetupAction(self, 0);
    } else {
        BgHidanHamstep_SetupAction(self, 2);
    }

    self->dyna.actor.gravity = -1.2f;
    self->dyna.actor.minVelocityY = -12.0f;

    if (PARAMS_GET_U(self->dyna.actor.params, 0, 8) == 0) {
        BgHidanHamstep* step = self;
        f32 posY = self->dyna.actor.home.pos.y - 100.0f;
        f32 sin = Math_SinS(self->dyna.actor.shape.rot.y + 0x8000);
        f32 cos = Math_CosS(self->dyna.actor.shape.rot.y + 0x8000);
        s16 params;
        Vec3f pos;

        for (i = 0; i < 5; i++) {
            pos.x = (((i * 160.0f) + 60.0f) * sin) + self->dyna.actor.home.pos.x;
            pos.y = posY;
            pos.z = (((i * 160.0f) + 60.0f) * cos) + self->dyna.actor.home.pos.z;

            params = (i + 1) & 0xFF;
            params |= self->dyna.actor.params & 0xFF00;

            step = (BgHidanHamstep*)Actor_SpawnAsChild(&play->actorCtx, &step->dyna.actor, play,
                                                       ACTOR_BG_HIDAN_HAMSTEP, pos.x, pos.y, pos.z,
                                                       self->dyna.actor.world.rot.x, self->dyna.actor.world.rot.y,
                                                       self->dyna.actor.world.rot.z, params);

            if (step == NULL) {
                step = self;
                while (step != NULL) {
                    Actor_Kill(&step->dyna.actor);
                    step = (BgHidanHamstep*)step->dyna.actor.child;
                }
                break;
            }
        }
    }
}
#endif

GLOBAL_ASM("asm/BgHidanHamstep_Init.s")

void BgHidanHamstep_Destroy(Actor* actor, GameState* state) {
    BgHidanHamstep* self = (BgHidanHamstep*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    if (PARAMS_GET_U(actor->params, 0, 8) == 0) {
        Collider_DestroyTris(play, &self->collider);
    }
    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1], 0);
}

void BgHidanHamstep_Update(Actor* actor, GameState* state) {
    BgHidanHamstep* self = (BgHidanHamstep*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void BgHidanHamstep_Draw(Actor* actor, GameState* state) {
    BgHidanHamstep* self = (BgHidanHamstep*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    if (PARAMS_GET_U(actor->params, 0, 8) == 0) {
        if (self->skelAnimModel[0] != NULL) {
            self->skelAnimModel[0]->SetUnkAC(1);
            self->skelAnimModel[0]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[0]->FUN_00372170(0);
        }
    } else {
        if (self->skelAnimModel[1] != NULL) {
            self->skelAnimModel[1]->SetUnkAC(1);
            self->skelAnimModel[1]->FUN_003721e0(&modelMtx);
            self->skelAnimModel[1]->FUN_00372170(0);
        }
    }
}

void FUN_002070e4(BgHidanHamstep* self, PlayState* play) {
}

// N64 func_80888860: action 1 (parent falls and bounces)
void FUN_00109acc(BgHidanHamstep* self, PlayState* play) {
    s32 quakeIndex;

    Actor_MoveForward(&self->dyna.actor);

    if (((self->dyna.actor.world.pos.y - self->dyna.actor.home.pos.y) < (-20.0f - self->dyna.actor.minVelocityY)) &&
        (self->dyna.actor.velocity.y <= 0.0f)) {
        self->unk_29C++;

        if (self->unk_29C > 6) {
            self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + -20.0f;
            BgHidanHamstep_SetupAction(self, 4);
        } else {
            { Vec3f* vel = &self->dyna.actor.velocity; vel->y *= -0.24f; }

            if (self->unk_29C == 1) {
                quakeIndex = Quake_Add(play->cameraPtrs[play->activeCamera], 3);
                Quake_SetSpeed(quakeIndex, -15536);
                Quake_SetQuakeValues(quakeIndex, 0, 0, 500, 0);
                Quake_SetCountdown(quakeIndex, 20);
                Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
                {
                    BgHidanHamstep* child = (BgHidanHamstep*)self->dyna.actor.child;

                    while (child != NULL) {
                        if ((child->dyna.actor.params & 0xFF) != 0) {
                            Vec3f pos = child->dyna.actor.world.pos;
                            s32 i;
                            f32 sin;
                            f32 cos;

                            pos.y -= 20.0f;
                            FUN_0037378c(play, &pos, 0.0f, 0, 600, 300, 0);
                            sin = Math_SinS(child->dyna.actor.shape.rot.y + 0x8000);
                            cos = Math_CosS(child->dyna.actor.shape.rot.y + 0x8000);
                            pos.y = child->dyna.actor.world.pos.y;
                            for (i = 0; i < 11; i++) {
                                pos.x = (sEffectPositions[i][1] * sin) + (sEffectPositions[i][0] * cos) +
                                        child->dyna.actor.world.pos.x;
                                pos.z = ((sEffectPositions[i][1] * cos) - (sEffectPositions[i][0] * sin)) +
                                        child->dyna.actor.world.pos.z;
                                FUN_0037378c(play, &pos, 0.0f, 0, 150, 150, 0);
                            }
                        }
                        child = (BgHidanHamstep*)child->dyna.actor.child;
                    }
                }
            }
        }
    }
}

// N64 func_80888A58: action 3 (steps fall into place). N64 helpers func_80888694/func_808884C8 written inline;
// the effects block needs its own `step` copy of self (ROM r6).
void FUN_00123038(Actor* thisx, PlayState* play) {
    BgHidanHamstep* self = (BgHidanHamstep*)thisx;
    s32 quakeIndex;

    Actor_MoveForward(&self->dyna.actor);

    {
        BgHidanHamstep* parent = (BgHidanHamstep*)thisx->parent;
        BgHidanHamstep* child;

        if ((thisx->params & 0xFF) > 1) {
            f32 diff = thisx->world.pos.y - parent->dyna.actor.world.pos.y;

            if (diff > 0.0f) {
                thisx->world.pos.y = parent->dyna.actor.world.pos.y;
            } else if (diff < -100.0f) {
                thisx->world.pos.y = parent->dyna.actor.world.pos.y - 100.0f;
            }
        }
        child = (BgHidanHamstep*)thisx->child;
        while (child != NULL) {
            if (child->dyna.actor.world.pos.y > thisx->world.pos.y) {
                child->dyna.actor.world.pos.y = thisx->world.pos.y;
            }
            child = (BgHidanHamstep*)child->dyna.actor.child;
        }
    }

    if (((thisx->world.pos.y - thisx->home.pos.y) <= sYPosOffsets[(thisx->params & 0xFF) - 1]) &&
        (thisx->velocity.y <= 0.0f)) {
        self->unk_29C++;

        if (self->unk_29C > 6) {
            thisx->world.pos.y = thisx->home.pos.y + sYPosOffsets[(thisx->params & 0xFF) - 1];
            BgHidanHamstep_SetupAction(self, 3);
        } else {
            thisx->velocity.y *= -0.24f;

            if (self->unk_29C == 1) {
                quakeIndex = Quake_Add(play->cameraPtrs[play->activeCamera], 3);
                Quake_SetSpeed(quakeIndex, -15536);
                Quake_SetQuakeValues(quakeIndex, 20, 1, 0, 0);
                Quake_SetCountdown(quakeIndex, 7);
                Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
                {
                    BgHidanHamstep* step = self;
                    Vec3f pos = step->dyna.actor.world.pos;
                    s32 i;
                    f32 sin;
                    f32 cos;

                    pos.y -= 20.0f;
                    FUN_0037378c(play, &pos, 0.0f, 0, 600, 300, 0);
                    sin = Math_SinS(step->dyna.actor.shape.rot.y + 0x8000);
                    cos = Math_CosS(step->dyna.actor.shape.rot.y + 0x8000);
                    pos.y = step->dyna.actor.world.pos.y;
                    for (i = 0; i < 11; i++) {
                        pos.x = (sEffectPositions[i][1] * sin) + (sEffectPositions[i][0] * cos) +
                                step->dyna.actor.world.pos.x;
                        pos.z = ((sEffectPositions[i][1] * cos) - (sEffectPositions[i][0] * sin)) +
                                step->dyna.actor.world.pos.z;
                        FUN_0037378c(play, &pos, 0.0f, 0, 150, 150, 0);
                    }
                }
                if ((thisx->params & 0xFF) == 5) {
                    FUN_00372244(&play->unk_5FCC, 0x1E, 0x1000487);
                }
            }
        }
    }
}

void FUN_001e9f50(BgHidanHamstep* self, PlayState* play) {
    PlayState* play2 = play;

    if (self->collider.base.acFlags & AC_HIT) {
        OnePointCutscene_Init(play, 3310, 100, &self->dyna.actor, CAM_ID_MAIN);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_HAMMER_SWITCH);
        self->collider.base.acFlags = AC_NONE;
        BgHidanHamstep_SetupAction(self, 1);
        Flags_SetSwitch(play, (u8)(self->dyna.actor.params >> 8));
    } else {
        CollisionCheck_SetAC(play2, &play->colChkCtx, &self->collider.base);
    }
}
}
