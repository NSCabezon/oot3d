extern "C" {
#include "z_bg_relay_objects.hpp"

#define FLAGS 0x80400010

void BgRelayObjects_Init(Actor* actor, GameState* state);
void BgRelayObjects_Destroy(Actor* actor, GameState* state);
void BgRelayObjects_Update(Actor* actor, GameState* state);
void BgRelayObjects_Draw(Actor* actor, GameState* state);

__attribute__((noinline)) void FUN_00214874(void);
s32 FUN_00366684(s32 arg0);
void FUN_00181c84(BgRelayObjects* self, PlayState* play); // N64: BgRelayObjects_DoNothing (bx lr)
void FUN_00181f38(BgRelayObjects* self, PlayState* play); // N64: func_808A90F4
void FUN_001596b8(BgRelayObjects* self, PlayState* play); // N64: func_808A91AC
void FUN_003cacd0(BgRelayObjects* self, PlayState* play); // N64: func_808A939C (rotating gear)


GLOBAL_ASM("data/z_Bg_Relay_Objects.data.s")

ActorInit Bg_Relay_Objects_InitVars = {
    ACTOR_BG_RELAY_OBJECTS,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_RELAY_OBJECTS,
    sizeof(BgRelayObjects),
    (ActorFunc)BgRelayObjects_Init,
    (ActorFunc)BgRelayObjects_Destroy,
    (ActorFunc)BgRelayObjects_Update,
    (ActorFunc)BgRelayObjects_Draw,
};

// Init chain data lives at 0x0051355C (see data/z_Bg_Relay_Objects.data.s); no symbol yet.
#define sInitChain ((InitChainEntry*)0x0051355C)

#if 0
// NON_MATCHING draft (Haiku pass): differs from ROM in the dampe-race branch (shared Actor_Kill block merged
// by the ROM compiler; ours duplicates it) and in the flag/room bit ops. Best attempt: 18 of 145 words differ.
void BgRelayObjects_Init(Actor* actor, GameState* state) {
    BgRelayObjects* self = (BgRelayObjects*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    self->switchFlag = actor->params & 0x3F;
    actor->params = (actor->params >> 8) & 0xFF;
    DynaPolyActor_Init(&self->dyna, 3);
    FUN_00372f38(actor, play, &self->skelAnimModel, 1, &self->skelAnimModel2, 0, 0);
    if (actor->params == 0) {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
        if (*(u16*)((u8*)&gSaveContext + 0x800 + 0xF8) & 0x20) {
            actor->world.rot.y = 0x2AB;
        } else {
            actor->world.rot.y = 0x55;
        }
        FUN_00214874();
        actor->room = -1;
        actor->flags |= 0x20;
        if (*(u32*)((u8*)play + 0x7F70) & 2) {
            actor->params = 0xFF;
            Actor_Kill(actor);
        } else {
            *(u32*)((u8*)play + 0x7F70) |= 2;
            self->actionFunc = FUN_003cacd0;
        }
    } else {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 0), &colHeader);
        if (actor->room == 0) {
            self->unk_1C1 = self->switchFlag - 0x33;
        } else {
            self->unk_1C1 = actor->room + 1;
        }
        actor->room = -1;
        self->timer = 1;
        if (self->unk_1C1 > 5) {
            if (*(u32*)((u8*)play + 0x7F70) & 1) {
                Actor_Kill(actor);
            } else {
                *(u32*)((u8*)play + 0x7F70) |= 1;
                self->actionFunc = FUN_00181c84;
            }
        } else if (self->unk_1C1 != 5) {
            Flags_UnsetSwitch(play, self->switchFlag);
            if (*(u32*)((u8*)play + 0x7F70) & (1 << self->unk_1C1)) {
                Actor_Kill(actor);
            } else {
                *(u32*)((u8*)play + 0x7F70) |= (1 << self->unk_1C1);
                self->actionFunc = FUN_00181f38;
            }
        } else {
            Flags_SetSwitch(play, self->switchFlag);
            actor->world.pos.y += 120.0f;
            self->actionFunc = FUN_001596b8;
            *(u32*)((u8*)play + 0x7F70) |= 1;
        }
    }
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
}

#endif
GLOBAL_ASM("asm/BgRelayObjects_Init.s")

void BgRelayObjects_Destroy(Actor* actor, GameState* state) {
    BgRelayObjects* self = (BgRelayObjects*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    if ((actor->params == 0) && (*(s32*)((u8*)&gSaveContext + 8) < (s32)0xFFF0)) {
        *(u16*)((u8*)&gSaveContext + 0xEF8) &= ~0x20;
    }
    FUN_00350f34(actor, &self->skelAnimModel, &self->skelAnimModel2, 0);
}

void BgRelayObjects_Update(Actor* actor, GameState* state) {
    BgRelayObjects* self = (BgRelayObjects*)actor;

    self->actionFunc(self, (PlayState*)state);
}

#if 0
// NON_MATCHING draft: ROM interleaves the two branches (mov r1,#1 hoisted, ldrh/cmp register choice differs).
// 7 of 30 words differ.
void BgRelayObjects_Draw(Actor* actor, GameState* state) {
    BgRelayObjects* self = (BgRelayObjects*)actor;
    MTX34 mtx;
    s32 one = 1;

    MTX34CopyAsm(&mtx, (MTX34*)((u8*)actor + 0x148));
    if (actor->params == 0) {
        ((u8*)self->skelAnimModel)[0xAC] = one;
        self->skelAnimModel->FUN_003721e0(&mtx);
        self->skelAnimModel->FUN_00372170(0);
    } else {
        ((u8*)self->skelAnimModel2)[0xAC] = one;
        self->skelAnimModel2->FUN_003721e0(&mtx);
        self->skelAnimModel2->FUN_00372170(0);
    }
}

#endif
GLOBAL_ASM("asm/BgRelayObjects_Draw.s")

void FUN_00214874(void) {
    if (FUN_00366684(0) != 0x010005B9) {
        Audio_QueueSeqCmd(0, 0x010005B9, 0);
    }
}

}
