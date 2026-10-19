extern "C" {
#include "z_bg_heavy_block.hpp"

#define FLAGS 0x80000000

#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

typedef enum HeavyBlockType {
    /* 0x00 */ HEAVYBLOCK_UNBREAKABLE,
    /* 0x01 */ HEAVYBLOCK_BREAKABLE,
    /* 0x02 */ HEAVYBLOCK_BIG_PIECE,
    /* 0x03 */ HEAVYBLOCK_SMALL_PIECE,
    /* 0x04 */ HEAVYBLOCK_UNBREAKABLE_OUTSIDE_CASTLE
} HeavyBlockType;

void BgHeavyBlock_Init(Actor* thisx, PlayState* play);
void BgHeavyBlock_Destroy(Actor* thisx, PlayState* play);
void BgHeavyBlock_Update(Actor* thisx, PlayState* play);
void BgHeavyBlock_Draw(Actor* actor, GameState* state);
void BgHeavyBlock_LiftedUp(BgHeavyBlock* self, PlayState* play);
void BgHeavyBlock_Fly(BgHeavyBlock* self, PlayState* play);
void FUN_0036f59c(Player* player, u32 sfxId);
void FUN_0036c684(PlayState* play, f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, u8 type);
void BgHeavyBlock_DoNothing(BgHeavyBlock* self, PlayState* play);
void Quake_SetQuakeValues(s32 idx, s16 y, s16 x, s16 zoom, s16 rollOffset);
void Quake_SetCountdown(s32 idx, s16 countdown);
s32 Quake_Add(Camera* cam, u32 type);

#define PIECE_FLAG_HIT_FLOOR (1 << 0)
#define UPDBGCHECKINFO_FLAG_0 (1 << 0)
#define UPDBGCHECKINFO_FLAG_2 (1 << 2)
#define BGCHECKFLAG_GROUND (1 << 0)

GLOBAL_ASM("data/z_Bg_Heavy_Block.data.s")

extern InitChainEntry BgHeavyBlock_sInitChain[];

ActorInit Bg_Heavy_Block_InitVars = {
    ACTOR_BG_HEAVY_BLOCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HEAVY_OBJECT,
    sizeof(BgHeavyBlock),
    (ActorFunc)BgHeavyBlock_Init,
    (ActorFunc)BgHeavyBlock_Destroy,
    (ActorFunc)BgHeavyBlock_Update,
    (ActorFunc)BgHeavyBlock_Draw,
};

// Draft: Init with inlined SetupDynapoly (3DS adds unk_19B = 2; timer 180; unk_1BC.y offset).
#if 0
void BgHeavyBlock_DrawPiece(Actor* thisx, PlayState* play);
void BgHeavyBlock_MovePiece(BgHeavyBlock* self, PlayState* play);
void BgHeavyBlock_Wait(BgHeavyBlock* self, PlayState* play);
void BgHeavyBlock_InitPiece(BgHeavyBlock* self, f32 scale);

static inline void BgHeavyBlock_SetupDynapoly(BgHeavyBlock* self, PlayState* play) {
    CollisionHeader* colHeader = NULL;

    self->dyna.actor.flags |= 0x20030;
    DynaPolyActor_Init(&self->dyna, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
    self->dyna.actor.unk_19B = 2;
}

void BgHeavyBlock_Init(Actor* thisx, PlayState* play) {
    BgHeavyBlock* self = (BgHeavyBlock*)thisx;

    Actor_ProcessInitChain(thisx, BgHeavyBlock_sInitChain);
    FUN_00372f38(thisx, play, &self->pieceModel[0], 0, &self->pieceModel[1], 2, &self->pieceModel[2], 1, 0);
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    self->pieceFlags = 0;

    if (*(u16*)((u8*)play + 0x104) == 0x64) {
        thisx->params &= 0xFF00;
        thisx->params |= 4;
    }

    switch (PARAMS_GET_U(thisx->params, 0, 8)) {
        case HEAVYBLOCK_BIG_PIECE:
            thisx->draw = (ActorFunc)BgHeavyBlock_DrawPiece;
            self->actionFunc = BgHeavyBlock_MovePiece;
            BgHeavyBlock_InitPiece(self, 1.0f);
            self->timer = 180;
            thisx->flags |= 0x10;
            self->unk_1BC.y = -50.0f;
            break;
        case HEAVYBLOCK_SMALL_PIECE:
            thisx->draw = (ActorFunc)BgHeavyBlock_DrawPiece;
            self->actionFunc = BgHeavyBlock_MovePiece;
            BgHeavyBlock_InitPiece(self, 2.0f);
            self->timer = 180;
            thisx->flags |= 0x10;
            self->unk_1BC.y = -20.0f;
            break;
        case HEAVYBLOCK_BREAKABLE:
            BgHeavyBlock_SetupDynapoly(self, play);

            if (Flags_GetSwitch(play, PARAMS_GET_U(thisx->params, 8, 6))) {
                Actor_Kill(thisx);
                return;
            }

            self->actionFunc = BgHeavyBlock_Wait;
            break;
        case HEAVYBLOCK_UNBREAKABLE_OUTSIDE_CASTLE:
            BgHeavyBlock_SetupDynapoly(self, play);

            if (Flags_GetSwitch(play, PARAMS_GET_U(thisx->params, 8, 6))) {
                self->actionFunc = BgHeavyBlock_DoNothing;
                thisx->shape.rot.x = thisx->world.rot.x = (s16)0x8AD0;
                thisx->shape.rot.y = thisx->world.rot.y = (s16)0xC000;
                thisx->shape.rot.z = thisx->world.rot.z = 0;
                thisx->world.pos.x = 1704.0f;
                thisx->world.pos.y = 1504.0f;
                thisx->world.pos.z = 516.0f;
            }

            self->actionFunc = BgHeavyBlock_Wait;
            break;
        case HEAVYBLOCK_UNBREAKABLE:
            BgHeavyBlock_SetupDynapoly(self, play);
            self->actionFunc = BgHeavyBlock_Wait;
            break;
        default:
            BgHeavyBlock_SetupDynapoly(self, play);
            self->actionFunc = BgHeavyBlock_Wait;
            break;
    }
}
#endif
GLOBAL_ASM("asm/BgHeavyBlock_Init.s")

void BgHeavyBlock_Destroy(Actor* thisx, PlayState* play) {
    BgHeavyBlock* self = (BgHeavyBlock*)thisx;

    switch (PARAMS_GET_U(thisx->params, 0, 8)) {
        case HEAVYBLOCK_BIG_PIECE:
        case HEAVYBLOCK_SMALL_PIECE:
            break;
        default:
            DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
            break;
    }
    FUN_00350f34(thisx, &self->pieceModel[0], &self->pieceModel[1], &self->pieceModel[2], 0);
}

void BgHeavyBlock_Update(Actor* thisx, PlayState* play) {
    BgHeavyBlock* self = (BgHeavyBlock*)thisx;

    self->actionFunc(self, play);
}

// Draft (73 words off, own TU needed for the guarded statics: guards 0x005115F0/F4 (first 8 bytes of this actor's data
// chunk), objects 0x005699B0/0x005699BC). Shape remaining: the two Matrix_Translate tails are cross-jumped in ROM and
// negated Vec3f loads are interleaved (x,z,neg x,y,...); `mov r5,#1` hoisted before the compare (SetUnkAC family).
#if 0
void BgHeavyBlock_LiftedUp(BgHeavyBlock* self, PlayState* play);
void BgHeavyBlock_Land(BgHeavyBlock* self, PlayState* play);
void FUN_003679d0(f32 x, f32 y, f32 z, MTX34* mtx, Vec3s* rot);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);

static inline void BgHeavyBlock_Translate(MTX34* mtx, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    Matrix_UpdatePosition(mtx, mtx, &t);
}

void BgHeavyBlock_Draw(Actor* actor, GameState* state) {
    BgHeavyBlock* self = (BgHeavyBlock*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);
    static Vector3f D_80884EC8(0.0f, 0.0f, 0.0f);
    static Vector3f D_80884ED4(0.0f, 400.0f, 0.0f);
    u8* player = (u8*)GET_PLAYER(play);
    if (self->actionFunc == BgHeavyBlock_LiftedUp) {
        FUN_003679d0(*(f32*)(player + 0x1228), *(f32*)(player + 0x122C), *(f32*)(player + 0x1230), &mtx,
                     &self->dyna.actor.shape.rot);
        BgHeavyBlock_Translate(&mtx, -self->unk_1BC.x, -self->unk_1BC.y, -self->unk_1BC.z);
    } else if ((self->dyna.actor.gravity == 0.0f) && (self->actionFunc == BgHeavyBlock_Land)) {
        FUN_003679d0(self->dyna.actor.home.pos.x, self->dyna.actor.home.pos.y, self->dyna.actor.home.pos.z, &mtx,
                     &self->dyna.actor.shape.rot);
        BgHeavyBlock_Translate(&mtx, -D_80884ED4.x, -D_80884ED4.y, -D_80884ED4.z);
    }
    VEC3TransformAsm((Vector3f*)&self->dyna.actor.world.pos, &mtx, &D_80884EC8);
    VEC3TransformAsm((Vector3f*)&self->dyna.actor.home.pos, &mtx, &D_80884ED4);
    if (self->pieceModel[0] != NULL) {
        self->pieceModel[0]->SetUnkAC(1);
        self->pieceModel[0]->FUN_003721e0(&mtx);
        self->pieceModel[0]->FUN_00372170(0);
    }
}
#endif
GLOBAL_ASM("asm/BgHeavyBlock_Draw.s")

void BgHeavyBlock_DoNothing(BgHeavyBlock* self, PlayState* play) {
}

GLOBAL_ASM("asm/BgHeavyBlock_DrawPiece.s")

// Draft: Fly with the inlined SpawnPieces (local Vec3f[6] copied with memcpy4 from rodata 0x4D70F8).
#if 0
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor,
                                Vec3f* pos);
void Quake_SetSpeed(s32 idx, s16 speed);
void Audio_PlaySoundAtPosition(PlayState* play, Vec3f* pos, s32 duration, u32 sfxId);
void BgHeavyBlock_Land(BgHeavyBlock* self, PlayState* play);

typedef struct {
    Vec3f v[6];
} BgHeavyBlockPieceOffsets;
extern const BgHeavyBlockPieceOffsets BgHeavyBlock_sPieceOffsets;

void BgHeavyBlock_Fly(BgHeavyBlock* self, PlayState* play) {
    s32 bgId;
    s32 quakeIndex;
    Vec3f checkPos;
    f32 yIntersect;

    Actor_MoveForward(&self->dyna.actor);
    checkPos.x = self->dyna.actor.home.pos.x;
    checkPos.y = self->dyna.actor.home.pos.y + 1000.0f;
    checkPos.z = self->dyna.actor.home.pos.z;
    yIntersect = BgCheck_EntityRaycastFloor4(&play->colCtx, &self->dyna.actor.floorPoly, &bgId, &self->dyna.actor,
                                             &checkPos);
    self->dyna.actor.floorHeight = yIntersect;

    if (self->dyna.actor.home.pos.y <= yIntersect) {
        switch (PARAMS_GET_U(self->dyna.actor.params, 0, 8)) {
            case HEAVYBLOCK_BREAKABLE: {
                BgHeavyBlockPieceOffsets spA4 = BgHeavyBlock_sPieceOffsets;
                Vec3f pos;
                f32 sinPitch;
                f32 cosPitch;
                f32 sinYaw;
                f32 cosYaw;
                s32 i;

                sinPitch = Math_SinS(self->dyna.actor.world.rot.x);
                cosPitch = Math_CosS(self->dyna.actor.world.rot.x);
                sinYaw = Math_SinS(self->dyna.actor.world.rot.y);
                cosYaw = Math_CosS(self->dyna.actor.world.rot.y);

                for (i = 0; i < 6; i++) {
                    pos.z = (spA4.v[i].y * sinPitch) + (spA4.v[i].z * cosPitch);

                    pos.x = self->dyna.actor.world.pos.x + (spA4.v[i].x * cosYaw) + (sinYaw * pos.z);
                    pos.y = self->dyna.actor.world.pos.y + (spA4.v[i].y * cosPitch) + (-spA4.v[i].z * sinPitch);
                    pos.z = self->dyna.actor.world.pos.z + (spA4.v[i].x * -sinYaw) + (cosYaw * pos.z);

                    Actor_Spawn(&play->actorCtx, play, ACTOR_BG_HEAVY_BLOCK, pos.x, pos.y, pos.z,
                                self->dyna.actor.shape.rot.x, self->dyna.actor.shape.rot.y, 0, 2, 1);
                    Actor_Spawn(&play->actorCtx, play, ACTOR_BG_HEAVY_BLOCK, pos.x, pos.y, pos.z,
                                self->dyna.actor.shape.rot.x, self->dyna.actor.shape.rot.y, 0, 3, 1);

                    FUN_0036c684(play, pos.x, pos.y, pos.z, 0.0f, 0.0f, 0.0f, 0);
                }
                Flags_SetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6));
                Actor_Kill(&self->dyna.actor);

                quakeIndex = Quake_Add(((Camera**)((u8*)play + 0xA54))[*(s16*)((u8*)play + 0xA64)], 3);
                Quake_SetSpeed(quakeIndex, 28000);
                Quake_SetQuakeValues(quakeIndex, 14, 2, 100, 0);
                Quake_SetCountdown(quakeIndex, 30);

                quakeIndex = Quake_Add(((Camera**)((u8*)play + 0xA54))[*(s16*)((u8*)play + 0xA64)], 2);
                Quake_SetSpeed(quakeIndex, 12000);
                Quake_SetQuakeValues(quakeIndex, 5, 0, 0, 0);
                Quake_SetCountdown(quakeIndex, 999);

                Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 30, 0x01000238);
                return;
            }
            case HEAVYBLOCK_UNBREAKABLE_OUTSIDE_CASTLE:
                Audio_PlayActorSound2(&self->dyna.actor, 0x01000174);

                quakeIndex = Quake_Add(((Camera**)((u8*)play + 0xA54))[*(s16*)((u8*)play + 0xA64)], 3);
                Quake_SetSpeed(quakeIndex, 28000);
                Quake_SetQuakeValues(quakeIndex, 16, 2, 120, 0);
                Quake_SetCountdown(quakeIndex, 40);

                self->actionFunc = BgHeavyBlock_Land;
                Flags_SetSwitch(play, PARAMS_GET_U(self->dyna.actor.params, 8, 6));
                break;
            case HEAVYBLOCK_UNBREAKABLE:
                Audio_PlayActorSound2(&self->dyna.actor, 0x010001F6);

                quakeIndex = Quake_Add(((Camera**)((u8*)play + 0xA54))[*(s16*)((u8*)play + 0xA64)], 3);
                Quake_SetSpeed(quakeIndex, 28000);
                Quake_SetQuakeValues(quakeIndex, 14, 2, 100, 0);
                Quake_SetCountdown(quakeIndex, 40);

                self->actionFunc = BgHeavyBlock_Land;
                break;
            default:
                quakeIndex = Quake_Add(((Camera**)((u8*)play + 0xA54))[*(s16*)((u8*)play + 0xA64)], 3);
                Quake_SetSpeed(quakeIndex, 28000);
                Quake_SetQuakeValues(quakeIndex, 14, 2, 100, 0);
                Quake_SetCountdown(quakeIndex, 40);

                self->actionFunc = BgHeavyBlock_Land;
        }
    }
    self->dyna.actor.shape.rot.x = Math_Atan2S(self->dyna.actor.velocity.y, self->dyna.actor.speedXZ);
}
#endif
GLOBAL_ASM("asm/BgHeavyBlock_Fly.s")

static inline void BgHeavyBlock_SetPieceRandRot(BgHeavyBlock* self, f32 scale) {
    self->dyna.actor.world.rot.x = Rand_CenteredFloat(1024.0f) * scale;
    self->dyna.actor.world.rot.y = Rand_CenteredFloat(1024.0f) * scale;
    self->dyna.actor.world.rot.z = Rand_CenteredFloat(1024.0f) * scale;
}

void BgHeavyBlock_InitPiece(BgHeavyBlock* self, f32 scale) {
    f32 rand;
    f32 yawSinCos;
    f32 randChoice;
    f32 xTerm;
    f32 zTerm;

    self->dyna.actor.gravity = -0.6f;
    self->dyna.actor.minVelocityY = -12.0f;
    randChoice = Rand_CenteredFloat(12.0f * scale);
    rand = (randChoice < 0.0f) ? randChoice - 2.0f : randChoice + 2.0f;
    self->dyna.actor.velocity.y = (Rand_ZeroFloat(8.0f) + 4.0f) * scale;
    self->dyna.actor.velocity.z = Rand_ZeroFloat(-8.0f * scale);
    yawSinCos = Math_CosS(self->dyna.actor.world.rot.y);
    xTerm = yawSinCos * rand;
    self->dyna.actor.velocity.x = (Math_SinS(self->dyna.actor.world.rot.y) * self->dyna.actor.velocity.z + xTerm);
    zTerm = Math_CosS(self->dyna.actor.world.rot.y) * self->dyna.actor.velocity.z;
    yawSinCos = Math_SinS(self->dyna.actor.world.rot.y);
    self->dyna.actor.velocity.z = zTerm + (-yawSinCos * rand);
    BgHeavyBlock_SetPieceRandRot(self, scale);
    Actor_SetScale(&self->dyna.actor, Rand_CenteredFloat(0.2f) + 1.0f);
}

// 3DS Land: outside-castle landing snaps the block to a fixed spot (SpawnDust args are evaluated right-to-left).
void BgHeavyBlock_Land(BgHeavyBlock* self, PlayState* play) {
    if (Math_SmoothStepToS(&self->dyna.actor.shape.rot.x, (s16)0x8AD0, 6, 2000, 100) != 0) {
        Math_StepToF(&self->dyna.actor.speedXZ, 0.0f, 20.0f);
        Math_StepToF(&self->dyna.actor.velocity.y, 0.0f, 3.0f);
        self->dyna.actor.gravity = 0.0f;
        self->dyna.actor.world.pos = self->dyna.actor.home.pos;
        Actor_MoveForward(&self->dyna.actor);
        self->dyna.actor.home.pos = self->dyna.actor.world.pos;
        switch (PARAMS_GET_U(self->dyna.actor.params, 0, 8)) {
            case HEAVYBLOCK_UNBREAKABLE_OUTSIDE_CASTLE:
                FUN_0036c684(play, Rand_CenteredFloat(30.0f) + 1678.0f, Rand_ZeroFloat(100.0f) + 1286.0f,
                             Rand_CenteredFloat(30.0f) + 552.0f, 0.0f, 0.0f, 0.0f, 0);
                FUN_0036c684(play, Rand_CenteredFloat(30.0f) + 1729.0f, Rand_ZeroFloat(80.0f) + 1269.0f,
                             Rand_CenteredFloat(30.0f) + 600.0f, 0.0f, 0.0f, 0.0f, 0);
                break;
            case HEAVYBLOCK_UNBREAKABLE:
                FUN_0036c684(play, Rand_CenteredFloat(100.0f) + -735.0f, 29.0f, Rand_CenteredFloat(100.0f) + -3418.0f,
                             0.0f, 0.0f, 0.0f, 3);
                break;
        }
    } else {
        if ((PARAMS_GET_U(self->dyna.actor.params, 0, 8)) == HEAVYBLOCK_UNBREAKABLE_OUTSIDE_CASTLE) {
            self->dyna.actor.shape.rot.x = self->dyna.actor.world.rot.x = (s16)0x8AD0;
            self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y = (s16)0xC000;
            self->dyna.actor.shape.rot.z = self->dyna.actor.world.rot.z = 0;
            self->dyna.actor.world.pos.x = 1704.0f;
            self->dyna.actor.world.pos.y = 1504.0f;
            self->dyna.actor.world.pos.z = 516.0f;
        }
        if ((gSaveContext.masterQuestFlag == 0) && (*(u16*)((u8*)play + 0x104) == 0xD) &&
            (self->dyna.actor.room == 0xE)) {
            self->dyna.actor.world.pos.y = 130.0f;
        }
        self->dyna.actor.flags &= ~0x30;
        self->actionFunc = BgHeavyBlock_DoNothing;
    }
}

// Draft: 3DS version builds z first (rand + cos * -70 + pos.z) then x.
#if 0
void BgHeavyBlock_LiftedUp(BgHeavyBlock* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((u16)self->timer == 17) {
        FUN_0036f59c(player, 0x0100006D);
    }

    if (self->timer < 60) {
        f32 xOffset;
        f32 xPos;
        f32 yPos;
        f32 zPos;

        zPos = (Rand_CenteredFloat(110.0f) + Math_CosS(self->dyna.actor.shape.rot.y) * -70.0f) +
               self->dyna.actor.world.pos.z;
        xOffset = Rand_CenteredFloat(110.0f) + Math_SinS(self->dyna.actor.shape.rot.y) * -70.0f;
        xPos = xOffset + self->dyna.actor.world.pos.x;
        yPos = self->dyna.actor.world.pos.y + 10.0f;
        FUN_0036c684(play, xPos, yPos, zPos, 0.0f, -1.0f, 0.0f, 0xC);
    }

    self->timer++;

    FUN_0036e980(play, &player->actor, 8);

    if (Actor_HasNoParent(&self->dyna.actor, play)) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x01000239);
        self->actionFunc = BgHeavyBlock_Fly;
    }
}
#endif
GLOBAL_ASM("asm/BgHeavyBlock_LiftedUp.s")

void BgHeavyBlock_MovePiece(BgHeavyBlock* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;

    thisx->velocity.y += thisx->gravity;

    if (!(thisx->velocity.y >= thisx->minVelocityY)) {
        thisx->velocity.y = thisx->minVelocityY;
    }

    thisx->velocity.x *= 0.98f;
    thisx->velocity.z *= 0.98f;
    FUN_0036b96c(thisx);
    thisx->shape.rot.x += thisx->world.rot.x;
    thisx->shape.rot.y += thisx->world.rot.y;
    thisx->shape.rot.z += thisx->world.rot.z;

    if (!(self->pieceFlags & PIECE_FLAG_HIT_FLOOR)) {
        thisx->world.pos.y += self->unk_1BC.y;
        thisx->prevPos.y += self->unk_1BC.y;
        Actor_UpdateBgCheckInfo(play, thisx, 50.0f, 50.0f, 0.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2);
        thisx->world.pos.y -= self->unk_1BC.y;
        thisx->prevPos.y -= self->unk_1BC.y;
        if (thisx->bgCheckFlags & BGCHECKFLAG_GROUND) {
            self->pieceFlags |= PIECE_FLAG_HIT_FLOOR;
            thisx->velocity.y = Rand_ZeroFloat(4.0f) + 2.0f;
            thisx->velocity.x = Rand_CenteredFloat(8.0f);
            thisx->velocity.z = Rand_CenteredFloat(8.0f);
            BgHeavyBlock_SetPieceRandRot(self, 1.0f);
            Audio_PlayActorSound2(thisx, 0x010001A9);
        }
    }

    if (self->timer > 0) {
        self->timer--;
    } else {
        Actor_Kill(thisx);
    }
}

void OnePointCutscene_Init(PlayState* play, s16 csId, s16 arg, Actor* actor, s32 camId);
void Quake_SetSpeed(s32 idx, s16 speed);

void BgHeavyBlock_Wait(BgHeavyBlock* self, PlayState* play) {
    s32 quakeIndex;

    if (Actor_HasParent(&self->dyna.actor, play)) {
        self->timer = 0;

        switch (PARAMS_GET_U(self->dyna.actor.params, 0, 8)) {
            case HEAVYBLOCK_BREAKABLE:
                OnePointCutscene_Init(play, 4020, 270, &self->dyna.actor, 0);
                break;
            case HEAVYBLOCK_UNBREAKABLE:
                OnePointCutscene_Init(play, 4021, 220, &self->dyna.actor, 0);
                break;
            case HEAVYBLOCK_UNBREAKABLE_OUTSIDE_CASTLE:
                OnePointCutscene_Init(play, 4022, 210, &self->dyna.actor, 0);
                break;
        }

        quakeIndex = Quake_Add(((Camera**)((u8*)play + 0xA54))[*(s16*)((u8*)play + 0xA64)], 3);
        Quake_SetSpeed(quakeIndex, 25000);
        Quake_SetQuakeValues(quakeIndex, 1, 1, 5, 0);
        Quake_SetCountdown(quakeIndex, 10);
        self->actionFunc = BgHeavyBlock_LiftedUp;
    }
}

// Draft: N64 BgHeavyBlock_SpawnDust (3DS camera at +0x80, eye +0x8C).
#if 0
void FUN_0034035c(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env,
                  s16 scale, s16 scaleStep, s32 life);

static inline s16 BgHeavyBlock_Vec3f_Pitch(Vec3f* a, Vec3f* b) {
    return Math_Atan2S(Math_Vec3f_DistXZ(a, b), a->y - b->y);
}

void FUN_0036c684(PlayState* play, f32 posX, f32 posY, f32 posZ, f32 velX, f32 velY, f32 velZ, u8 dustParams) {
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;
    Vec3f eye;
    Vec3f at;
    s16 sp6E;
    s16 sp6C;
    Vec3f accel;
    Vec3f velocity;
    Vec3f pos;
    f32 sp44;
    s16 scaleStep;
    s16 scale;

    pos.x = posX;
    pos.y = posY;
    pos.z = posZ;

    if (dustParams & 1) {
        primColor.r = 150;
        primColor.g = primColor.b = envColor.g = envColor.b = 0;
        envColor.r = 80;
        primColor.a = envColor.a = 0;
    } else {
        primColor.r = 170;
        primColor.g = 130;
        primColor.b = 90;
        primColor.a = 255;
        envColor.r = 100;
        envColor.g = 60;
        envColor.b = 20;
        envColor.a = 255;
    }

    accel.z = 0.0f;
    accel.x = 0.0f;
    accel.y = (dustParams & 8) ? 0.0f : 0.5f;

    eye = *(Vec3f*)((u8*)play->cameraPtrs[play->activeCamera] + 0x8C);
    at = *(Vec3f*)((u8*)play->cameraPtrs[play->activeCamera] + 0x80);

    scale = 1000;
    scaleStep = 160;

    switch (dustParams & 6) {
        case 4:
        case 6:
            velocity.x = velX;
            velocity.y = velY;
            velocity.z = velZ;
            scale = 300;
            scaleStep = 50;
            break;
        case 2:
            sp44 = Rand_ZeroFloat(5.0f) + 5.0f;
            sp6E = Rand_CenteredFloat(65280.0f);

            velocity.x = (Math_SinS(sp6E) * sp44) + velX;
            velocity.y = velY;
            velocity.z = (Math_CosS(sp6E) * sp44) + velZ;
            break;
        case 0:
            sp6E = Math_Vec3f_Yaw(&eye, &at);
            sp6C = -BgHeavyBlock_Vec3f_Pitch(&eye, &at);

            velocity.x = ((5.0f * Math_SinS(sp6E)) * Math_CosS(sp6C)) + velX;
            velocity.y = (Math_SinS(sp6C) * 5.0f) + velY;
            velocity.z = ((5.0f * Math_CosS(sp6E)) * Math_CosS(sp6C)) + velZ;

            pos.x -= (velocity.x * 20.0f);
            pos.y -= (velocity.y * 20.0f);
            pos.z -= (velocity.z * 20.0f);
            break;
    }

    FUN_0034035c(play, &pos, &velocity, &accel, &primColor, &envColor, scale, scaleStep,
                 (s32)Rand_ZeroFloat(10.0f) + 20);
}
#endif
GLOBAL_ASM("asm/FUN_0036c684.s")

}
