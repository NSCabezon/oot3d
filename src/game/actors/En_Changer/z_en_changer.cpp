extern "C" {
#include "z_en_changer.hpp"

#define FLAGS 0x80000000

void EnChanger_Init(Actor* actor, GameState* state);
void EnChanger_Destroy(Actor* actor, GameState* state);
void EnChanger_Update(Actor* actor, GameState* state);

void FUN_0017963c(EnChanger* self, GameState* state);
void FUN_00157298(EnChanger* self, PlayState* play);
void FUN_00239264(EnChanger* self, PlayState* play);

GLOBAL_ASM("data/z_En_Changer.data.s")

ActorInit En_Changer_InitVars = {
    ACTOR_EN_CHANGER,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnChanger),
    (ActorFunc)EnChanger_Init,
    (ActorFunc)EnChanger_Destroy,
    (ActorFunc)EnChanger_Update,
    NULL,
};

GLOBAL_ASM("asm/EnChanger_Init.s")

// Chest "opened" flag lives at +0x38C of the EnBox (raw offset until EnBox is typed)
static inline u16 EnChanger_ChestOpened(Actor* chest) {
    return *(u16*)((u8*)chest + 0x38C);
}

void FUN_00157298(EnChanger* self, PlayState* play) {
    f32 zPos;
    f32 yPos;
    f32 xPos;
    s16 selectedChest;
    s16 unopenedChestItemType;
    Actor* left;
    Actor* right;

    left = self->leftChest;
    right = self->rightChest;

    if (self->timer == 0) {
        unopenedChestItemType = selectedChest = self->selectedChest;
        switch (selectedChest) {
            case 0:
                xPos = right->world.pos.x;
                yPos = right->world.pos.y;
                zPos = right->world.pos.z;
                if (self->rightChestGetItemId == 0x71) {
                    Actor_Spawn(&play->actorCtx, play, 0x168, xPos, yPos, zPos, 0, 0, 0, 0xF, 1);
                    Flags_SetSwitch(play, 0x32);
                } else {
                    unopenedChestItemType = (s16)(self->rightChestGetItemId - 0x68);
                    Actor_Spawn(&play->actorCtx, play, 0x168, xPos, yPos, zPos, 0, 0, 0, unopenedChestItemType, 1);
                }
                break;
            case 1:
                xPos = left->world.pos.x;
                yPos = left->world.pos.y;
                zPos = left->world.pos.z;
                if (self->leftChestGetItemId == 0x71) {
                    Actor_Spawn(&play->actorCtx, play, 0x168, xPos, yPos, zPos, 0, 0, 0, 0xF, 1);
                    Flags_SetSwitch(play, 0x32);
                } else {
                    unopenedChestItemType = (s16)(self->leftChestGetItemId - 0x68);
                    Actor_Spawn(&play->actorCtx, play, 0x168, xPos, yPos, zPos, 0, 0, 0, unopenedChestItemType, 1);
                }
                break;
        }
        Actor_Kill(&self->actor);
    }
}

void EnChanger_Destroy(Actor* actor, GameState* state) {
}

void FUN_0017963c(EnChanger* self, GameState* state) {
    if (EnChanger_ChestOpened(self->leftChest) != 0) {
        self->timer = 120;
        Flags_SetTreasure((PlayState*)state, self->rightChestNum & 0x1F);
        self->actionFunc = (EnChangerActionFunc)FUN_00157298;
    } else if (EnChanger_ChestOpened(self->rightChest) != 0) {
        self->selectedChest = 1;
        self->timer = 120;
        Flags_SetTreasure((PlayState*)state, self->leftChestNum & 0x1F);
        self->actionFunc = (EnChangerActionFunc)FUN_00157298;
    }
}

// N64 EnChanger_SetHeartPieceFlag: once the final chest is opened, set ITEMGETINF_1B (itemGetInf[1] bit 0x800)
void FUN_00239264(EnChanger* self, PlayState* play) {
    if (EnChanger_ChestOpened(self->finalChest) != 0) {
        if (!(gSaveContext.itemGetInf[1] & 0x800)) {
            gSaveContext.itemGetInf[1] |= 0x800;
        }
        Actor_Kill(&self->actor);
    }
}

void EnChanger_Update(Actor* actor, GameState* state) {
    EnChanger* self = (EnChanger*)actor;

    self->actionFunc(self, state);

    if (self->timer != 0) {
        self->timer--;
    }
}
}
