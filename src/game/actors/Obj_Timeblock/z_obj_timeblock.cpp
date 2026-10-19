extern "C" {
#include "z_obj_timeblock.hpp"

#define FLAGS 0x0A000011

void ObjTimeblock_Init(Actor* actor, GameState* state);
void ObjTimeblock_Destroy(Actor* actor, GameState* state);
void ObjTimeblock_Update(Actor* actor, GameState* state);
void ObjTimeblock_Draw(Actor* actor, GameState* state);

void ObjTimeblock_AltBehaviourNotVisible(ObjTimeblock* self, PlayState* play);
void ObjTimeblock_Normal(ObjTimeblock* self, PlayState* play);
void ObjTimeblock_AltBehaviorVisible(ObjTimeblock* self, PlayState* play);
s32 ObjTimeblock_WaitForOcarina(ObjTimeblock* self, PlayState* play);
s32 ObjTimeblock_WaitForSong(ObjTimeblock* self, PlayState* play);

extern f32 ObjTimeblock_sRanges[]; // data_syms.s (N64 sRanges)

// MessageContext ocarina fields are not in the repo struct yet (3DS offsets from PlayState).
#define TIMEBLOCK_OCARINA_MODE(play) (*(u16*)((u8*)(play) + 0x2B7E))
#define TIMEBLOCK_LAST_SONG(play) (*(u16*)((u8*)(play) + 0x2B7C))

static __inline __pure float ObjTimeblock_Fabsf(float x) {
    return __fabsf(x);
}
s32 FUN_0032d8d8(DynaPolyActor* dyna);     // DynaPolyActor_IsPlayerAbove
void FUN_0037073c(PlayState* play, s32 action); // Message_StartOcarina

GLOBAL_ASM("../src/game/actors/Obj_Timeblock/data_syms.s")

GLOBAL_ASM("data/z_Obj_Timeblock.data.s")

ActorInit Obj_Timeblock_InitVars = {
    ACTOR_OBJ_TIMEBLOCK,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_TIMEBLOCK,
    sizeof(ObjTimeblock),
    (ActorFunc)ObjTimeblock_Init,
    (ActorFunc)ObjTimeblock_Destroy,
    (ActorFunc)ObjTimeblock_Update,
    (ActorFunc)ObjTimeblock_Draw,
};

typedef struct ObjTimeblockSizeOptions {
    /* 0x00 */ f32 scale;
    /* 0x04 */ f32 height;
    /* 0x08 */ s16 demoEffectParams;
} ObjTimeblockSizeOptions; // size = 0x0C

extern ObjTimeblockSizeOptions ObjTimeblock_sSizeOptions[]; // data_syms.s
extern InitChainEntry ObjTimeblock_sInitChain[];            // data_syms.s

// N64 helper, inlined into ObjTimeblock_Init by armcc.
static inline u32 ObjTimeblock_CalculateIsVisible(ObjTimeblock* self) {
    if (!((self->dyna.actor.params >> 10) & 1)) {
        if (self->unk_177 == 0) {
            return self->unk_175;
        } else {
            u8 temp = ((self->dyna.actor.params >> 15) & 1) ? true : false;

            if (self->unk_177 == 1) {
                return self->unk_174 ^ temp;
            } else {
                u8 linkIsChild = (LINK_AGE_IN_YEARS == YEARS_CHILD) ? true : false;

                return self->unk_174 ^ temp ^ linkIsChild;
            }
        }
    } else {
        return ((self->dyna.actor.params >> 15) & 1 ? true : false) ^ self->unk_174;
    }
}

void ObjTimeblock_Init(Actor* thisx, GameState* state) {
    ObjTimeblock* self = (ObjTimeblock*)thisx;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, 0);
    thisx->world.rot.z = thisx->shape.rot.z = 0;

    ReadCollisionHeaderFromZsi(FUN_00372f38(thisx, play, &self->skelAnimModel, 0, 0)->FUN_003532c0(0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);

    Actor_ProcessInitChain(thisx, ObjTimeblock_sInitChain);
    Actor_SetScale(thisx, ObjTimeblock_sSizeOptions[(thisx->params >> 8) & 3].scale);

    if ((thisx->params >> 6) & 1) {
        self->unk_177 = 0;
    } else {
        if ((thisx->params & 0x3F) < 0x38) {
            self->unk_177 = 2;
        } else {
            self->unk_177 = 1;
        }
    }

    self->songObserverFunc = ObjTimeblock_WaitForOcarina;

    Actor_SetFocus(thisx, ObjTimeblock_sSizeOptions[(thisx->params >> 8) & 3].height);

    self->unk_174 = (Flags_GetSwitch(play, thisx->params & 0x3F)) ? true : false;
    self->unk_175 = (thisx->params >> 15) & 1 ? true : false;
    self->isVisible = ObjTimeblock_CalculateIsVisible(self);

    if (!((thisx->params >> 10) & 1)) {
        self->actionFunc = ObjTimeblock_Normal;
    } else if (self->isVisible) {
        self->actionFunc = ObjTimeblock_AltBehaviorVisible;
    } else {
        self->actionFunc = ObjTimeblock_AltBehaviourNotVisible;
    }
}

void ObjTimeblock_Destroy(Actor* thisx, GameState* state) {
    ObjTimeblock* self = (ObjTimeblock*)thisx;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    if (self->skelAnimModel != NULL) {
        FUN_003508b8(thisx, self->skelAnimModel, 0);
    }
}

void ObjTimeblock_Update(Actor* thisx, GameState* state) {
    ObjTimeblock* self = (ObjTimeblock*)thisx;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);

    if (self->demoEffectTimer > 0) {
        self->demoEffectTimer--;
    }

    if (self->isVisible) {
        FUN_0036d15c(play, &play->colCtx.dyna, self->dyna.bgId);
    } else {
        FUN_0036b940(play, &play->colCtx.dyna, self->dyna.bgId);
    }
}

void ObjTimeblock_Draw(Actor* thisx, GameState* state) {
    ObjTimeblock* self = (ObjTimeblock*)thisx;
    PlayState* play = (PlayState*)state;

    if (self->isVisible) {
        FUN_00357fd0(GET_PLAYER((PlayState*)state), thisx->unk_178, &thisx->world.pos);
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&thisx->modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}

#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

s32 OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 arg2, s32 arg3);
void FUN_00239fbc(ObjTimeblock* self);
void ObjTimeblock_DoNothing(ObjTimeblock* self, PlayState* play);

static inline void ObjTimeblock_SpawnDemoEffect(ObjTimeblock* self, PlayState* play) {
    Actor_Spawn(&play->actorCtx, play, 0x8B, self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y,
                self->dyna.actor.world.pos.z, 0, 0, 0,
                ObjTimeblock_sSizeOptions[(self->dyna.actor.params >> 8) & 3].demoEffectParams, 1);
}

static inline void ObjTimeblock_ToggleSwitchFlag(PlayState* play, s32 flag) {
    if (Flags_GetSwitch(play, flag)) {
        Flags_UnsetSwitch(play, flag);
    } else {
        Flags_SetSwitch(play, flag);
    }
}

static inline void ObjTimeblock_UpdateSwitchState(ObjTimeblock* self, PlayState* play) {
    s32 switchFlag = self->dyna.actor.params & 0x3F;

    self->unk_172 = TIMEBLOCK_LAST_SONG(play);

    if (self->demoEffectFirstPartTimer > 0 && --self->demoEffectFirstPartTimer == 0) {
        self->unk_174 = (Flags_GetSwitch(play, switchFlag)) ? true : false;
    }

    self->isVisible = ObjTimeblock_CalculateIsVisible(self);
    self->unk_176 = (Flags_GetSwitch(play, switchFlag)) ? true : false;
}

void ObjTimeblock_AltBehaviorVisible(ObjTimeblock* self, PlayState* play) {
    if (self->songObserverFunc(self, play) && self->demoEffectTimer <= 0) {
        self->demoEffectFirstPartTimer = FRAMES(12);
        ObjTimeblock_SpawnDemoEffect(self, play);
        self->demoEffectTimer = FRAMES(160);
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        ObjTimeblock_ToggleSwitchFlag(play, self->dyna.actor.params & 0x3F);
    }

    ObjTimeblock_UpdateSwitchState(self, play);

    if (self->demoEffectTimer == (s32)FRAMES(160) - 110) {
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
    }

    if (!self->isVisible && self->demoEffectTimer <= 0) {
        FUN_00239fbc(self);
    }
}

void ObjTimeblock_AltBehaviourNotVisible(ObjTimeblock* self, PlayState* play) {
    s32 switchFlag = self->dyna.actor.params & 0x3F;
    s8 switchFlagIsSet = (Flags_GetSwitch(play, switchFlag)) ? true : false;

    if (self->unk_176 ^ switchFlagIsSet && switchFlagIsSet ^ ((self->dyna.actor.params >> 15) & 1 ? true : false)) {
        if (self->demoEffectTimer <= 0) {
            ObjTimeblock_SpawnDemoEffect(self, play);
            self->demoEffectTimer = FRAMES(160);
        }
        self->demoEffectFirstPartTimer = FRAMES(12);
    }

    ObjTimeblock_UpdateSwitchState(self, play);

    if (self->isVisible && self->demoEffectTimer <= 0) {
        self->actionFunc = ObjTimeblock_AltBehaviorVisible;
    }
}

void ObjTimeblock_DoNothing(ObjTimeblock* self, PlayState* play) {
}

void ObjTimeblock_Normal(ObjTimeblock* self, PlayState* play) {
    u32 newIsVisible;

    if (self->songObserverFunc(self, play) && self->demoEffectTimer <= 0) {
        ObjTimeblock_SpawnDemoEffect(self, play);
        self->demoEffectTimer = FRAMES(160);
        OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        self->demoEffectFirstPartTimer = FRAMES(12);

        if (self->unk_177 == 0) {
            self->dyna.actor.params ^= 0x8000;
        } else {
            ObjTimeblock_ToggleSwitchFlag(play, self->dyna.actor.params & 0x3F);
        }
    }

    self->unk_172 = TIMEBLOCK_LAST_SONG(play);
    if (self->demoEffectFirstPartTimer > 0) {
        self->demoEffectFirstPartTimer--;
        if (self->demoEffectFirstPartTimer == 0) {
            if (self->unk_177 == 0) {
                self->unk_175 = (self->dyna.actor.params >> 15) & 1 ? true : false;
            } else {
                self->unk_174 = (Flags_GetSwitch(play, self->dyna.actor.params & 0x3F)) ? true : false;
            }
        }
    }

    newIsVisible = ObjTimeblock_CalculateIsVisible(self);
    if (self->unk_177 == 1 && newIsVisible != self->isVisible) {
        self->actionFunc = ObjTimeblock_DoNothing;
    }
    self->isVisible = newIsVisible;

    if (self->demoEffectTimer == (s32)FRAMES(160) - 110) {
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x0100048C);
    }
}

// WaitForOcarina draft (NOT matched: 5 words at 0xa4..0xb4, the else-branch flag store reg choice).
// Inlined into WaitForOcarina by armcc (N64 helper). Table: sRanges (data_syms.s).
static inline u8 ObjTimeblock_PlayerIsInRange(ObjTimeblock* self, PlayState* play) {
    if (self->isVisible && FUN_0032d8d8(&self->dyna)) {
        return false;
    }

    if (self->dyna.actor.xzDistToPlayer <= ObjTimeblock_sRanges[(self->dyna.actor.params >> 11) & 7]) {
        Vec3f playerRelativePos;
        f32 blockSize;

        FUN_0036c5d8(&self->dyna.actor, &playerRelativePos, &GET_PLAYER(play)->actor.world.pos);
        blockSize = self->dyna.actor.scale.x * 50.0f + 6.0f;
        // Return true if player's xz position is not inside the block
        if (blockSize < ObjTimeblock_Fabsf(playerRelativePos.x) || blockSize < ObjTimeblock_Fabsf(playerRelativePos.z)) {
            return true;
        }
    }

    return false;
}

s32 ObjTimeblock_WaitForOcarina(ObjTimeblock* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (ObjTimeblock_PlayerIsInRange(self, play)) {
        u32 f = player->stateFlags2;
        if (f & 0x1000000) {
            FUN_0037073c(play, 1);
            self->songObserverFunc = ObjTimeblock_WaitForSong;
        } else {
            player->stateFlags2 = f | 0x800000;
        }
    }
    return false;
}

s32 ObjTimeblock_WaitForSong(ObjTimeblock* self, PlayState* play) {
    if (TIMEBLOCK_OCARINA_MODE(play) == 4) {
        self->songObserverFunc = ObjTimeblock_WaitForOcarina;
    }
    if (TIMEBLOCK_LAST_SONG(play) == 0xA) {
        if (self->unk_172 == 254) {
            self->songEndTimer = 110;
        } else {
            self->songEndTimer--;
            if (self->songEndTimer == 0) {
                return true;
            }
        }
    }
    return false;
}

// Obj_Warp2block copy of WaitForSong (N64 func_80BA228C); same layout, observer func is FUN_0025ac68.
s32 FUN_0025ac68(ObjTimeblock* self, PlayState* play);

s32 FUN_0020803c(ObjTimeblock* self, PlayState* play) {
    if (TIMEBLOCK_OCARINA_MODE(play) == 4) {
        self->songObserverFunc = FUN_0025ac68;
    }
    if (TIMEBLOCK_LAST_SONG(play) == 0xA) {
        if (self->unk_172 == 254) {
            self->songEndTimer = 110;
        } else {
            self->songEndTimer--;
            if (self->songEndTimer == 0) {
                return true;
            }
        }
    }
    return false;
}

}
