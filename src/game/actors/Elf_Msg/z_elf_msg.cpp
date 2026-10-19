extern "C" {
#include "z_elf_msg.hpp"
#include "model_manager.hpp"

#define FLAGS 0x00000010

extern __pure float _fabsf(float);

// Local view of the Player fields used by the Navi call-spot functions (Player struct not defined in repo)
typedef struct ElfMsgNavi {
    /* 0x0000 */ char unk_000[0x98C];
    /* 0x098C */ ElfMsg* elfMsg;
} ElfMsgNavi;

typedef struct ElfMsgPlayer {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x1724 - 0x1A4];
    /* 0x1724 */ ElfMsgNavi* naviActor;
    /* 0x1728 */ s16 naviTextId;
} ElfMsgPlayer;

static __inline __pure float ElfMsg_Fabsf(float f) {
    return _fabsf(f);
}

void ElfMsg_Init(Actor* thisx, PlayState* play);
void ElfMsg_Destroy(Actor* thisx, PlayState* play);
void ElfMsg_Update(Actor* thisx, PlayState* play);
void ElfMsg_Draw(Actor* thisx, PlayState* play);
void ElfMsg_CallNaviCuboid(ElfMsg* self, PlayState* play);
void ElfMsg_CallNaviCylinder(ElfMsg* self, PlayState* play);

GLOBAL_ASM("data/z_Elf_Msg.data.s")
GLOBAL_ASM("../src/game/actors/Elf_Msg/syms.s")

ActorInit Elf_Msg_InitVars = {
    ACTOR_ELF_MSG,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ElfMsg),
    (ActorFunc)ElfMsg_Init,
    (ActorFunc)ElfMsg_Destroy,
    (ActorFunc)ElfMsg_Update,
    (ActorFunc)ElfMsg_Draw,
};

static s32 ElfMsg_GetMessageId(ElfMsg* self) {
    if (self->actor.params & 0x8000) {
        return (self->actor.params & 0xFF) + 0x100;
    } else {
        return -((self->actor.params & 0xFF) + 0x100);
    }
}

static s32 ElfMsg_KillCheck(ElfMsg* self, PlayState* play) {
    s32 unused = 0; // not in source: needed to reproduce the original register allocation (play in r6)
    if ((self->actor.world.rot.y > 0) && (self->actor.world.rot.y <= 0x40) &&
        Flags_GetSwitch(play, self->actor.world.rot.y - 1)) {
        if (((self->actor.params >> 8) & 0x3F) != 0x3F) {
            Flags_SetSwitch(play, (self->actor.params >> 8) & 0x3F);
        }
        Actor_Kill(&self->actor);
        return 1;
    } else if ((self->actor.world.rot.y == -1) && Flags_GetClear(play, self->actor.room)) {
        if (((self->actor.params >> 8) & 0x3F) != 0x3F) {
            Flags_SetSwitch(play, (self->actor.params >> 8) & 0x3F);
        }
        Actor_Kill(&self->actor);
        return 1;
    } else if (((self->actor.params >> 8) & 0x3F) == 0x3F) {
        return 0;
    } else if (Flags_GetSwitch(play, (self->actor.params >> 8) & 0x3F)) {
        Actor_Kill(&self->actor);
        return 1;
    }
    return 0;
}

static __inline void ElfMsg_SetupAction(ElfMsg* self, ElfMsgActionFunc f) {
    self->actionFunc = f;
}

void ElfMsg_Init(Actor* thisx, PlayState* play) {
    ElfMsg* self = (ElfMsg*)thisx;

    if (!ElfMsg_KillCheck(self, play)) {
        Actor_ProcessInitChain(thisx, (InitChainEntry*)0x0052033c);
        {
            // one f32 temp assigned AND stored in each arm (ROM keeps per-arm stores, shared s0, no if-conversion)
            f32 scale;
            if (thisx->world.rot.x == 0) {
                scale = 0.4f;
                thisx->scale.z = scale;
                thisx->scale.x = scale;
            } else {
                scale = thisx->world.rot.x * 0.04f;
                thisx->scale.z = scale;
                thisx->scale.x = scale;
            }
        }

        if (thisx->world.rot.z == 0) {
            thisx->scale.y = 0.4f;
        } else {
            thisx->scale.y = thisx->world.rot.z * 0.04f;
        }

        // 3DS swaps the N64 branch order: bit 0x4000 set -> Cylinder
        if (thisx->params & 0x4000) {
            ElfMsg_SetupAction(self, ElfMsg_CallNaviCylinder);
        } else {
            ElfMsg_SetupAction(self, ElfMsg_CallNaviCuboid);
        }

        thisx->shape.rot.x = thisx->shape.rot.y = thisx->shape.rot.z = 0;
    }
}

void ElfMsg_Destroy(Actor* thisx, PlayState* play) {
}

// Draft, NOT matching as C: the C body is 296 B but functions.tsv gives 308 B (12 trailing bytes at 0x001F2E74,
// unreferenced `ldr r0,[r0,#0x50c]; mov r1,#0; b FUN_00372170`). Emitting C shifts every later function, so this
// stays asm until the function is split with cfg_split.py. Body matches byte-for-byte under fastchk (0/77 words).
GLOBAL_ASM("../split/asm/FUN_001f2e74.s")

void ElfMsg_Update(Actor* thisx, PlayState* play) {
    ElfMsg* self = (ElfMsg*)thisx;

    if (!ElfMsg_KillCheck(self, play)) {
        Actor* actor = &self->actor;
        if (Actor_ProcessTalkRequest(actor, play)) {
            if (((self->actor.params >> 8) & 0x3F) != 0x3F) {
                Flags_SetSwitch(play, (self->actor.params >> 8) & 0x3F);
            }
            Actor_Kill(actor);
            return;
        }
        if (((u32)(actor->world.rot.y - 0x42) > 0x3E) || Flags_GetSwitch(play, actor->world.rot.y - 0x41)) {
            self->actionFunc(self, play);
        }
    }
}

// Inlines the shared GetModelManager() static (guard 0x0055A21C, include/model_manager.hpp); its 2D drawer sub-object
// lives at +0x32A0.
void FUN_0033a754(void* out, s32 a, s32 b, s32 c, f32 sx, f32 sy, f32 sz, f32 px, f32 py, f32 pz);
void FUN_0033a638(void* gfx, s32 a, void* mtx, void* col);

typedef struct {
    u32 w[4];
} ElfMsgColor;
extern ElfMsgColor ElfMsg_sDrawColor; // 0x004D91FC (syms.s)

void ElfMsg_Draw(Actor* thisx, PlayState* play) {
    u8 mtx[0x30];
    ElfMsgColor col;

    if (*(u16*)((u8*)gGameInfo + 0xE72) == 0) {
        return;
    }
    col = ElfMsg_sDrawColor;
    FUN_0033a754(mtx, 0, 0, 0, thisx->scale.x, thisx->scale.y, thisx->scale.x, thisx->world.pos.x, thisx->world.pos.y,
                 thisx->world.pos.z);
    FUN_0033a638((char*)&GetModelManager() + 0x32A0, 0, mtx, &col);
}

void ElfMsg_CallNaviCuboid(ElfMsg* self, PlayState* play) {
    ElfMsgPlayer* player = (ElfMsgPlayer*)GET_PLAYER(play);
    ElfMsgNavi* navi = player->naviActor;
    f32 range = self->actor.scale.x * 100.0f;
    f32 dx = self->actor.world.pos.x - player->actor.world.pos.x;
    f32 dz = self->actor.world.pos.z - player->actor.world.pos.z;

    if (((dx * dx) + (dz * dz) < (range * range)) &&
        (player->actor.world.pos.y >= self->actor.world.pos.y) &&
        ((player->actor.world.pos.y - self->actor.world.pos.y) < (100.0f * self->actor.scale.y))) {
        player->naviTextId = ElfMsg_GetMessageId(self);
        navi->elfMsg = self;
    }
}

void ElfMsg_CallNaviCylinder(ElfMsg* self, PlayState* play) {
    ElfMsgPlayer* player = (ElfMsgPlayer*)GET_PLAYER(play);
    ElfMsgNavi* navi = player->naviActor;

    if ((ElfMsg_Fabsf(player->actor.world.pos.x - self->actor.world.pos.x) < (self->actor.scale.x * 100.0f)) &&
        (player->actor.world.pos.y >= self->actor.world.pos.y) &&
        ((player->actor.world.pos.y - self->actor.world.pos.y) < (100.0f * self->actor.scale.y)) &&
        (ElfMsg_Fabsf(player->actor.world.pos.z - self->actor.world.pos.z) < (self->actor.scale.z * 100.0f))) {
        player->naviTextId = ElfMsg_GetMessageId(self);
        navi->elfMsg = self;
    }
}

}
