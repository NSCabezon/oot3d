extern "C" {
#include "z_en_lightbox.hpp"
#include "regs.hpp"

#define FLAGS 0x80000010

#define NA_SE_EV_BOMB_BOUND 0x01000186
#define BGCHECKFLAG_GROUND 0x0001
#define BGCHECKFLAG_GROUND_TOUCH 0x0002
#define BGCHECKFLAG_WALL 0x0008

void FUN_0034df30(Actor* actor, PlayState* play); // Actor_OfferCarry

void EnLightbox_Init(Actor* actor, GameState* state);
void EnLightbox_Destroy(Actor* actor, GameState* state);
void EnLightbox_Update(Actor* actor, GameState* state);
void EnLightbox_Draw(Actor* actor, GameState* state);

ActorInit En_Lightbox_InitVars = {
    ACTOR_EN_LIGHTBOX,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_LIGHTBOX,
    sizeof(EnLightbox),
    (ActorFunc)EnLightbox_Init,
    (ActorFunc)EnLightbox_Destroy,
    (ActorFunc)EnLightbox_Update,
    (ActorFunc)EnLightbox_Draw,
};

void EnLightbox_Init(Actor* actor, GameState* state) {
    EnLightbox* self = (EnLightbox*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    f32 yOffset = 0.0f;

    switch (actor->params) {
        case 0:
            Actor_SetScale(actor, 0.025f);
            break;
        case 1:
            Actor_SetScale(actor, 0.05f);
            break;
        case 2:
            Actor_SetScale(actor, 0.075f);
            break;
        case 3:
            Actor_SetScale(actor, 0.1f);
        default:
            break;
    }

    actor->focus.pos = actor->world.pos;
    actor->colChkInfo.cylRadius = 30;
    actor->colChkInfo.cylHeight = 50;
    ActorShape_Init(&actor->shape, yOffset, ActorShadow_DrawCircle, 6.0f);
    self->dyna.unk_1B8 = 0;
    self->dyna.unk_1B4 = 0;
    actor->targetMode = 0;
    actor->gravity = -2.0f;
    FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 3), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
}

void EnLightbox_Destroy(Actor* actor, GameState* state) {
    EnLightbox* self = (EnLightbox*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

// NON_MATCHING draft (kept as GLOBAL_ASM below): identical logic, but armcc if-converts the
// `speedXZ != 0 && (bgCheckFlags & WALL)` test into predicated ldrhne/tstne, while the ROM keeps two branches.
#if 0
void EnLightbox_Update(Actor* actor, GameState* state) {
    EnLightbox* self = (EnLightbox*)actor;
    PlayState* play = (PlayState*)state;

    if (self->dyna.unk_1BA != 0) {
        if (Actor_HasNoParent(actor, play)) {
            self->dyna.unk_1BA = 0;
        }
    } else {
        if (Actor_HasParent(actor, play)) {
            self->dyna.unk_1BA++;
        } else {
            if (actor->speedXZ) {
                if (actor->bgCheckFlags & BGCHECKFLAG_WALL) {
                    actor->world.rot.y = actor->wallYaw;
                    Audio_PlaySoundGeneral(NA_SE_EV_BOMB_BOUND, &actor->world.pos, 4, &FLOAT_0054ac20,
                                           &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                    actor->speedXZ *= 0.7f;
                    actor->bgCheckFlags &= ~BGCHECKFLAG_WALL;
                }
            }

            if (!(actor->bgCheckFlags & BGCHECKFLAG_GROUND)) {
                Math_StepToF(&actor->speedXZ, 0, IREG(57) / 100.0f);
            } else {
                Math_StepToF(&actor->speedXZ, 0, IREG(58) / 100.0f);
                if ((actor->bgCheckFlags & BGCHECKFLAG_GROUND_TOUCH) && (actor->velocity.y < IREG(59) / 100.0f)) {
                    Audio_PlaySoundGeneral(NA_SE_EV_BOMB_BOUND, &actor->world.pos, 4, &FLOAT_0054ac20,
                                           &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
                    actor->velocity.y *= IREG(60) / 100.0f;
                    actor->bgCheckFlags &= ~BGCHECKFLAG_GROUND;
                } else {
                    FUN_0034df30(actor, play);
                }
            }
        }
    }
    Actor_MoveForward(actor);
    Actor_UpdateBgCheckInfo(play, actor, actor->colChkInfo.cylHeight, actor->colChkInfo.cylRadius,
                            actor->colChkInfo.cylRadius, 0x1D);
    actor->focus.pos = actor->world.pos;
}
#endif

GLOBAL_ASM("asm/EnLightbox_Update.s")

void EnLightbox_Draw(Actor* actor, GameState* state) {
    EnLightbox* self = (EnLightbox*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->skelAnimModel->FUN_00372170(0);
    }
}
}
