extern "C" {
#include "z_demo_gj.hpp"

#define FLAGS 0x00400030

void DemoGj_Init(Actor* thisx, PlayState* play);
void DemoGj_Destroy(Actor* thisx, PlayState* play);
void DemoGj_Update(Actor* thisx, PlayState* play);
void DemoGj_Draw(Actor* thisx, PlayState* play);

typedef void (*DemoGjUpdateFunc)(DemoGj* self, PlayState* play);
typedef void (*DemoGjDrawFunc)(Actor* thisx, PlayState* play);

void DemoGj_SetupRotation(DemoGj* self, PlayState* play);

GLOBAL_ASM("data/z_Demo_Gj.data.s")

// sUpdateFuncs / sDrawFuncs live in the data dump above (no labels) -> absolute symbols (EQU) in data_syms.s.
GLOBAL_ASM("../src/game/actors/Demo_Gj/data_syms.s")
extern DemoGjUpdateFunc Demo_Gj_sUpdateFuncs[];
extern DemoGjDrawFunc Demo_Gj_sDrawFuncs[];

ActorInit Demo_Gj_InitVars = {
    ACTOR_DEMO_GJ,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GJ,
    sizeof(DemoGj),
    (ActorFunc)DemoGj_Init,
    (ActorFunc)DemoGj_Destroy,
    (ActorFunc)DemoGj_Update,
    (ActorFunc)DemoGj_Draw,
};

static void DemoGj_DestroyCylinder(Actor* p, PlayState* play) {
    DemoGj* self = (DemoGj*)p;

    switch (((u16)self->dyna.actor.params & 0xFF)) {
        case 0x10:
            Collider_DestroyCylinder(play, &self->colliderCylinders[0]);
            Collider_DestroyCylinder(play, &self->colliderCylinders[1]);
            Collider_DestroyCylinder(play, &self->colliderCylinders[2]);
            break;
        case 0x11:
            Collider_DestroyCylinder(play, &self->colliderCylinders[0]);
            Collider_DestroyCylinder(play, &self->colliderCylinders[1]);
            Collider_DestroyCylinder(play, &self->colliderCylinders[2]);
            break;
        case 0x16:
            Collider_DestroyCylinder(play, &self->colliderCylinders[0]);
            break;
    }
}

void DemoGj_Destroy(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    FUN_00350f34(thisx, &self->unk_2F4, &self->unk_2F8, &self->unk_2FC, &self->unk_300, &self->unk_304,
                 &self->unk_308, &self->unk_30C, &self->unk_310, 0);
    DemoGj_DestroyCylinder(thisx, play);
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
}

void DemoGj_Update(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;
    DemoGjUpdateFunc updateFunc;

    if ((self->updateMode < 0) || (self->updateMode >= 0x15) ||
        (updateFunc = Demo_Gj_sUpdateFuncs[self->updateMode]) == NULL) {
        return;
    }

    updateFunc(self, play);
}

void DemoGj_Draw(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;
    DemoGjDrawFunc drawFunc;

    if ((self->drawConfig < 0) || (self->drawConfig >= 0x13) ||
        (drawFunc = Demo_Gj_sDrawFuncs[self->drawConfig]) == NULL) {
        return;
    }

    drawFunc(thisx, play);
}

// Model-draw shape shared by the caseD_* draw entries (model->SetUnkAC(1); FUN_003721e0(&modelMtx); FUN_00372170(0)).
void caseD_1_0037cc68(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_2F4;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_2_0038d594(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_2F8;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_3_0038d5c0(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_2FC;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_4_0038d5ec(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_300;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_5_0038d618(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_304;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_10_002ae21c(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_2F8;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_11_002ae248(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_2FC;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_12_002ae274(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_310;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_6_0038d644(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_308;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_7_0038d670(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_30C;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

void caseD_8_0038d69c(Actor* thisx, PlayState* play) {
    SkeletonAnimationModel* model = ((DemoGj*)thisx)->unk_310;

    model->SetUnkAC(1);
    model->FUN_003721e0(&thisx->modelMtx);
    model->FUN_00372170(0);
}

#define IS_CUTSCENE_LAYER (gSaveContext.sceneSetupIndex >= 4)

extern InitChainEntry Demo_Gj_sInitChain[];

// N64 DemoGj_InitCommon (3DS: takes the Actor* and casts inside; the ROM keeps a separate copy for the bgId store).
static void DemoGj_InitCommon(Actor* p, PlayState* play, CollisionHeader* header) {
    CollisionHeader* newHeader;

    if (header != NULL) {
        DemoGj* self = (DemoGj*)p;

        Actor_ProcessInitChain(p, Demo_Gj_sInitChain);
        DynaPolyActor_Init((DynaPolyActor*)p, 0);
        newHeader = NULL;
        ReadCollisionHeaderFromZsi(header, &newHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, p, newHeader);
    }
}

// N64 DemoGj_InitSetIndices (name kept from the ROM symbol).
s32 DemoGj_InitSetIndexes(DemoGj* self, PlayState* play, s32 updateMode, s32 drawConfig, CollisionHeader* header) {
    CollisionHeader* newHeader;

    if (!IS_CUTSCENE_LAYER) {
        self->updateMode = updateMode;
        self->drawConfig = drawConfig;
        if (header != NULL) {
            Actor_ProcessInitChain(&self->dyna.actor, Demo_Gj_sInitChain);
            DynaPolyActor_Init(&self->dyna, 0);
            newHeader = NULL;
            ReadCollisionHeaderFromZsi(header, &newHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, newHeader);
        }
        return true;
    }
    Actor_Kill(&self->dyna.actor);
    return false;
}

void Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* init);

extern ColliderCylinderInit Demo_Gj_sCylinderInit1;
extern ColliderCylinderInit Demo_Gj_sCylinderInit2;
extern ColliderCylinderInit Demo_Gj_sCylinderInit3;

static void DemoGj_InitCylinder(Actor* p, PlayState* play, ColliderCylinder* collider,
                                ColliderCylinderInit* colliderInit) {
    Collider_InitCylinder(play, collider);
    Collider_SetCylinderType1(play, collider, p, colliderInit);
}

static void DemoGj_InitDestructableRubble1(Actor* p, PlayState* play) {
    DemoGj* self = (DemoGj*)p;

    DemoGj_InitSetIndexes(self, play, 15, 0, NULL);
    DemoGj_InitCylinder(p, play, &self->colliderCylinders[0], &Demo_Gj_sCylinderInit1);
    DemoGj_InitCylinder(p, play, &self->colliderCylinders[1], &Demo_Gj_sCylinderInit1);
    DemoGj_InitCylinder(p, play, &self->colliderCylinders[2], &Demo_Gj_sCylinderInit1);
}

static void DemoGj_InitDestructableRubble2(Actor* p, PlayState* play) {
    DemoGj* self = (DemoGj*)p;

    DemoGj_InitSetIndexes(self, play, 16, 0, NULL);
    DemoGj_InitCylinder(p, play, &self->colliderCylinders[0], &Demo_Gj_sCylinderInit2);
    DemoGj_InitCylinder(p, play, &self->colliderCylinders[1], &Demo_Gj_sCylinderInit2);
    DemoGj_InitCylinder(p, play, &self->colliderCylinders[2], &Demo_Gj_sCylinderInit2);
}

static void DemoGj_InitDestructableRubble3(Actor* p, PlayState* play) {
    DemoGj* self = (DemoGj*)p;

    DemoGj_InitSetIndexes(self, play, 17, 0, NULL);
    DemoGj_InitCylinder(p, play, &self->colliderCylinders[0], &Demo_Gj_sCylinderInit3);
}

void DemoGj_Init(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    switch ((u16)thisx->params & 0xFF) {
        case 4:
            DemoGj_InitSetIndexes(self, play, 0, 1, (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0));
            FUN_00372f38(thisx, play, &self->unk_2F4, 0, 0);
            break;

        case 8:
            DemoGj_InitSetIndexes(self, play, 1, 2, (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 1));
            FUN_00372f38(thisx, play, &self->unk_2F8, 1, 0);
            break;

        case 9:
            DemoGj_InitSetIndexes(self, play, 2, 3, (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 2));
            FUN_00372f38(thisx, play, &self->unk_2FC, 2, 0);
            break;

        case 10:
            DemoGj_InitSetIndexes(self, play, 3, 4, (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 3));
            FUN_00372f38(thisx, play, &self->unk_300, 3, 0);
            break;

        case 11:
            DemoGj_InitSetIndexes(self, play, 4, 5, (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 4));
            FUN_00372f38(thisx, play, &self->unk_304, 4, 0);
            break;

        case 12:
            DemoGj_InitSetIndexes(self, play, 5, 6, (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 5));
            FUN_00372f38(thisx, play, &self->unk_308, 5, 0);
            break;

        case 13:
            DemoGj_InitSetIndexes(self, play, 6, 7, (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 6));
            FUN_00372f38(thisx, play, &self->unk_30C, 6, 0);
            break;

        case 16:
            DemoGj_InitDestructableRubble1(thisx, play);
            FUN_00372f38(thisx, play, &self->unk_2F8, 1, 0);
            break;

        case 17:
            DemoGj_InitDestructableRubble2(thisx, play);
            FUN_00372f38(thisx, play, &self->unk_2FC, 2, 0);
            break;

        case 22:
            DemoGj_InitDestructableRubble3(thisx, play);
            FUN_00372f38(thisx, play, &self->unk_310, 7, 0);
            break;

        default:
            Actor_Kill(thisx);
            break;
    }
}

#if 0 // NON_MATCHING draft: DemoGj_SetupRotation (22 words off: same code; prologue scheduling differs (ROM loads the type into r1 and &gGameInfo late), which moves the literal pool 8 bytes. Dead `Actor* a2 = actor;` alias on the yPosition load took it from 85 to 24 words. ROM tail has a separate 8-byte function at 0x31f384 -> needs cfg split 0x0031f384 FUN_0031f384)
void DemoGj_SetupRotation(DemoGj* self, PlayState* play) {
    Actor* actor = &self->dyna.actor;
    Actor* a2 = actor;
    f32 yPosition = a2->world.pos.y;
    f32* speedXZ = &actor->speedXZ;
    f32* yVelocity = &actor->velocity.y;
    Vec3s* unk_172 = &self->unk_1CA;
    f32 verticalTranslation;
    Vec3f vec;
    f32 verticalFactor;
    f32 xzPlaneFactor;

    switch ((u16)actor->params & 0xFF) {
        case 8:
            verticalTranslation = kREG(23);
            vec.x = kREG(24) * 0.01f + 1.0f;
            vec.y = kREG(25) * 0.01f + 1.0f;
            vec.z = kREG(26) * 0.01f + 1.0f;
            verticalFactor = kREG(27) * 0.01f + -1.0f;
            xzPlaneFactor = kREG(28) * 0.01f + 1.0f;
            break;

        case 9:
            verticalTranslation = kREG(36);
            vec.x = kREG(37) * 0.01f + 1.0f;
            vec.y = kREG(38) * 0.01f + 1.0f;
            vec.z = kREG(39) * 0.01f + 1.0f;
            verticalFactor = kREG(40) * 0.01f + -1.0f;
            xzPlaneFactor = kREG(41) * 0.01f + 1.0f;
            break;

        case 10:
            verticalTranslation = kREG(49);
            vec.x = kREG(50) * 0.01f + 1.0f;
            vec.y = kREG(51) * 0.01f + 1.0f;
            vec.z = kREG(52) * 0.01f + 1.0f;
            verticalFactor = kREG(53) * 0.01f + -1.0f;
            xzPlaneFactor = kREG(54) * 0.01f + 1.0f;
            break;

        case 11:
            verticalTranslation = kREG(62);
            vec.x = kREG(63) * 0.01f + 1.0f;
            vec.y = kREG(64) * 0.01f + 1.0f;
            vec.z = kREG(65) * 0.01f + 1.0f;
            verticalFactor = kREG(66) * 0.01f + -1.0f;
            xzPlaneFactor = kREG(67) * 0.01f + 1.0f;
            break;

        case 12:
            verticalTranslation = kREG(75);
            vec.x = kREG(76) * 0.01f + 1.0f;
            vec.y = kREG(77) * 0.01f + 1.0f;
            vec.z = kREG(78) * 0.01f + 1.0f;
            verticalFactor = kREG(79) * 0.01f + -1.0f;
            xzPlaneFactor = kREG(80) * 0.01f + 1.0f;
            break;

        case 13:
            verticalTranslation = kREG(88);
            vec.x = kREG(89) * 0.01f + 1.0f;
            vec.y = kREG(90) * 0.01f + 1.0f;
            vec.z = kREG(91) * 0.01f + 1.0f;
            verticalFactor = kREG(92) * 0.01f + -1.0f;
            xzPlaneFactor = kREG(93) * 0.01f + 1.0f;
            break;

        case 14:
            verticalTranslation = kREG(10) + -190.0f;
            vec.x = kREG(9) * 0.01f + 1.0f;
            vec.y = kREG(8) * 0.01f + 1.0f;
            vec.z = kREG(7) * 0.01f + 1.0f;
            verticalFactor = kREG(6) * 0.01f + -1.0f;
            xzPlaneFactor = kREG(5) * 0.01f + 1.0f;
            break;

        default:
            return;
    }

    yPosition += verticalTranslation;
    if (yPosition <= 1086.0f && (*yVelocity < 0.0f)) {
        if (!self->isRotated) {
            *yVelocity *= verticalFactor;
            *speedXZ *= xzPlaneFactor;

            unk_172->x *= vec.x;
            unk_172->y *= vec.y;
            unk_172->z *= vec.z;

            if (*yVelocity <= -actor->gravity) {
                *yVelocity = 0.0f;
                *speedXZ = 0.0f;

                unk_172->x = 0;
                unk_172->y = 0;
                unk_172->z = 0;
            }

            self->isRotated = true;
        }
    }
}
#endif

GLOBAL_ASM("asm/DemoGj_SetupRotation.s")

// DemoGj_IsGanondorfFloatingInAir (N64 func_809797E4(this, 2)). The ganon mode byte sits at +0xA10 of the
// 3DS BossGanon2 layout, which is not modelled in Boss_Ganon2's header yet (debt).
typedef struct DemoGjGanonView {
    /* 0x0000 */ char unk_00[0xA10];
    /* 0x0A10 */ u8 mode;
} DemoGjGanonView;

// N64 func_809797E4(this, mode).
static s32 DemoGj_CheckGanonMode(DemoGj* self, u8 mode) {
    DemoGjGanonView* ganon = (DemoGjGanonView*)self->ganon;

    if ((ganon != NULL) && (ganon->mode == mode)) {
        return true;
    }
    return false;
}

static s32 DemoGj_IsGanondorfFloatingInAir(DemoGj* self) {
    return DemoGj_CheckGanonMode(self, 2);
}

void caseD_8_0037cc94(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    Actor_MoveForward(thisx);
    self->rotationVec.x += kREG(18);
    self->rotationVec.y += (s16)(kREG(19) + 1000);
    self->rotationVec.z += (s16)(kREG(20) + 3000);

    DemoGj_SetupRotation(self, play);

    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        Actor_Kill(thisx);
    }
}

void caseD_9_0037cd38(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    Actor_MoveForward(thisx);
    self->rotationVec.x += kREG(31);
    self->rotationVec.y += (s16)(kREG(32) + 1000);
    self->rotationVec.z += (s16)(kREG(33) + 3000);

    DemoGj_SetupRotation(self, play);

    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        Actor_Kill(thisx);
    }
}

void caseD_a_0037cddc(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    Actor_MoveForward(thisx);
    self->rotationVec.x += kREG(44);
    self->rotationVec.y += (s16)(kREG(45) + 1000);
    self->rotationVec.z += (s16)(kREG(46) + 3000);

    DemoGj_SetupRotation(self, play);

    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        Actor_Kill(thisx);
    }
}

void caseD_b_0037ce80(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    Actor_MoveForward(thisx);
    self->rotationVec.x += kREG(57);
    self->rotationVec.y += (s16)(kREG(58) + 1000);
    self->rotationVec.z += (s16)(kREG(59) + 3000);

    DemoGj_SetupRotation(self, play);

    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        Actor_Kill(thisx);
    }
}

void caseD_c_0037cf24(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    Actor_MoveForward(thisx);
    self->rotationVec.x += kREG(70);
    self->rotationVec.y += (s16)(kREG(71) + 1000);
    self->rotationVec.z += (s16)(kREG(72) + 3000);

    DemoGj_SetupRotation(self, play);

    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        Actor_Kill(thisx);
    }
}

void caseD_d_0037cfc8(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    Actor_MoveForward(thisx);
    self->rotationVec.x += kREG(83);
    self->rotationVec.y += (s16)(kREG(84) + 1000);
    self->rotationVec.z += (s16)(kREG(85) + 3000);

    DemoGj_SetupRotation(self, play);

    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        Actor_Kill(thisx);
    }
}

// N64 DemoGj_FindGanon: searches the boss list for Ganondorf while `ganon` is unset.
static s32 DemoGj_FindGanon(DemoGj* self, PlayState* play) {
    Actor* actor;

    if (self->ganon == NULL) {
        actor = play->actorCtx.actorLists[ACTORCAT_BOSS].head;
        while (actor != NULL) {
            if (actor->id == ACTOR_BOSS_GANON2) {
                self->ganon = (BossGanon2*)actor;
                return true;
            }
            actor = actor->next;
        }
        return false;
    }
}

// Shared tail of the caseD_10/11/f entries: collision from ZSI `index`, update/draw config, scale * 0.8.
static void DemoGj_SetupShrunkRubble(Actor* p, PlayState* play, s32 index, s32 updateMode, s32 drawConfig) {
    DemoGj* self = (DemoGj*)p;
    Vec3f* scale = &self->dyna.actor.scale;

    DemoGj_InitCommon(p, play, (CollisionHeader*)DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, index));
    self->updateMode = updateMode;
    self->drawConfig = drawConfig;
    scale->x *= 0.8f;
    scale->y *= 0.8f;
    scale->z *= 0.8f;
}

void caseD_f_0028e374(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    DemoGj_FindGanon(self, play);
    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        DemoGj_SetupShrunkRubble(thisx, play, 1, 0x12, 0x10);
    }
}

void caseD_10_0028e4a0(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    DemoGj_FindGanon(self, play);
    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        DemoGj_SetupShrunkRubble(thisx, play, 2, 0x13, 0x11);
    }
}

void caseD_11_0028e5cc(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    DemoGj_FindGanon(self, play);
    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        DemoGj_SetupShrunkRubble(thisx, play, 7, 0x14, 0x12);
    }
}

// N64 DemoGj_Explode (ROM FUN_003216b0): six kakera bursts around initialPos, then the explosion sfx.
void FUN_003216b0(DemoGj* self, PlayState* play, Vec3f* initialPos, Vec3f* direction) {
    Vec3f explosionPos;
    Vec3f velocity;
    s16 phi;
    f32 aux;
    s32 life;
    s16 scale;
    s16 theta = 0;
    s32 i;

    for (i = 0; i < 6; i++) {
        explosionPos.x = Math_SinS(theta) * 16.0f;
        explosionPos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        explosionPos.z = Math_CosS(theta) * 16.0f;

        velocity.x = (explosionPos.x * 0.6f) + (12.0f * direction->x);
        velocity.y = (Rand_ZeroOne() * 36.0f) + 6.0f;
        velocity.z = (explosionPos.z * 0.6f) + (12.0f * direction->z);

        explosionPos.x += initialPos->x;
        explosionPos.y += initialPos->y;
        explosionPos.z += initialPos->z;

        aux = Rand_ZeroOne();
        if (aux < 0.1f) {
            phi = 0x61;
        } else if (aux < 0.7f) {
            phi = 0x41;
        } else {
            phi = 0x21;
        }

        life = (s32)(Rand_ZeroOne() * 30.0f) + 30;
        scale = (s16)((Rand_ZeroOne() * 20.0f) + 20.0f);
        EffectSsKakera_Spawn(play, &explosionPos, &velocity, initialPos, -200, phi, 10, 10, 0, scale, 20, 300, life,
                             -1, OBJECT_GEFF, NULL);

        theta += 0x2AAA;
    }

    {
        PlayState* play2 = play;

        Audio_PlaySoundAtPosition(play2, &self->dyna.actor.world.pos, 50, 0x01000219);
    }
}

#if 0 // NON_MATCHING drafts: caseD_12_002ae2a0 (~150 words off) / caseD_13_002ae53c (~146 words off). Same shape as caseD_14 (N64 func_8097B22C + cylinder move + SetAC); ROM keeps TWO copies of the actor pointer (r5 for everything, r4 only for killFlag/unk_2E8) and VFP numbering of the zero vector differs.
static inline s32 DemoGj_GetCollectibleType(DemoGj* self) {
    s32 type = (self->dyna.actor.params >> 11) & 0x1F;

    return type;
}

static inline s32 DemoGj_GetCollectibleAmount(DemoGj* self) {
    s32 amount = (self->dyna.actor.params >> 8) & 0x7;

    return amount;
}

static inline s32 DemoGj_HitByExplosion(DemoGj* self, PlayState* play, ColliderCylinder* collider) {
    if (Actor_GetCollidedExplosive(play, &collider->base) != NULL) {
        return true;
    }
    return false;
}

static inline void DemoGj_DropCollectible(DemoGj* self, PlayState* play) {
    Vec3f* pos = &self->dyna.actor.world.pos;
    s16 collectible = DemoGj_GetCollectibleType(self);
    s32 amount = DemoGj_GetCollectibleAmount(self);
    s32 i;

    for (i = 0; i < amount; i++) {
        Item_DropCollectible(play, pos, collectible);
    }
}

static inline void DemoGj_DirectedExplosion(DemoGj* self, PlayState* play, Vec3f* direction) {
    Vec3f pos;

    pos.x = self->dyna.actor.world.pos.x;
    pos.y = self->dyna.actor.world.pos.y;
    pos.z = self->dyna.actor.world.pos.z;
    FUN_003216b0(self, play, &pos, direction);
}

static inline s32 DemoGj_HasCylinderAnyExploded(DemoGj* self, PlayState* play) {
    if (DemoGj_HitByExplosion(self, play, &self->colliderCylinders[0])) {
        return true;
    }
    if (DemoGj_HitByExplosion(self, play, &self->colliderCylinders[1])) {
        return true;
    }
    if (DemoGj_HitByExplosion(self, play, &self->colliderCylinders[2])) {
        return true;
    }
    return false;
}

static inline void DemoGj_SetCylindersAsAC(DemoGj* self, PlayState* play) {
    Collider* collider0 = &self->colliderCylinders[0].base;
    Collider* collider1 = &self->colliderCylinders[1].base;
    Collider* collider2 = &self->colliderCylinders[2].base;

    CollisionCheck_SetAC(play, &play->colChkCtx, collider0);
    CollisionCheck_SetAC(play, &play->colChkCtx, collider1);
    CollisionCheck_SetAC(play, &play->colChkCtx, collider2);
}

// Moves the cylinders relative to the actor position (caseD_12).
static inline void DemoGj_MoveCylinders1(DemoGj* self, PlayState* play) {
    ColliderCylinder* collider0 = &self->colliderCylinders[0];
    ColliderCylinder* collider1 = &self->colliderCylinders[1];
    ColliderCylinder* collider2 = &self->colliderCylinders[2];
    Vec3f* actorPos = &self->dyna.actor.world.pos;
    s16 yaw = self->dyna.actor.world.rot.y;
    f32 cos = Math_CosS(yaw);
    f32 sin = Math_SinS(yaw);

    collider0->dim.pos.z = actorPos->z + (20.0f * cos) - (-20.0f * sin);
    collider0->dim.pos.x = actorPos->x + (20.0f * sin) + (-20.0f * cos);
    collider0->dim.pos.y = actorPos->y;

    collider1->dim.pos.z = actorPos->z + (-20.0f * cos) - (20.0f * sin);
    collider1->dim.pos.x = actorPos->x + (-20.0f * sin) + (20.0f * cos);
    collider1->dim.pos.y = actorPos->y;

    collider2->dim.pos.z = actorPos->z + (-60.0f * cos) - (60.0f * sin);
    collider2->dim.pos.x = actorPos->x + (-60.0f * sin) + (60.0f * cos);
    collider2->dim.pos.y = actorPos->y;
}

// Moves the cylinders relative to the actor position (caseD_13).
static inline void DemoGj_MoveCylinders2(DemoGj* self, PlayState* play) {
    ColliderCylinder* collider0 = &self->colliderCylinders[0];
    ColliderCylinder* collider1 = &self->colliderCylinders[1];
    ColliderCylinder* collider2 = &self->colliderCylinders[2];
    Vec3f* actorPos = &self->dyna.actor.world.pos;
    s16 yaw = self->dyna.actor.world.rot.y;
    f32 cos = Math_CosS(yaw);
    f32 sin = Math_SinS(yaw);

    collider0->dim.pos.z = actorPos->z - (35.0f * sin);
    collider0->dim.pos.x = actorPos->x + (35.0f * cos);
    collider0->dim.pos.y = actorPos->y;

    collider1->dim.pos.z = actorPos->z - (-10.0f * sin);
    collider1->dim.pos.x = actorPos->x + (-10.0f * cos);
    collider1->dim.pos.y = actorPos->y;

    collider2->dim.pos.z = actorPos->z - (-55.0f * sin);
    collider2->dim.pos.x = actorPos->x + (-55.0f * cos);
    collider2->dim.pos.y = actorPos->y;
}

static inline void DemoGj_UpdateRubble(DemoGj* self, PlayState* play) {
    Actor* thisx = &self->dyna.actor;

    if (DemoGj_CheckGanonMode(self, 4)) {
        Actor_Kill(thisx);
    } else if (DemoGj_HasCylinderAnyExploded(self, play)) {
        Vec3f vec1 = { 0.0f, 0.0f, 0.0f };

        DemoGj_DropCollectible(self, play);
        DemoGj_DirectedExplosion(self, play, &vec1);
        Actor_Kill(thisx);
    } else if (self->killFlag) {
        Vec3f vec2 = self->unk_2E8;

        vec2.y = 0.0f;
        DemoGj_DropCollectible(self, play);
        DemoGj_DirectedExplosion(self, play, &vec2);
        Actor_Kill(thisx);
    }
}

void caseD_12_002ae2a0(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    DemoGj_UpdateRubble(self, play);
    DemoGj_MoveCylinders1(self, play);
    DemoGj_SetCylindersAsAC(self, play);
}

void caseD_13_002ae53c(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    DemoGj_UpdateRubble(self, play);
    DemoGj_MoveCylinders2(self, play);
    DemoGj_SetCylindersAsAC(self, play);
}
#endif

#if 0 // NON_MATCHING draft: caseD_14_002ae7c4 (8 words off: VFP numbering of the zero const s0/s2 and sb/sl swap in the 2nd drop loop)
// Local prototype (not in include/): returns the explosive touching `collider`, or NULL.
Actor* Actor_GetCollidedExplosive(PlayState* play, Collider* collider);

static s32 DemoGj_GetCollectibleType(DemoGj* self) {
    s32 type = (self->dyna.actor.params >> 11) & 0x1F;

    return type;
}

static s32 DemoGj_GetCollectibleAmount(DemoGj* self) {
    s32 amount = (self->dyna.actor.params >> 8) & 0x7;

    return amount;
}

static s32 DemoGj_HitByExplosion(DemoGj* self, PlayState* play, ColliderCylinder* collider) {
    if (Actor_GetCollidedExplosive(play, &collider->base) != NULL) {
        return true;
    }
    return false;
}

static void DemoGj_DropCollectible(DemoGj* self, PlayState* play) {
    Vec3f* pos = &self->dyna.actor.world.pos;
    s16 collectible = DemoGj_GetCollectibleType(self);
    s32 amount = DemoGj_GetCollectibleAmount(self);
    s32 i;

    for (i = 0; i < amount; i++) {
        Item_DropCollectible(play, pos, collectible);
    }
}

static void DemoGj_DirectedExplosion(DemoGj* self, PlayState* play, Vec3f* direction) {
    Vec3f pos;

    pos.x = self->dyna.actor.world.pos.x;
    pos.y = self->dyna.actor.world.pos.y;
    pos.z = self->dyna.actor.world.pos.z;
    FUN_003216b0(self, play, &pos, direction);

    pos.x = self->dyna.actor.world.pos.x;
    pos.y = self->dyna.actor.world.pos.y + 100.0f;
    pos.z = self->dyna.actor.world.pos.z;
    FUN_003216b0(self, play, &pos, direction);
}

static inline void DemoGj_SetVec(Vec3f* v, f32 z, f32 x, f32 y) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void caseD_14_002ae7c4(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;
    ColliderCylinder* collider0 = &self->colliderCylinders[0];

    if (DemoGj_CheckGanonMode(self, 4)) {
        Actor_Kill(thisx);
    } else if (DemoGj_HitByExplosion(self, play, collider0)) {
        Vec3f vec;
        DemoGj_SetVec(&vec, 0.0f, 0.0f, 0.0f);
        DemoGj_DropCollectible(self, play);
        DemoGj_DirectedExplosion(self, play, &vec);
        Actor_Kill(thisx);
    } else if (self->killFlag) {
        Vec3f vec = self->unk_2E8;
        vec.y = 0.0f;
        DemoGj_DropCollectible(self, play);
        DemoGj_DirectedExplosion(self, play, &vec);
        Actor_Kill(thisx);
    }

    Collider_UpdateCylinder(thisx, collider0);
    CollisionCheck_SetAC(play, &play->colChkCtx, &collider0->base);
}
#endif

// 3DS build of N64 DemoGj_SetupMovement (ROM DemoGj_SetupMovement). Rubble pile N uses type 7 + N.
#define DEMOGJ_RAD_TO_BINANG(rad) ((s16)((rad) * (32768.0f / 3.14159265f)))

void DemoGj_SetupMovement(Actor* actor, PlayState* play) {
    DemoGj* self = (DemoGj*)actor;
    Vec3f* pos;
    Vec3s* unk_172;
    f32 xDistance;
    f32 zDistance;

    if (self->ganon != NULL) {
        pos = &actor->world.pos;
        Vec3f* ganonPos = &((Actor*)self->ganon)->world.pos;
        f32 ax = pos->x;
        xDistance = ax - ganonPos->x;
        zDistance = pos->z - ganonPos->z;
        f32* speedXZ = &actor->speedXZ;
        f32* yVelocity = &actor->velocity.y;
        unk_172 = &self->unk_1CA;
        f32* minVelocityY = &actor->minVelocityY;
        f32* gravity = &actor->gravity;
        switch ((u16)actor->params & 0xFF) {
            case 8:
                *speedXZ = kREG(16) + 10.0f;
                *yVelocity = kREG(17) + 40.0f;
                unk_172->x = kREG(18);
                unk_172->y = kREG(19) + 0x3E8;
                unk_172->z = kREG(20) + 0xBB8;
                *minVelocityY = kREG(21) * 0.01f + -29.0f;
                *gravity = kREG(22) * 0.01f + -5.0f;
                break;

            case 9:
                *speedXZ = kREG(29) + 10.0f;
                *yVelocity = kREG(30) + 40.0f;
                unk_172->x = kREG(31);
                unk_172->y = kREG(32) + 0x3E8;
                unk_172->z = kREG(33) + 0xBB8;
                *minVelocityY = kREG(34) * 0.01f + -29.0f;
                *gravity = kREG(35) * 0.01f + -5.0f;
                break;

            case 10:
                *speedXZ = kREG(42) + 10.0f;
                *yVelocity = kREG(43) + 40.0f;
                unk_172->x = kREG(44);
                unk_172->y = kREG(45) + 0x3E8;
                unk_172->z = kREG(46) + 0xBB8;
                *minVelocityY = kREG(47) * 0.01f + -29.0f;
                *gravity = kREG(48) * 0.01f + -5.0f;
                break;

            case 11:
                *speedXZ = kREG(55) + 10.0f;
                *yVelocity = kREG(56) + 40.0f;
                unk_172->x = kREG(57);
                unk_172->y = kREG(58) + 0x3E8;
                unk_172->z = kREG(59) + 0xBB8;
                *minVelocityY = kREG(60) * 0.01f + -29.0f;
                *gravity = kREG(61) * 0.01f + -5.0f;
                break;

            case 12:
                *speedXZ = kREG(68) + 10.0f;
                *yVelocity = kREG(69) + 40.0f;
                unk_172->x = kREG(70);
                unk_172->y = kREG(71) + 0x3E8;
                unk_172->z = kREG(72) + 0xBB8;
                *minVelocityY = kREG(73) * 0.01f + -29.0f;
                *gravity = kREG(74) * 0.01f + -5.0f;
                break;

            case 13:
                *speedXZ = kREG(81) + 10.0f;
                *yVelocity = kREG(82) + 40.0f;
                unk_172->x = kREG(83);
                unk_172->y = kREG(84) + 0x3E8;
                unk_172->z = kREG(85) + 0xBB8;
                *minVelocityY = kREG(86) * 0.01f + -29.0f;
                *gravity = kREG(87) * 0.01f + -5.0f;
                break;

            case 14:
                *speedXZ = kREG(94) + 10.0f;
                *yVelocity = kREG(95) + 70.0f;
                unk_172->x = kREG(15);
                unk_172->y = kREG(14) + 0x3E8;
                unk_172->z = kREG(13) + 0xBB8;
                *minVelocityY = kREG(12) * 0.01f + -29.0f;
                *gravity = kREG(11) * 0.01f + -5.0f;
                break;
        }

        if ((xDistance == 0.0f) && (zDistance == 0.0f)) {
            Player* player = GET_PLAYER(play);

            Vec3f* playerPos = &player->actor.world.pos;

            xDistance = playerPos->x - pos->x;
            zDistance = playerPos->z - pos->z;

            if ((xDistance != 0.0f) || (zDistance != 0.0f)) {
                actor->world.rot.y = DEMOGJ_RAD_TO_BINANG(Math_FAtan2F(xDistance, zDistance));
            }
        } else {
            actor->world.rot.y = DEMOGJ_RAD_TO_BINANG(Math_FAtan2F(xDistance, zDistance));
        }
    }
}

void caseD_e_0037d06c(Actor* thisx, PlayState* play) {
    DemoGj* self = (DemoGj*)thisx;

    Actor_MoveForward(thisx);
    self->rotationVec.x += kREG(15);
    self->rotationVec.y += (s16)(kREG(14) + 1000);
    self->rotationVec.z += (s16)(kREG(13) + 3000);

    DemoGj_SetupRotation(self, play);

    if (DemoGj_IsGanondorfFloatingInAir(self)) {
        Actor_Kill(thisx);
    }
}

}
