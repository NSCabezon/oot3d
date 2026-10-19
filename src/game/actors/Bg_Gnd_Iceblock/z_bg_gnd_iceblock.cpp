extern "C" {
#include "z_bg_gnd_iceblock.hpp"

#define FLAGS 0x80400030

void BgGndIceblock_Init(Actor* actor, GameState* state);
void BgGndIceblock_Destroy(Actor* actor, GameState* state);
void BgGndIceblock_Update(Actor* actor, GameState* state);
void BgGndIceblock_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Gnd_Iceblock.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Gnd_Iceblock/data_syms.s")

extern u8 BgGndIceblock_sBlockPositions[];
extern InitChainEntry BgGndIceblock_sInitChain[];
f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);
void BgGndIceblock_Fall(BgGndIceblock* self, PlayState* play);
void BgGndIceblock_Hole(BgGndIceblock* self, PlayState* play);
void BgGndIceblock_Slide(BgGndIceblock* self, PlayState* play);

s32 Player_InCsMode(PlayState* play);
void BgGndIceblock_Idle(BgGndIceblock* self, PlayState* play);
void BgGndIceblock_Reset(BgGndIceblock* self, PlayState* play);

ActorInit Bg_Gnd_Iceblock_InitVars = {
    ACTOR_BG_GND_ICEBLOCK,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DEMO_KEKKAI,
    sizeof(BgGndIceblock),
    (ActorFunc)BgGndIceblock_Init,
    (ActorFunc)BgGndIceblock_Destroy,
    (ActorFunc)BgGndIceblock_Update,
    (ActorFunc)BgGndIceblock_Draw,
};

void BgGndIceblock_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgGndIceblock* self = (BgGndIceblock*)thisx;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zarInfo;

    Actor_ProcessInitChain(thisx, BgGndIceblock_sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    zarInfo = FUN_00372f38(thisx, play, &self->model, 3, 0);
    self->model->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(0xF));
    self->model->unk_0C->SetAnimType(1);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc((DynaPolyActor*)thisx, state, 0), &colHeader);
    self->targetPos = thisx->home.pos;
    self->actionFunc = BgGndIceblock_Idle;
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    if (thisx->world.pos.x == 2792.0f) {
        thisx->params = 0;
        BgGndIceblock_sBlockPositions[0] = 7;
    } else if (thisx->world.pos.x == 3032.0f) {
        thisx->params = 1;
        BgGndIceblock_sBlockPositions[1] = 14;
    }
}

void BgGndIceblock_Destroy(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    BgGndIceblock* self = (BgGndIceblock*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(thisx, &self->model, 0);
}

void BgGndIceblock_SetPosition(BgGndIceblock* self, s32 blockPosition) {
    Actor* thisx = &self->dyna.actor;
    u8 xPosIdx[22] = {
        0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 6, 6, 6, 6, 6, 6,
    };
    u8 zPosIdx[22] = {
        5, 4, 3, 2, 1, 0, 5, 4, 2, 1, 6, 1, 0, 5, 3, 2, 5, 4, 3, 2, 1, 0,
    };

    BgGndIceblock_sBlockPositions[thisx->params] = blockPosition;
    self->targetPos.x = 2552.0f + (xPosIdx[blockPosition] * 120.0f);
    self->targetPos.z = -540.0f - (zPosIdx[blockPosition] * 120.0f);
}

static s32 BgGndIceblock_CheckForBlock(s32 blockPosition, u8 a, u8 b) {
    return blockPosition == a || blockPosition == b;
}

static void BgGndIceblock_SetNextPosition(BgGndIceblock* self) {
    u8 a = BgGndIceblock_sBlockPositions[0];
    u8 b = BgGndIceblock_sBlockPositions[1];
    if (self->dyna.unk_1B0 == 0) {
        switch (BgGndIceblock_sBlockPositions[self->dyna.actor.params]) {
            case 3:
            case 4:
                BgGndIceblock_SetPosition(self, 5);
                break;
            case 7:
                if (BgGndIceblock_CheckForBlock(8, a, b)) {
                    BgGndIceblock_SetPosition(self, 9);
                } else {
                    BgGndIceblock_SetPosition(self, 8);
                }
                break;
            case 11:
                BgGndIceblock_SetPosition(self, 12);
                break;
            case 14:
                BgGndIceblock_SetPosition(self, 15);
                break;
            case 18:
            case 19:
                BgGndIceblock_SetPosition(self, 20);
                break;
        }
    } else if (self->dyna.unk_1B0 == -0x8000) {
        switch (BgGndIceblock_sBlockPositions[self->dyna.actor.params]) {
            case 2:
            case 3:
                BgGndIceblock_SetPosition(self, 1);
                break;
            case 7:
            case 9:
                BgGndIceblock_SetPosition(self, 6);
                break;
            case 11:
                BgGndIceblock_SetPosition(self, 10);
                break;
            case 14:
            case 15:
                BgGndIceblock_SetPosition(self, 13);
                break;
            case 17:
                BgGndIceblock_SetPosition(self, 16);
                break;
            case 18:
                if (!BgGndIceblock_CheckForBlock(17, a, b)) {
                    BgGndIceblock_SetPosition(self, 16);
                }
                break;
        }
    } else if (self->dyna.unk_1B0 == 0x4000) {
        switch (BgGndIceblock_sBlockPositions[self->dyna.actor.params]) {
            case 6:
                BgGndIceblock_SetPosition(self, 13);
                break;
            case 7:
                BgGndIceblock_SetPosition(self, 17);
                break;
            case 9:
                BgGndIceblock_SetPosition(self, 11);
                break;
            case 12:
                BgGndIceblock_SetPosition(self, 21);
                break;
            case 14:
                BgGndIceblock_SetPosition(self, 18);
                break;
            case 15:
                BgGndIceblock_SetPosition(self, 19);
                break;
        }
    } else if (self->dyna.unk_1B0 == -0x4000) {
        switch (BgGndIceblock_sBlockPositions[self->dyna.actor.params]) {
            case 6:
                BgGndIceblock_SetPosition(self, 0);
                break;
            case 7:
                BgGndIceblock_SetPosition(self, 1);
                break;
            case 9:
            case 11:
                BgGndIceblock_SetPosition(self, 4);
                break;
            case 14:
                BgGndIceblock_SetPosition(self, 2);
                break;
            case 15:
                if (!BgGndIceblock_CheckForBlock(8, a, b)) {
                    BgGndIceblock_SetPosition(self, 8);
                } else {
                    BgGndIceblock_SetPosition(self, 3);
                }
                break;
        }
    }
}

void BgGndIceblock_Idle(BgGndIceblock* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (self->dyna.unk_1A8 != 0.0f) {
        player->stateFlags2 &= ~0x10;
        if (self->dyna.unk_1A8 > 0.0f) {
            BgGndIceblock_SetNextPosition(self);
            if (Actor_WorldDistXZToPoint(&self->dyna.actor, &self->targetPos) > 1.0f) {
                FUN_0036e980(play, &self->dyna.actor, 8);
                self->actionFunc = BgGndIceblock_Slide;
            }
        }
        self->dyna.unk_1A8 = 0.0f;
    }
}

// Unmatched (BgGndIceblock_Slide, 2 words + 1 literal-pool entry): ROM builds sWhite as `sGray - 4` at both
// FUN_00366150 call sites and reloads the gray literal each time (pool shares ONE 0x510FFA entry). armcc CSEs the
// literal into r7 when both sites use the same symbol; an EQU alias for the 2nd site (sGrayB) matches the code
// but adds a 2nd pool entry (size +4). Needs real adjacent statics (sWhite/sGray) to reproduce.
GLOBAL_ASM("asm/BgGndIceblock_Slide.s")

#if 0
extern Color_RGBA8 BgGndIceblock_sGray;
extern Color_RGBA8 BgGndIceblock_sGrayB; // alias of sGray (EQU 0x00510FFA)
extern Vec3f BgGndIceblock_sZeroVec;
#define WHITE2 ((Color_RGBA8*)((u32)&BgGndIceblock_sGray - 4))
#define WHITE3 ((Color_RGBA8*)((u32)&BgGndIceblock_sGrayB - 4))
void FUN_00366150(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor,
                  Color_RGBA8* envColor, s32 life, s32 scale);
s16 Rand_S16Offset(s16 base, s16 range);
void BgGndIceblock_Slide(BgGndIceblock* self, PlayState* play) {
    s32 atTarget;
    Vec3f pos;
    Vec3f velocity;
    f32 spread;
    Actor* thisx = &self->dyna.actor;

    Math_StepToF(&thisx->speedXZ, 10.0f, 0.5f);
    atTarget = Math_StepToF(&thisx->world.pos.x, self->targetPos.x, thisx->speedXZ);
    atTarget &= Math_StepToF(&thisx->world.pos.z, self->targetPos.z, thisx->speedXZ);
    if (atTarget) {
        thisx->speedXZ = 0.0f;
        self->targetPos.x = thisx->world.pos.x;
        self->targetPos.z = thisx->world.pos.z;
        Audio_PlayActorSound2(thisx, 0x0100018C);
        s32 act;
        switch (BgGndIceblock_sBlockPositions[thisx->params]) {
            case 0:
            case 1:
            case 5:
            case 13:
            case 16:
            case 20:
            case 21:
                act = 1;
                break;
            case 8:
                act = 2;
                break;
            default:
                act = 0;
                break;
        }
        switch (act) {
            case 0:
                self->actionFunc = BgGndIceblock_Idle;
                FUN_0036e980(play, &self->dyna.actor, 7);
                break;
            case 1:
                self->actionFunc = BgGndIceblock_Fall;
                break;
            case 2:
                self->actionFunc = BgGndIceblock_Hole;
                break;
        }
    } else if (thisx->speedXZ > 6.0f) {
        f32 r = 120.0f;
        spread = Rand_CenteredFloat(r);
        velocity.x = -(1.5f + Rand_ZeroOne()) * Math_SinS(self->dyna.unk_1B0);
        velocity.y = Rand_ZeroOne() + 1.0f;
        velocity.z = -(1.5f + Rand_ZeroOne()) * Math_CosS(self->dyna.unk_1B0);
        pos.x = thisx->world.pos.x - (60.0f * Math_SinS(self->dyna.unk_1B0)) - (spread * Math_CosS(self->dyna.unk_1B0));
        pos.z = thisx->world.pos.z - (60.0f * Math_CosS(self->dyna.unk_1B0)) + (spread * Math_SinS(self->dyna.unk_1B0));
        pos.y = thisx->world.pos.y;
        FUN_00366150(play, &pos, &velocity, &BgGndIceblock_sZeroVec, WHITE2, &BgGndIceblock_sGray, 250, Rand_S16Offset(40, 15));
        spread = Rand_CenteredFloat(r);
        pos.x = thisx->world.pos.x - (60.0f * Math_SinS(self->dyna.unk_1B0)) + (spread * Math_CosS(self->dyna.unk_1B0));
        pos.z = thisx->world.pos.z - (60.0f * Math_CosS(self->dyna.unk_1B0)) - (spread * Math_SinS(self->dyna.unk_1B0));
        FUN_00366150(play, &pos, &velocity, &BgGndIceblock_sZeroVec, WHITE3, &BgGndIceblock_sGrayB, 250, Rand_S16Offset(40, 15));
        FUN_00373264(thisx, 0x010000E0);
    }
}
#endif

void BgGndIceblock_Fall(BgGndIceblock* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;

    thisx->velocity.y += 1.0f;
    if (Math_StepToF(&thisx->world.pos.y, thisx->home.pos.y - 300.0f, thisx->velocity.y)) {
        thisx->velocity.y = 0.0f;
        thisx->world.pos.x = thisx->home.pos.x;
        thisx->world.pos.y = thisx->home.pos.y - 100.0f;
        thisx->world.pos.z = thisx->home.pos.z;
        if (Player_InCsMode(play)) {
            FUN_0036e980(play, thisx, 7);
        }
        self->actionFunc = BgGndIceblock_Reset;
    }
}

void BgGndIceblock_Hole(BgGndIceblock* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;

    thisx->velocity.y += 1.0f;
    if (Math_StepToF(&thisx->world.pos.y, thisx->home.pos.y - 100.0f, thisx->velocity.y)) {
        thisx->velocity.y = 0.0f;
        if (Player_InCsMode(play)) {
            FUN_0036e980(play, thisx, 7);
        }
        self->actionFunc = BgGndIceblock_Idle;
    }
}

void BgGndIceblock_Reset(BgGndIceblock* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Actor* thisx = &self->dyna.actor;

    if (self->dyna.unk_1A8 != 0.0f) {
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_1A8 = 0.0f;
    }
    if (Math_StepToF(&thisx->world.pos.y, thisx->home.pos.y, 1.0f)) {
        self->targetPos = thisx->home.pos;
        thisx->speedXZ = 0.0f;
        self->actionFunc = BgGndIceblock_Idle;
        if (thisx->params == 0) {
            BgGndIceblock_sBlockPositions[0] = 7;
        } else if (thisx->params == 1) {
            BgGndIceblock_sBlockPositions[1] = 14;
        }
    }
}

void BgGndIceblock_Update(Actor* thisx, GameState* state) {
    BgGndIceblock* self = (BgGndIceblock*)thisx;

    self->actionFunc(self, (PlayState*)state);
}

void BgGndIceblock_Draw(Actor* thisx, GameState* state) {
    BgGndIceblock* self = (BgGndIceblock*)thisx;
    MTX34 modelMtx;
    f32 speed;

    MTX34CopyAsm(&modelMtx, &thisx->modelMtx);
    speed = 1.0f;
    if (PauseContext_GetState() != 0) {
        speed = 0.0f;
    }
    if (self->model != NULL) {
        self->model->unk_0C->animSpeed = speed;
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&modelMtx);
        self->model->FUN_00372170(0);
    }
}

}
