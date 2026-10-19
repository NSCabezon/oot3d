extern "C" {
#include "z_bg_jya_bombiwa.hpp"

#define FLAGS 0x80400000

void BgJyaBombiwa_Init(Actor* actor, GameState* state);
void BgJyaBombiwa_Destroy(Actor* actor, GameState* state);
void BgJyaBombiwa_Update(Actor* actor, GameState* state);
void BgJyaBombiwa_Draw(Actor* actor, GameState* state);

// Collider helpers not declared in include/ yet (local, as in Bg_Jya_Goroiwa).
void Collider_InitJntSph(PlayState* play, BgJyaBombiwaColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgJyaBombiwaColliderJntSph* collider, Actor* actor,
                        BgJyaBombiwaJntSphInit* init, BgJyaBombiwaColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, BgJyaBombiwaColliderJntSph* collider);
void Collider_UpdateSpheres(s32 index, BgJyaBombiwaColliderJntSph* collider, MTX34* mtx);

GLOBAL_ASM("data/z_Bg_Jya_Bombiwa.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Jya_Bombiwa/data_syms.s")

// sJntSphInit / sInitChain live in the data dump above; absolute symbols from data_syms.s.
extern BgJyaBombiwaJntSphInit BgJyaBombiwa_sJntSphInit;
extern InitChainEntry BgJyaBombiwa_sInitChain[];

ActorInit Bg_Jya_Bombiwa_InitVars = {
    ACTOR_BG_JYA_BOMBIWA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaBombiwa),
    (ActorFunc)BgJyaBombiwa_Init,
    (ActorFunc)BgJyaBombiwa_Destroy,
    (ActorFunc)BgJyaBombiwa_Update,
    (ActorFunc)BgJyaBombiwa_Draw,
};

void BgJyaBombiwa_Init(Actor* actor, GameState* state) {
    BgJyaBombiwa* self = (BgJyaBombiwa*)actor;
    PlayState* play = (PlayState*)state;
    void* colInfo;

    FUN_00372f38(actor, play, &self->skelAnimModel, 5, 0);
    colInfo = DynaPolyInfo_Alloc(&self->dyna, state, 1);
    {
        CollisionHeader* colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(colInfo, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, actor, &BgJyaBombiwa_sJntSphInit, self->colliderElements);
    if (Flags_GetSwitch(play, actor->params & 0x3F)) {
        Actor_Kill(actor);
    } else {
        Actor_ProcessInitChain(actor, BgJyaBombiwa_sInitChain);
    }
}

void BgJyaBombiwa_Destroy(Actor* actor, GameState* state) {
    BgJyaBombiwa* self = (BgJyaBombiwa*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

static void BgJyaBombiwa_Break(BgJyaBombiwa* self, PlayState* play) {
    Vec3f pos;
    Vec3f velocity;
    s16 arg5;
    s8 arg6;
    s8 arg7;
    s32 i;
    s16 scale;

    for (i = 0; i < 16; i++) {
        pos.x = ((Rand_ZeroOne() * 80.0f) + self->dyna.actor.world.pos.x) - 40.0f;
        pos.y = (Rand_ZeroOne() * 140.0f) + self->dyna.actor.world.pos.y;
        pos.z = ((Rand_ZeroOne() * 80.0f) + self->dyna.actor.world.pos.z) - 40.0f;
        velocity.x = (Rand_ZeroOne() - 0.5f) * 10.0f;
        velocity.y = Rand_ZeroOne() * 12.0f;
        velocity.z = (Rand_ZeroOne() - 0.5f) * 10.0f;
        scale = (s32)(i * 1.8f) + 3;
        arg6 = 0xC;
        arg7 = 8;
        if (scale > 15) {
            arg5 = 0x201;
        } else {
            arg5 = 1;
        }
        if (Rand_ZeroOne() < 0.4f) {
            arg5 |= 0x40;
        } else {
            arg5 |= 0x20;
            if (scale < 10) {
                arg6 = 0x50;
                arg7 = 80;
            }
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, &pos, -400, arg5, arg6, arg7, 0, scale, 1, 20, 80, -1,
                             OBJECT_JYA_OBJ, (void*)0xC);
    }
    pos.x = self->dyna.actor.world.pos.x;
    pos.y = self->dyna.actor.world.pos.y + 70.0f;
    pos.z = self->dyna.actor.world.pos.z;
    FUN_0037378c(play, &pos, 100.0f, 0xA, 0x64, 0xA0, 1);
}

void BgJyaBombiwa_Update(Actor* actor, GameState* state) {
    BgJyaBombiwa* self = (BgJyaBombiwa*)actor;
    PlayState* play = (PlayState*)state;

    if (self->collider.base.acFlags & AC_HIT) {
        BgJyaBombiwa_Break(self, play);
        Flags_SetSwitch(play, actor->params & 0x3F);
        Audio_PlaySoundAtPosition(play, &actor->world.pos, 40, 0x01000167);
        Actor_Kill(actor);
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}

void BgJyaBombiwa_Draw(Actor* actor, GameState* state) {
    BgJyaBombiwa* self = (BgJyaBombiwa*)actor;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        FUN_00357fd0(GET_PLAYER((PlayState*)state), actor->unk_178, &actor->world.pos);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
    Collider_UpdateSpheres(0, &self->collider, &mtx);
}
}
