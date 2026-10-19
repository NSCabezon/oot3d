extern "C" {
#include "z_bg_mizu_water.hpp"

#define FLAGS 0x80400030

// roomCtx.unk_7C0 is an s16 array on 3DS (N64 roomCtx.drawParams)
#define DRAW_PARAMS(play) ((s16*)&(play)->roomCtx.unk_7C0)

// 3DS water box entries are 0x10 bytes (repo WaterBox in z3Dbgcheck.hpp is only 4 bytes)
typedef struct MizuWaterBox {
    /* 0x00 */ char unk_00[0x2];
    /* 0x02 */ s16 ySurface;
    /* 0x04 */ char unk_04[0xC];
} MizuWaterBox;

typedef struct WaterLevel {
    s32 switchFlag;
    s32 yDiff;
} WaterLevel;

void BgMizuWater_Init(Actor* thisx, PlayState* play);
void BgMizuWater_Destroy(Actor* thisx, PlayState* play);
void BgMizuWater_Update(Actor* thisx, PlayState* play);
void BgMizuWater_Draw(Actor* thisx, PlayState* play);

void FUN_003e78ac(Actor* thisx, PlayState* play);
void FUN_003dfe64(Actor* thisx, PlayState* play);

void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* pos);
void FUN_003725e0(PlayState* play); // Message_CloseTextbox
void OnePointCutscene_Init(PlayState* play, s32 csId, s32 arg2, Actor* actor, s32 camId);

ActorInit Bg_Mizu_Water_InitVars = {
    ACTOR_BG_MIZU_WATER,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuWater),
    (ActorFunc)BgMizuWater_Init,
    (ActorFunc)BgMizuWater_Destroy,
    (ActorFunc)BgMizuWater_Update,
    (ActorFunc)BgMizuWater_Draw,
};

GLOBAL_ASM("data/z_Bg_Mizu_Water.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Mizu_Water/data_syms.s")

// sInitChain (0x00513160), sWaterLevels (0x00513164) and sWaterBoxIndices (0x00513184) live in the data dump above.
// They have no symbols in the scatter file, so they are absolute symbols (EQU) in data_syms.s.
extern InitChainEntry BgMizuWater_sInitChain[];
extern WaterLevel BgMizuWater_sWaterLevels[];
extern u32 BgMizuWater_sWaterBoxIndices[];

#define sInitChain BgMizuWater_sInitChain
#define sWaterLevels BgMizuWater_sWaterLevels
#define sWaterBoxIndices BgMizuWater_sWaterBoxIndices

static u32 BgMizuWater_GetWaterLevelActionIndex(s16 switchFlag, PlayState* play) {
    u32 ret;

    if (Flags_GetSwitch(play, WATER_TEMPLE_WATER_F1_FLAG) && (switchFlag != WATER_TEMPLE_WATER_F1_FLAG)) {
        ret = 3;
    } else if (Flags_GetSwitch(play, WATER_TEMPLE_WATER_F2_FLAG) && (switchFlag != WATER_TEMPLE_WATER_F2_FLAG)) {
        ret = 2;
    } else if (Flags_GetSwitch(play, WATER_TEMPLE_WATER_F3_FLAG) && (switchFlag != WATER_TEMPLE_WATER_F3_FLAG)) {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

static void BgMizuWater_SetWaterBoxesHeight(MizuWaterBox* waterBoxes, s16 height) {
    s32 i;

    for (i = 0; i < 8; i++) {
        waterBoxes[sWaterBoxIndices[i]].ySurface = height;
    }
}

void BgMizuWater_Init(Actor* thisx, PlayState* play) {
    BgMizuWater* self = (BgMizuWater*)thisx;
    MizuWaterBox* waterBoxes;
    s32 waterLevelActionIndex;

    waterBoxes = (MizuWaterBox*)play->colCtx.stat.colHeader->waterBoxes;
    self->type = thisx->params & 0xFF;
    self->switchFlag = (thisx->params >> 8) & 0xFF;
    Actor_ProcessInitChain(thisx, sInitChain);

    {
        ZARInfo* zarInfo = FUN_00372f38(thisx, play, &self->skelAnimModel, 0xB, 0);
        void* cmab = zarInfo->GetCMABByIndex(1);
        self->skelAnimModel->unk_0C->FUN_00372d94(cmab);
        self->skelAnimModel->unk_0C->SetAnimType(1);
    }

    self->baseY = thisx->world.pos.y;
    self->targetY = thisx->world.pos.y;

    switch (self->type) {
        case 0:
            waterLevelActionIndex = BgMizuWater_GetWaterLevelActionIndex(-1, play);
            thisx->world.pos.y = sWaterLevels[waterLevelActionIndex].yDiff + self->baseY;
            BgMizuWater_SetWaterBoxesHeight(waterBoxes, thisx->world.pos.y);
            thisx->params = sWaterLevels[waterLevelActionIndex].switchFlag;
            Flags_UnsetSwitch(play, WATER_TEMPLE_WATER_F1_FLAG);
            Flags_UnsetSwitch(play, WATER_TEMPLE_WATER_F2_FLAG);
            Flags_UnsetSwitch(play, WATER_TEMPLE_WATER_F3_FLAG);

            switch (thisx->params) {
                case 0x1E:
                    Flags_SetSwitch(play, WATER_TEMPLE_WATER_F3_FLAG);
                    break;
                case 0x1D:
                    Flags_SetSwitch(play, WATER_TEMPLE_WATER_F2_FLAG);
                    break;
                case 0x1C:
                default:
                    Flags_SetSwitch(play, WATER_TEMPLE_WATER_F1_FLAG);
                    break;
            }
            self->targetY = thisx->world.pos.y;
            break;
        case 1:
            break;
        case 2:
            if (Flags_GetSwitch(play, self->switchFlag)) {
                thisx->world.pos.y = self->baseY + 85.0f;
            }
            waterBoxes[6].ySurface = thisx->world.pos.y;
            break;
        case 3:
            if (Flags_GetSwitch(play, self->switchFlag)) {
                thisx->world.pos.y = self->baseY + 110.0f;
                self->targetY = thisx->world.pos.y;
            }
            waterBoxes[8].ySurface = thisx->world.pos.y;
            break;
        case 4:
            if (Flags_GetSwitch(play, self->switchFlag)) {
                thisx->world.pos.y = self->baseY + 160.0f;
                self->targetY = thisx->world.pos.y;
            }
            waterBoxes[16].ySurface = thisx->world.pos.y;
            break;
    }

    self->actionFunc = (BgMizuWaterActionFunc)FUN_003e78ac;
}

void BgMizuWater_Destroy(Actor* thisx, PlayState* play) {
    BgMizuWater* self = (BgMizuWater*)thisx;

    FUN_00350f34(thisx, &self->skelAnimModel, 0);
}

void FUN_003e78ac(Actor* thisx, PlayState* play) {
    BgMizuWater* self = (BgMizuWater*)thisx;
    s32 waterLevelActionIndex;
    s16 prevSwitchFlag = thisx->params;

    switch (self->type) {
        case 0:
            waterLevelActionIndex = BgMizuWater_GetWaterLevelActionIndex(thisx->params, play);
            if (waterLevelActionIndex != 0) {
                if (prevSwitchFlag != sWaterLevels[waterLevelActionIndex].switchFlag) {
                    OnePointCutscene_Init(play, 3120, (s16)-(waterLevelActionIndex + 100), NULL, 0);
                    thisx->params = sWaterLevels[waterLevelActionIndex].switchFlag;
                    self->targetY = sWaterLevels[waterLevelActionIndex].yDiff + self->baseY;
                }
            }
            if ((prevSwitchFlag != thisx->params) && (prevSwitchFlag != 0)) {
                Flags_UnsetSwitch(play, prevSwitchFlag);
            }
            break;
        case 1:
            break;
        case 2:
            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->targetY = self->baseY + 85.0f;
            } else {
                self->targetY = self->baseY;
            }
            break;
        case 3:
            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->targetY = self->baseY + 110.0f;
            } else {
                self->targetY = self->baseY;
            }
            break;
        case 4:
            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->targetY = self->baseY + 160.0f;
            } else {
                self->targetY = self->baseY;
            }
    }

    if (self->targetY != thisx->world.pos.y) {
        self->actionFunc = (BgMizuWaterActionFunc)FUN_003dfe64;
    }
}

void FUN_003dfe64(Actor* thisx, PlayState* play) {
    BgMizuWater* self = (BgMizuWater*)thisx;
    s16 prevSwitchFlag;
    s32 waterLevelActionIndex;
    MizuWaterBox* waterBoxes;

    waterBoxes = (MizuWaterBox*)play->colCtx.stat.colHeader->waterBoxes;
    switch (self->type) {
        case 0:
            prevSwitchFlag = thisx->params;
            waterLevelActionIndex = BgMizuWater_GetWaterLevelActionIndex(thisx->params, play);
            if (waterLevelActionIndex != 0) {
                if (prevSwitchFlag != sWaterLevels[waterLevelActionIndex].switchFlag) {
                    thisx->params = sWaterLevels[waterLevelActionIndex].switchFlag;
                    self->targetY = sWaterLevels[waterLevelActionIndex].yDiff + self->baseY;
                }
            }

            if ((prevSwitchFlag != thisx->params) && (prevSwitchFlag != 0)) {
                Flags_UnsetSwitch(play, prevSwitchFlag);
            }

            if (Math_StepToF(&thisx->world.pos.y, self->targetY, 5.0f)) {
                DRAW_PARAMS(play)[0] = 0;
                self->actionFunc = (BgMizuWaterActionFunc)FUN_003e78ac;
                FUN_003725e0(play);
            }
            BgMizuWater_SetWaterBoxesHeight((MizuWaterBox*)play->colCtx.stat.colHeader->waterBoxes, thisx->world.pos.y);
            break;
        case 1:
            break;
        case 2:
            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->targetY = self->baseY + 85.0f;
            } else {
                self->targetY = self->baseY;
            }
            if (Math_StepToF(&thisx->world.pos.y, self->targetY, 1.0f)) {
                DRAW_PARAMS(play)[0] = 0;
                self->actionFunc = (BgMizuWaterActionFunc)FUN_003e78ac;
            }
            waterBoxes[6].ySurface = thisx->world.pos.y;
            break;
        case 3:
            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->targetY = self->baseY + 110.0f;
            } else {
                self->targetY = self->baseY;
            }
            if (Math_StepToF(&thisx->world.pos.y, self->targetY, 1.0f)) {
                DRAW_PARAMS(play)[0] = 0;
                self->actionFunc = (BgMizuWaterActionFunc)FUN_003e78ac;
            }
            waterBoxes[8].ySurface = thisx->world.pos.y;
            break;
        case 4:
            if (Flags_GetSwitch(play, self->switchFlag)) {
                self->targetY = self->baseY + 160.0f;
            } else {
                self->targetY = self->baseY;
            }
            if (Math_StepToF(&thisx->world.pos.y, self->targetY, 1.0f)) {
                DRAW_PARAMS(play)[0] = 0;
                self->actionFunc = (BgMizuWaterActionFunc)FUN_003e78ac;
            }
            waterBoxes[16].ySurface = thisx->world.pos.y;
            break;
    }

    if (self->targetY < thisx->world.pos.y) {
        FUN_0035ae08(thisx, 0x010001B5); // NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG
    } else if (self->targetY > self->actor.world.pos.y) {
        FUN_0035ae08(thisx, 0x010001B5); // NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG
    }
}

void BgMizuWater_Update(Actor* thisx, PlayState* play) {
    BgMizuWater* self = (BgMizuWater*)thisx;
    s32 posY;
    s32 unk0;
    s32 unk1;

    self->drawY = 0.0f;
    if (self->type == 3) {
        self->drawY = -500.0f;
    }
    if (self->type == 0) {
        posY = self->actor.world.pos.y;
        unk0 = 0;
        unk1 = 0;
        if (posY < WATER_TEMPLE_WATER_F1_Y) {
            unk0 = 0;
            {
                f32 t = (posY - WATER_TEMPLE_WATER_B1_Y) / (WATER_TEMPLE_WATER_F1_Y - WATER_TEMPLE_WATER_B1_Y);
                unk1 = t * 200;
            }
        } else if (posY < WATER_TEMPLE_WATER_F2_Y) {
            unk0 = 1;
            unk1 = 255 - (s32)((posY - WATER_TEMPLE_WATER_F1_Y) / (WATER_TEMPLE_WATER_F2_Y - WATER_TEMPLE_WATER_F1_Y) *
                               (255 - 160));
        } else if (posY <= WATER_TEMPLE_WATER_F3_Y) {
            unk0 = 2;
            unk1 = 255 - (s32)((posY - WATER_TEMPLE_WATER_F2_Y) / (WATER_TEMPLE_WATER_F3_Y - WATER_TEMPLE_WATER_F2_Y) *
                               (255 - 160));
        }
        DRAW_PARAMS(play)[1] = ((u8)unk0 << 8) | (unk1 & 0xFF);
    }

    self->actionFunc(self, play);
}

GLOBAL_ASM("asm/BgMizuWater_Draw.s")

/*
 * BgMizuWater_Draw is still GLOBAL_ASM.
 * Root cause of the r5 constant (stage 3, Opus): the ROM's `mov r5,#1` is the integer 1 CSE'd with ANOTHER use of 1
 * that later folds into an addressing mode -- an inline-function index parameter, exactly like
 * BgJyaKanaami_ScaleCol(&mtx, 1, ...) (matched, r6 = 1). Any `v[i]` store through an inline helper/operator[] with
 * i = 0/1/2 gives push {r4,r5,lr}, frame 0x3c, pos at sp+0. Plain constants, dead duplicate stores, `== 1` mode
 * flags, inline params used as values, loops and local flags all fold too early (r1).
 * Same idiom is the likely blocker in ~100 still-asm Draw functions (ArrowFire_Draw, DoorAna_Draw, EnBomChu_Draw...).
 * Draft below = 2 words differ (only the order of the y/z stores vs `mov r2,sp`), but it needs a reversed
 * (z, y, x) parameter order and out-of-order stores -> not plausible source, so NOT used. Remaining: find the natural
 * helper that gives x->s2, y->s0 (the shared 0.0f), z->s1 (vmov copy) with stores in x,y,z order.
 * 2026-10-14 near-miss sweep: still 2 words. Tried (generated, ~1000 variants): the En_Siofuki fix (helper with
 * fewer params + 0.0f literal inside), every param subset x order x store order, member/index mixes, chained zero
 * stores, f32 block locals per argument, zero local shared with scale, scale ternary/if-else, early return,
 * dead self alias / play2 / statement-swap single-site mutations. Natural xyz/xz helpers get the right store order
 * but swap s0/s2 (6 words).
 * recipe_near4 2026-10-15: still 2. Helper param order y,x,z (all 36 param x store orders, idx/member) no; the En_Fhg_Fire fix
 * (2-param setter `v->x=x; v->y=y; v->z=0.0f`, or by-value Vector3f) loses the r5 index-1 shape here (20-24 words).
 */
#if 0
struct BgMizuWaterVec {
    f32& operator[](s32 i) {
        return (&x)[i];
    }
    f32 x, y, z;
};
static inline void BgMizuWater_Translate(MTX34* mtx, f32 z, f32 y, f32 x) {
    BgMizuWaterVec pos;

    pos[0] = x;
    pos[2] = z;
    pos[1] = y;
    Matrix_UpdatePosition(mtx, mtx, (Vec3f*)&pos);
}
void BgMizuWater_Draw(Actor* thisx, PlayState* play) {
    BgMizuWater* self = (BgMizuWater*)thisx;
    MTX34 mtx;
    f32 scale;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    scale = 1.0f;
    if (PauseContext_GetState()) {
        scale = 0.0f;
    }
    if (self->skelAnimModel != NULL) {
        BgMizuWater_Translate(&mtx, 0.0f, 0.0f, self->drawY);
        self->skelAnimModel->unk_0C->SetAnimSpeed(scale);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
#endif

}
