extern "C" {
#include "z_bg_spot17_bakudankabe.hpp"

#define FLAGS 0x80400000

#define NA_SE_EV_WALL_BROKEN 0x01000167
#define NA_SE_SY_CORRECT_CHIME 0x01000487
#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

void BgSpot17Bakudankabe_Init(Actor* actor, GameState* state);
void BgSpot17Bakudankabe_Destroy(Actor* actor, GameState* state);
void BgSpot17Bakudankabe_Update(Actor* actor, GameState* state);
void BgSpot17Bakudankabe_Draw(Actor* actor, GameState* state);

ActorInit Bg_Spot17_Bakudankabe_InitVars = {
    ACTOR_BG_SPOT17_BAKUDANKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT17_OBJ,
    sizeof(BgSpot17Bakudankabe),
    (ActorFunc)BgSpot17Bakudankabe_Init,
    (ActorFunc)BgSpot17Bakudankabe_Destroy,
    (ActorFunc)BgSpot17Bakudankabe_Update,
    (ActorFunc)BgSpot17Bakudankabe_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgSpot17Bakudankabe_Init(Actor* actor, GameState* state) {
    BgSpot17Bakudankabe* self = (BgSpot17Bakudankabe*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 0, 6))) {
        Actor_Kill(actor);
        return;
    }

    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    Actor_ProcessInitChain(actor, sInitChain);
}

void BgSpot17Bakudankabe_Destroy(Actor* actor, GameState* state) {
    BgSpot17Bakudankabe* self = (BgSpot17Bakudankabe*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

// Update: exact C. The ROM word after it (0x0039F204, `bx lr`) is a separate empty function, split out of the
// merged cfg span by scripts/cfg_split.py (split/asm/FUN_0039f204.s).
// Keys: ROM keeps `thisx` in r8 for outer calls and a separate `self` copy in r5 for the inlined N64 helper ->
// 2nd typed local `s2` used only in the helper call; raw `state` to FUN_00346d94 keeps play in r0 at entry;
// `scale` declared before `gravityInfluence` (r7/r6).
static __inline void func_808B6BC0(BgSpot17Bakudankabe* self, PlayState* play) {
    s32 i;
    Vec3f burstDepthY;
    Vec3f burstDepthX;
    f32 sinY;
    f32 cosY;

    sinY = Math_SinS(self->dyna.actor.shape.rot.y);
    cosY = Math_CosS(self->dyna.actor.shape.rot.y);

    burstDepthX.z = 0.0f;
    burstDepthX.x = 0.0f;

    for (i = 0; i < 20; i++) {
        s16 scale;
        s16 gravityInfluence;
        f32 temp1;
        f32 temp2;
        s16 rotationSpeed;

        temp1 = (Rand_ZeroOne() - 0.5f) * 140.0f;
        temp2 = (Rand_ZeroOne() - 0.5f) * 20.0f;

        burstDepthY.x = self->dyna.actor.world.pos.x + temp2 * sinY + (temp1 * cosY);
        burstDepthY.y = self->dyna.actor.world.pos.y + 30.0f + (i * 6.5f);
        burstDepthY.z = self->dyna.actor.world.pos.z + temp2 * cosY - (temp1 * sinY);

        burstDepthX.y = (Rand_ZeroOne() - 0.2f) * 12.0f;
        scale = Rand_ZeroOne() * 55.0f + 8.0f;

        if (scale < 20) {
            gravityInfluence = -300;
        } else if (scale < 35) {
            gravityInfluence = -360;
        } else {
            gravityInfluence = -420;
        }

        if (Rand_ZeroOne() < 0.4f) {
            rotationSpeed = 65;
        } else {
            rotationSpeed = 33;
        }
        EffectSsKakera_Spawn(play, &burstDepthY, &burstDepthX, &burstDepthY, gravityInfluence, rotationSpeed, 0x1E,
                             4, 0, scale, 1, 3, 80, -1, OBJECT_GAMEPLAY_FIELD_KEEP, (void*)5);
    }
    Math_Vec3f_Copy(&burstDepthY, &self->dyna.actor.world.pos);
    FUN_0037378c(play, &burstDepthY, 60.0f, 4, 110, 160, 1);
    burstDepthY.y += 40.0f;
    FUN_0037378c(play, &burstDepthY, 60.0f, 4, 120, 160, 1);
    burstDepthY.y += 40.0f;
    FUN_0037378c(play, &burstDepthY, 60.0f, 4, 110, 160, 1);
}

void BgSpot17Bakudankabe_Update(Actor* thisx, GameState* state) {
    BgSpot17Bakudankabe* self = (BgSpot17Bakudankabe*)thisx;
    PlayState* play = (PlayState*)state;

    if (thisx->xzDistToPlayer < 650.0f && FUN_00346d94((PlayState*)state, thisx) != NULL) {
        BgSpot17Bakudankabe* s2 = self;
        func_808B6BC0(s2, play);
        Flags_SetSwitch(play, PARAMS_GET_U(thisx->params, 0, 6));
        Audio_PlaySoundAtPosition(play, &thisx->world.pos, 40, NA_SE_EV_WALL_BROKEN);
        FUN_00372244((u8*)play + 0x5FCC, 30, NA_SE_SY_CORRECT_CHIME);
        Actor_Kill(thisx);
    }
}

GLOBAL_ASM("../split/asm/FUN_0039f204.s")

void BgSpot17Bakudankabe_Draw(Actor* actor, GameState* state) {
    BgSpot17Bakudankabe* self = (BgSpot17Bakudankabe*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
