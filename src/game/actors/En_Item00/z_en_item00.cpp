extern "C" {
#include "z_en_item00.hpp"

#define FLAGS 0x00000000


void EnItem00_Init(Actor* actor, GameState* state);
void EnItem00_Destroy(Actor* actor, GameState* state);
void EnItem00_Update(Actor* actor, GameState* state);
void EnItem00_Draw(Actor* actor, GameState* state);

#define PLAY_FRAMES(p) (*(u32*)((u8*)(p) + 0x5BF4))
#define GAMEOVER_STATE(p) (*(u16*)((u8*)(p) + 0x318C))

typedef struct EnItem00Statics {
    /* 0x00 */ s16 dynaMoved;
    /* 0x02 */ char unk_02[0x0E];
    /* 0x10 */ u32 frame;
} EnItem00Statics;

extern EnItem00Statics EnItem00_sStatics;

void FUN_00376a78(PlayState* play, s32 itemId);
void FUN_00352dbc(PlayState* play, s32 amount);
void FUN_00346778(Actor* actor, PlayState* play, s32 getItemId);
s8 Object_GetIndex(ObjectContext* objectCtx, s16 objectId);
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_00369178(SkeletonAnimationModel* model, s32 idx);
void FUN_004c02e8(EnItem00* self, PlayState* play);

typedef struct EnItem00ModelEntry {
    /* 0x0 */ s16 objectId;
    /* 0x2 */ u8 cmbIndex;
    /* 0x3 */ u8 pad;
} EnItem00ModelEntry;

extern InitChainEntry EnItem00_sInitChain[];
extern ColliderCylinderInit EnItem00_sCylinderInit;
extern EnItem00ModelEntry EnItem00_sModelTable[];
void FUN_002b175c(EnItem00* self, PlayState* play);

GLOBAL_ASM("data/z_En_Item00.data.s")

ActorInit En_Item00_InitVars = {
    ACTOR_EN_ITEM00,
    ACTORCAT_MISC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnItem00),
    (ActorFunc)EnItem00_Init,
    (ActorFunc)EnItem00_Destroy,
    (ActorFunc)EnItem00_Update,
    (ActorFunc)EnItem00_Draw,
};

GLOBAL_ASM("asm/EnItem00_Init.s")

#if 0 // EnItem00_Init draft, ~110 words off: preamble temp regs (lsr r0 / and r0,#0xff before mov r0,r7), table ptr/zero regs sb<->sl, linker-inlined SetObjectDependency stub
static s32 EnItem00_LoadModel(EnItem00* self, PlayState* play) {
    s32 ok = 0;

    if (self->actor.objBankIndex >= 0) {
        FUN_00372f38(&self->actor, play, &self->model, EnItem00_sModelTable[self->actor.params].cmbIndex, 0);
        ok = 1;
    }
    return ok;
}

void EnItem00_Init(Actor* thisx, GameState* state) {
    EnItem00* self = (EnItem00*)thisx;
    PlayState* play = (PlayState*)state;
    f32 yOffset = 980.0f;
    f32 shadowScale = 6.0f;
    s32 getItemId = 0;
    s16 spawnParam8000;
    s16 flag;
    s32 loaded;

    self->actor.unk_19A = 1;
    spawnParam8000 = self->actor.params & 0x8000;
    self->unk_214 = (self->actor.params & 0x4000) >> 14;
    flag = (self->actor.params & 0x3F00) >> 8;
    self->collectibleFlag = flag;
    self->actor.params &= 0xFF;

    if (Flags_GetCollectible(play, flag)) {
        Actor_Kill(&self->actor);
        return;
    }

    Actor_ProcessInitChain(&self->actor, EnItem00_sInitChain);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &EnItem00_sCylinderInit);

    self->unk_1B0 = 1;

    switch (self->actor.params) {
        case 0:
        case 1:
        case 2:
            Actor_SetScale(&self->actor, 0.015f);
            self->scale = 0.015f;
            yOffset = 750.0f;
            break;
        case 0x11:
            self->unk_1B0 = 0;
            Actor_SetScale(&self->actor, 0.03f);
            self->scale = 0.03f;
            yOffset = 350.0f;
            break;
        case 6:
            self->unk_1B0 = 0;
            yOffset = 650.0f;
            Actor_SetScale(&self->actor, 0.02f);
            self->scale = 0.02f;
            break;
        case 3:
            self->actor.home.rot.z = Rand_CenteredFloat(65535.0f);
            yOffset = 430.0f;
            Actor_SetScale(&self->actor, 0.02f);
            self->scale = 0.02f;
            break;
        case 7:
            yOffset = 430.0f;
            self->unk_1B0 = 0;
            Actor_SetScale(&self->actor, 0.02f);
            self->scale = 0.02f;
            break;
        case 5:
            yOffset = 400.0f;
            Actor_SetScale(&self->actor, 0.02f);
            self->scale = 0.02f;
            break;
        case 8:
        case 9:
        case 0xA:
            Actor_SetScale(&self->actor, 0.035f);
            self->scale = 0.035f;
            yOffset = 250.0f;
            break;
        case 4:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xF:
        case 0x10:
        case 0x19:
            Actor_SetScale(&self->actor, 0.03f);
            self->scale = 0.03f;
            yOffset = 320.0f;
            break;
        case 0xE:
            Actor_SetScale(&self->actor, 0.045f);
            self->scale = 0.045f;
            yOffset = 320.0f;
            break;
        case 0x13:
            Actor_SetScale(&self->actor, 0.045f);
            self->scale = 0.045f;
            yOffset = 750.0f;
            break;
        case 0x14:
            Actor_SetScale(&self->actor, 0.03f);
            self->scale = 0.03f;
            yOffset = 750.0f;
            break;
        case 0x12:
            yOffset = 500.0f;
            Actor_SetScale(&self->actor, 0.01f);
            self->scale = 0.01f;
            break;
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
            // N64: Actor_SetObjectDependency(play, actor). Empty on 3DS (see Bg_Haka_Megane).
            ((void (*)(PlayState*, Actor*))GraphicsContext_UnknownStub)(play, &self->actor);
            Actor_SetScale(&self->actor, 0.5f);
            self->scale = 0.5f;
            yOffset = 0.0f;
            shadowScale = 0.6f;
            self->actor.world.rot.x = 0x4000;
            break;
    }

    self->actor.objBankIndex = Object_GetIndex(&play->objectCtx, EnItem00_sModelTable[self->actor.params].objectId);
    self->model = NULL;
    if (!EnItem00_LoadModel(self, play)) {
        Actor_Kill(&self->actor);
        return;
    }

    switch (self->actor.params) {
        case 2:
            FUN_00369178(self->model, 0x6E);
            break;
        case 0:
            FUN_00369178(self->model, 0x6C);
            break;
        case 1:
            FUN_00369178(self->model, 0x6D);
            break;
        case 0x13:
            FUN_00369178(self->model, 0x71);
            break;
        case 0x14:
            FUN_00369178(self->model, 0x70);
            break;
    }

    self->unk_1AE = 0;
    ActorShape_Init(&self->actor.shape, yOffset, ActorShadow_DrawCircle, shadowScale);
    self->actor.shape.shadowAlpha = 180;
    self->actor.focus.pos = self->actor.world.pos;
    self->getItemId = 0;

    if (!spawnParam8000) {
        self->actionFunc = FUN_004c02e8;
        self->despawnTimer = -1;
        return;
    }

    self->despawnTimer = 15;
    self->unk_1AC = 35;

    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = 0.0f;

    switch (self->actor.params) {
        case 0:
            FUN_00376a78(play, 0x84);
            break;
        case 1:
            FUN_00376a78(play, 0x85);
            break;
        case 2:
            FUN_00376a78(play, 0x86);
            break;
        case 0x14:
            FUN_00376a78(play, 0x87);
            break;
        case 0x13:
            FUN_00376a78(play, 0x88);
            break;
        case 3:
            FUN_00376a78(play, 0x83);
            break;
        case 0x12:
            FUN_00352dbc(play, 0x70);
            break;
        case 4:
        case 0xB:
            FUN_00376a78(play, 0x8E);
            break;
        case 5:
            FUN_00376a78(play, 3);
            break;
        case 8:
            FUN_00376a78(play, 0x92);
            break;
        case 9:
            FUN_00376a78(play, 0x93);
            break;
        case 0xA:
            FUN_00376a78(play, 0x94);
            break;
        case 0xE:
            getItemId = 0x43;
            break;
        case 0xF:
            getItemId = 0x44;
            break;
        case 0x11:
            FUN_00376a78(play, 0x77);
            break;
        case 0x10:
            getItemId = 0x3C;
            break;
        case 0xC:
            getItemId = 2;
            break;
        case 0xD:
            getItemId = 7;
            break;
    }

    if ((getItemId != 0) && !Actor_HasParent(&self->actor, play)) {
        FUN_00346778(&self->actor, play, getItemId);
    }

    self->actionFunc = (void (*)(EnItem00*, PlayState*))FUN_002b175c;
    FUN_002b175c(self, play);
}
#endif

GLOBAL_ASM("../src/game/actors/En_Item00/data_syms.s")

void EnItem00_Destroy(Actor* actor, GameState* state) {
    EnItem00* self = (EnItem00*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(actor, &self->model, 0);
}

void FUN_002b175c(EnItem00* self, PlayState* play) {
    Player* player = (Player*)GET_PLAYER(play);

    if (self->getItemId != 0) {
        if (!Actor_HasParent(&self->actor, play)) {
            GiveItem(&self->actor, play, self->getItemId, 50.0f, 80.0f);
            self->despawnTimer++;
        } else {
            self->getItemId = 0;
        }
    }

    if (self->despawnTimer == 0) {
        Actor_Kill(&self->actor);
        return;
    }

    if (player->interactRangeActor != NULL && player->interactRangeActor != &self->actor) {
        Actor_Kill(&self->actor);
    }

    self->actor.world.pos = player->actor.world.pos;

    if (self->actor.params <= 3 || self->actor.params == 0x11) {
        self->actor.shape.rot.y += 0x280;
    }

    self->actor.world.pos.y += 40.0f + Math_SinS((s16)self->despawnTimer * 15000) * ((s16)self->despawnTimer * 0.3f);

    if (LINK_IS_ADULT) {
        self->actor.world.pos.y += 20.0f;
    }
}

GLOBAL_ASM("asm/EnItem00_Update.s")

#if 0 // EnItem00_Update draft (N64 shape + 3DS offsets): r4/r5 swapped (self lands in r4) and ROM keeps sp3A=1 in r8 unfolded (`ldrsh D94; orrs r0,r0,r8`)
void EnItem00_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnItem00* self = (EnItem00*)thisx;
    s16* params;
    Actor* dynaActor;
    s32 getItemId = 0;
    s16 sp3A = 0;
    s16 i;

    if (self->despawnTimer > 0) {
        self->despawnTimer--;
    }

    if ((self->despawnTimer > 0) && (self->despawnTimer <= 40) && (self->unk_1AC <= 0)) {
        self->unk_1AE = self->despawnTimer;
    }

    self->actionFunc(self, play);
    Math_SmoothStepToF(&self->actor.scale.x, self->scale, 0.1f, self->scale * 0.1f, 0.0f);

    self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;

    if (self->actor.gravity != 0.0f) {
        sp3A = 1;
        Actor_MoveForward(&self->actor);
        if (EnItem00_sStatics.frame != PLAY_FRAMES(play)) {
            EnItem00_sStatics.frame = PLAY_FRAMES(play);
            EnItem00_sStatics.dynaMoved = 0;
            for (i = 0; i < 50; i++) {
                if (play->colCtx.dyna.bgActorFlags[i] & 1) {
                    dynaActor = play->colCtx.dyna.actorMeshArr[i].actor;
                    if ((dynaActor != NULL) && (dynaActor->update != NULL)) {
                        if ((dynaActor->world.pos.x != dynaActor->prevPos.x) ||
                            (dynaActor->world.pos.y != dynaActor->prevPos.y) ||
                            (dynaActor->world.pos.z != dynaActor->prevPos.z)) {
                            EnItem00_sStatics.dynaMoved++;
                            break;
                        }
                    }
                }
            }
        }

        if (sp3A || EnItem00_sStatics.dynaMoved) {
            Actor_UpdateBgCheckInfo(play, &self->actor, 10.0f, 15.0f, 15.0f, 0x1D);

            if (self->actor.floorHeight <= -10000.0f) {
                Actor_Kill(&self->actor);
                return;
            }
        }
    }

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);

    if ((self->actor.params == 0x15) || (self->actor.params == 0x16) || (self->actor.params == 0x17) ||
        (self->actor.params == 0x18)) {
        self->actor.shape.yOffset = Math_CosS(self->actor.shape.rot.x) * 37.0f;
        self->actor.shape.yOffset = (self->actor.shape.yOffset >= 0.0f) ? self->actor.shape.yOffset : -self->actor.shape.yOffset;
    }

    if (self->unk_1AC > 0) {
        return;
    }

    if (!((self->actor.xzDistToPlayer <= 30.0f) && (self->actor.yDistToPlayer >= -50.0f) &&
          (self->actor.yDistToPlayer <= 50.0f))) {
        if (!Actor_HasParent(&self->actor, play)) {
            return;
        }
    }

    if (GAMEOVER_STATE(play) != 0) {
        return;
    }

    switch (self->actor.params) {
        case 0:
            FUN_00376a78(play, 0x84);
            break;
        case 1:
            FUN_00376a78(play, 0x85);
            break;
        case 2:
            FUN_00376a78(play, 0x86);
            break;
        case 0x14:
            FUN_00376a78(play, 0x87);
            break;
        case 0x13:
            FUN_00376a78(play, 0x88);
            break;
        case 0xD:
            getItemId = 7;
            break;
        case 0xC:
            getItemId = 2;
            break;
        case 3:
            FUN_00376a78(play, 0x83);
            break;
        case 0x12:
            FUN_00352dbc(play, 0x70);
            break;
        case 4:
        case 0xB:
            FUN_00376a78(play, 0x8E);
            break;
        case 5:
            FUN_00376a78(play, 3);
            break;
        case 8:
            FUN_00376a78(play, 0x92);
            break;
        case 9:
            FUN_00376a78(play, 0x93);
            break;
        case 0xA:
            FUN_00376a78(play, 0x94);
            break;
        case 0x10:
            getItemId = 0x3C;
            break;
        case 0x11:
            getItemId = 0x42;
            break;
        case 6:
            getItemId = 0x3E;
            break;
        case 7:
            getItemId = 0x3D;
            break;
        case 0xE:
            getItemId = 0x44;
            break;
        case 0xF:
            getItemId = 0x43;
            break;
        case 0x15:
            getItemId = 0x29;
            break;
        case 0x16:
            getItemId = 0x2A;
            break;
        case 0x17:
            getItemId = 0x2D;
            break;
        case 0x18:
            getItemId = 0x2C;
            break;
    }

    params = &self->actor.params;

    if ((getItemId != 0) && !Actor_HasParent(&self->actor, play)) {
        FUN_00346778(&self->actor, play, getItemId);
    }

    switch (*params) {
        case 6:
        case 7:
        case 0x11:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
            if (Actor_HasParent(&self->actor, play)) {
                Flags_SetCollectible(play, self->collectibleFlag);
                Actor_Kill(&self->actor);
            }
            break;
        default:
            if ((*params <= 2) || (*params == 0x13)) {
                Audio_PlaySoundGeneral(0x01000488, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            } else if (getItemId != 0) {
                if (Actor_HasParent(&self->actor, play)) {
                    Flags_SetCollectible(play, self->collectibleFlag);
                    Actor_Kill(&self->actor);
                }
                return;
            } else {
                Audio_PlaySoundGeneral(0x010004A9, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
            }

            Flags_SetCollectible(play, self->collectibleFlag);

            self->despawnTimer = 15;
            self->unk_1AC = 35;
            self->actor.shape.rot.z = 0;
            self->actor.speedXZ = 0;
            self->actor.velocity.y = 0;
            self->actor.gravity = 0;

            Actor_SetScale(&self->actor, self->scale);

            self->getItemId = 0;
            self->actionFunc = (void (*)(EnItem00*, PlayState*))FUN_002b175c;
            break;
    }
    self->unk_214 = 0;
}
#endif

void EnItem00_Draw(Actor* thisx, GameState* state) {
    EnItem00* self = (EnItem00*)thisx;
    SkeletonAnimationModel* model;
    MTX34* mtx;
    if (self->unk_1AE & self->unk_1B0) {
        return;
    }
    model = self->model;
    mtx = &thisx->modelMtx;
    if (model == NULL) {
        return;
    }
    if (self->unk_214 != 0) {
        return;
    }
    self->model->FUN_003721e0(mtx);
    self->model->SetUnkAC(1);
    self->model->FUN_00372170(0);
}

}
