extern "C" {
#include "z_bg_spot11_oasis.hpp"

#define FLAGS 0x80400010

void BgSpot11Oasis_Init(Actor* actor, GameState* state);
void BgSpot11Oasis_Destroy(Actor* actor, GameState* state);
void BgSpot11Oasis_Update(Actor* actor, GameState* state);

void FUN_003bf500(BgSpot11Oasis* self, PlayState* play);
void FUN_003af468(BgSpot11Oasis* self, PlayState* play);
void FUN_003b307c(BgSpot11Oasis* self, PlayState* play);
void FUN_002ad270(Actor* actor, GameState* state);

s32 Flags_GetEnv(PlayState* play, s32 flag);
void OnePointCutscene_Init(PlayState* play, s16 csId, s16 arg2, Actor* actor, s32 camId);
s32 FUN_002a8bc0(Vec3f* a, Vec3f* b, Vec3f* c, f32 z, f32 x);
void EffectSsBubble_Spawn(PlayState* play, Vec3f* pos, f32 arg2, f32 arg3, f32 arg4, f32 scale);

ActorInit Bg_Spot11_Oasis_InitVars = {
    ACTOR_BG_SPOT11_OASIS,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT11_OBJ,
    sizeof(BgSpot11Oasis),
    (ActorFunc)BgSpot11Oasis_Init,
    (ActorFunc)BgSpot11Oasis_Destroy,
    (ActorFunc)BgSpot11Oasis_Update,
    NULL,
};

static s16 sOasisPolygon[][2] = {
    { 1260, 2040 }, { 1259, 1947 }, { 1135, 1860 }, { 1087, 1912 }, { 1173, 2044 },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 1, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

static Vec3f sBubbleSpawnOffsets[] = {
    { 0.0f, -100.0f, 0.0f },   { 100.0f, -80.0f, -50.0f }, { -50.0f, -80.0f, -100.0f },
    { -75.0f, -90.0f, 90.0f }, { 30.0f, -100.0f, 40.0f },
};

static inline void BgSpot11Oasis_SetWaterLevel(PlayState* play, s16 waterSurface) {
    WaterBox* waterBox = &play->colCtx.stat.colHeader->waterBoxes[0];

    waterBox->ySurface = waterSurface;
}

void BgSpot11Oasis_Init(Actor* actor, GameState* state) {
    BgSpot11Oasis* self = (BgSpot11Oasis*)actor;
    PlayState* play = (PlayState*)state;

    Actor_ProcessInitChain(actor, sInitChain);
    ZARInfo* zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    self->skelAnimModel->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0));
    self->skelAnimModel->unk_0C->SetAnimType(1);
    self->actionFunc = FUN_003bf500;
    actor->world.pos.y = -100.0f;
    BgSpot11Oasis_SetWaterLevel(play, -100);
}

void BgSpot11Oasis_Destroy(Actor* actor, GameState* state) {
    BgSpot11Oasis* self = (BgSpot11Oasis*)actor;

    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgSpot11Oasis_Update(Actor* actor, GameState* state) {
    BgSpot11Oasis* self = (BgSpot11Oasis*)actor;
    PlayState* play = (PlayState*)state;
    u32 gameplayFrames;

    self->actionFunc(self, play);
    if (self->actionFunc == FUN_003bf500) {
        actor->draw = NULL;
        return;
    }
    actor->draw = FUN_002ad270;
    if (self->unk_1A8 && (self->actor.unk_EC.z < 400.0f) && (self->actor.unk_EC.z > -40.0f)) {
        gameplayFrames = *(u32*)((u8*)play + 0x5BF4); // gameplayFrames
        if (gameplayFrames & 4) {
            Vec3f sp30;
            Vec3f* offset = &sBubbleSpawnOffsets[self->bubbleSpawnIdx];

            sp30.x = self->actor.world.pos.x + offset->x;
            sp30.y = self->actor.world.pos.y + offset->y;
            sp30.z = self->actor.world.pos.z + offset->z;
            EffectSsBubble_Spawn(play, &sp30, 0.0f, 15.0f, 50.0f, (Rand_ZeroOne() * 0.12f) + 0.02f);
            if (Rand_ZeroOne() < 0.3f) {
                self->bubbleSpawnIdx = Rand_ZeroOne() * 4.9f;
            }
        }
    } else {
        self->unk_1A8 = 1;
    }
}

void FUN_003bf500(BgSpot11Oasis* self, PlayState* play) {
    Player* player;
    Vec3f sp58;
    Vec3f sp4C;
    Vec3f sp40;
    s32 i;

    if (Flags_GetEnv(play, 5)) {
        player = GET_PLAYER(play);
        sp58.x = sOasisPolygon[0][0];
        sp58.z = sOasisPolygon[0][1];

        sp40.y = 0.0f;
        sp4C.y = 0.0f;
        sp58.y = 0.0f;

        for (i = 1; i < ARRAY_COUNT(sOasisPolygon) - 1; i++) {
            sp4C.x = sOasisPolygon[i][0];
            sp4C.z = sOasisPolygon[i][1];
            sp40.x = sOasisPolygon[i + 1][0];
            sp40.z = sOasisPolygon[i + 1][1];
            if (FUN_002a8bc0(&sp58, &sp4C, &sp40, player->actor.world.pos.z, player->actor.world.pos.x)) {
                OnePointCutscene_Init(play, 4150, -99, &self->actor, 0);
                self->actionFunc = FUN_003af468;
                return;
            }
        }
    }
}

void FUN_003af468(BgSpot11Oasis* self, PlayState* play) {
    if (Math_StepToF(&self->actor.world.pos.y, 0.0f, 0.7f)) {
        self->actionFunc = FUN_003b307c;
        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, self->actor.world.pos.x, self->actor.world.pos.y + 40.0f,
                    self->actor.world.pos.z, 0, 0, 0, 4, 1);
        FUN_00372244(&play->unk_5FCC, 30, 0x01000487);
    }
    play->colCtx.stat.colHeader->waterBoxes[0].ySurface = self->actor.world.pos.y;
}

void FUN_003b307c(BgSpot11Oasis* self, PlayState* play) {
}

void FUN_002ad270(Actor* actor, GameState* state) {
    BgSpot11Oasis* self = (BgSpot11Oasis*)actor;
    f32 animSpeed;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    animSpeed = 1.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }
    self->skelAnimModel->unk_0C->SetAnimSpeed(animSpeed);
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
