extern "C" {
#include "z_door_killer.hpp"

#define FLAGS 0x80000010
#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

typedef enum DoorKillerBehaviour {
    /* 0 */ DOOR_KILLER_DOOR,
    /* 1 */ DOOR_KILLER_RUBBLE_PIECE_1,
    /* 2 */ DOOR_KILLER_RUBBLE_PIECE_2,
    /* 3 */ DOOR_KILLER_RUBBLE_PIECE_3,
    /* 4 */ DOOR_KILLER_RUBBLE_PIECE_4
} DoorKillerBehaviour;

void Collider_DestroyJntSph(PlayState* play, DoorKillerColliderJntSph* collider);
void FUN_00141da4(DoorKiller* self, PlayState* play);

void DoorKiller_Init(Actor* thisx, PlayState* play);
void DoorKiller_Destroy(Actor* thisx, PlayState* play);
void DoorKiller_Update(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_Door_Killer.data.s")

ActorInit Door_Killer_InitVars = {
    ACTOR_DOOR_KILLER,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DOOR_KILLER,
    sizeof(DoorKiller),
    (ActorFunc)DoorKiller_Init,
    (ActorFunc)DoorKiller_Destroy,
    (ActorFunc)DoorKiller_Update,
    NULL,
};

GLOBAL_ASM("asm/DoorKiller_Init.s")

void DoorKiller_Destroy(Actor* thisx, PlayState* play) {
    DoorKiller* self = (DoorKiller*)thisx;

    switch ((u8)PARAMS_GET_U(self->actor.params, 0, 8)) {
        case DOOR_KILLER_DOOR:
            Collider_DestroyCylinder(play, &self->colliderCylinder);
            Collider_DestroyJntSph(play, &self->colliderJntSph);
            SkelAnime_Free2(&self->skelAnime);
            ((FaceAnimations*)&self->unk_568)->Destroy();
            break;
        case DOOR_KILLER_RUBBLE_PIECE_1:
        case DOOR_KILLER_RUBBLE_PIECE_2:
        case DOOR_KILLER_RUBBLE_PIECE_3:
        case DOOR_KILLER_RUBBLE_PIECE_4:
            FUN_00350f34(&self->actor, &self->unk_734, 0);
            ((FaceAnimations*)&self->unk_738)->Destroy();
            break;
    }
}

void FUN_0010cfd0(DoorKiller* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f playerPosRelToDoor;

    FUN_0036c5d8(&self->actor, &playerPosRelToDoor, &player->actor.world.pos);
    if (self->playerIsOpening) {
        self->actionFunc = FUN_00141da4;
        self->timer = 20;
        self->playerIsOpening = false;
    }
}

typedef struct DoorKillerDieState {
    u8 unk_00[0x67];
    s8 dieCounter;
    u8 switchFlagSet;
} DoorKillerDieState;

void FUN_003ebe60(DoorKiller* self, PlayState* play) {
    DoorKillerDieState* d = (DoorKillerDieState*)((u8*)self + 0x900);
    s16 params = self->actor.params;

    if ((((params >> 8) & 0x3F) != 0x3F) && !d->switchFlagSet) {
        Flags_SetSwitch(play, (params >> 8) & 0x3F);
        self->switchFlagSet = true;
    }
    if (d->dieCounter >= 30) {
        Actor_Kill(&self->actor);
        return;
    }
    self->dieCounter++;
    self->actor.draw = NULL;
    self->actor.flags &= ~1;
}

// FUN_0010d270 (3DS DoorKiller_Wobble), 131 of 188 words differ (far). The 3DS code inlines Matrix_RotateAxis
// (FUN_003625f8) for jointMtx[1..7] (axis Y then Z; the unit axis is const-folded but the omc*ax/ay/az products stay
// runtime). ROM keeps 0/1/pi-per-s16/40.74/0.0245 in s16-s20 loaded BEFORE the loop with vmov copies for the
// per-iteration axis components; this draft hoists s21-s23 copies instead (size 716 vs 752 B). Same class as the
// En_Syateki_Niw RotateAxis blocker; FUN_001419ac / FUN_00141458 use the same inline with different VFP numbering.
#if 0
void FUN_0036c174(MTX34* dst, MTX34* a, MTX34* b);
void FUN_0036c258(f32* sinOut, f32* cosOut, f32 angle);
void FUN_00141458(DoorKiller* self, PlayState* play);

// 3DS Matrix_RotateAxis (FUN_003625f8) inlined with a constant unit axis (no sqrt/div).
static inline void DoorKiller_RotateAxis(MTX34* out, f32 ax, f32 ay, f32 az, f32 angle) {
    f32 sinA;
    f32 cosA;
    f32 omc;
    f32 tAx;
    f32 tAxAy;
    f32 tAxAz;
    f32 tAyP;
    f32 tAyAz;

    FUN_0036c258(&sinA, &cosA, angle * 40.743664f * 0.024543693f * 40.743664f);
    omc = 1.0f - cosA;
    tAx = omc * ax;
    tAxAy = tAx * ay;
    tAxAz = tAx * az;
    tAyP = omc * ay;
    tAyAz = tAyP * az;
    out->data[0][0] = cosA + omc * (ax * ax);
    out->data[0][1] = tAxAy - sinA * az;
    out->data[0][2] = tAxAz + sinA * ay;
    out->data[0][3] = 0.0f;
    out->data[1][0] = tAxAy + sinA * az;
    out->data[1][1] = cosA + omc * (ay * ay);
    out->data[1][2] = tAyAz - sinA * ax;
    out->data[1][3] = 0.0f;
    out->data[2][0] = tAxAz - sinA * ay;
    out->data[2][1] = tAyAz + sinA * ax;
    out->data[2][2] = cosA + omc * (az * az);
    out->data[2][3] = 0.0f;
}

// DoorKiller_Wobble
void FUN_0010d270(DoorKiller* self, PlayState* play) {
    s16 rotation;
    s32 i;
    MTX34 m;

    if ((self->timer == 24) || (self->timer == 12)) {
        Audio_PlayActorSound2(&self->actor, 0x0100044B);
    }
    if (self->timer != 0) {
        self->timer--;
    } else {
        self->actionFunc = FUN_00141458;
        self->timer = 12;
        self->hasHitPlayerOrGround &= ~1;
        return;
    }
    rotation = Math_SinS(self->timer * 0x1554) * self->timer * 100.0f * 0.6666667f;
    for (i = 1; i < 8; i++) {
        DoorKiller_RotateAxis(&m, 0.0f, 1.0f, 0.0f, (f32)rotation * 0.000095873802f);
        FUN_0036c174(&self->joints[i].m, &self->joints[i].m, &m);
    }
    rotation = (s16)(Math_CosS(self->timer * 0x1554) * -1000.0f + 1000.0f);
    for (i = 1; i < 8; i++) {
        DoorKiller_RotateAxis(&m, 0.0f, 0.0f, 1.0f, (f32)rotation * 0.000095873802f);
        FUN_0036c174(&self->joints[i].m, &self->joints[i].m, &m);
    }
}

#endif

GLOBAL_ASM("asm/DoorKiller_Update.s")

void FUN_00196208(Actor* thisx, PlayState* play) {
    Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_KILLER, thisx->world.pos.x, thisx->world.pos.y + 9.0f,
                thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y, thisx->shape.rot.z,
                DOOR_KILLER_RUBBLE_PIECE_1, 1);
    Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_KILLER, thisx->world.pos.x + 7.88f, thisx->world.pos.y + 39.8f,
                thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y, thisx->shape.rot.z,
                DOOR_KILLER_RUBBLE_PIECE_2, 1);
    Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_KILLER, thisx->world.pos.x - 15.86f, thisx->world.pos.y + 61.98f,
                thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y, thisx->shape.rot.z,
                DOOR_KILLER_RUBBLE_PIECE_3, 1);
    Actor_Spawn(&play->actorCtx, play, ACTOR_DOOR_KILLER, thisx->world.pos.x + 3.72f, thisx->world.pos.y + 85.1f,
                thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y, thisx->shape.rot.z,
                DOOR_KILLER_RUBBLE_PIECE_4, 1);
}

}
